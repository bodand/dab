/* dab project
 *
 * Originally created: 2023-10-02.
 *
 * src/core/public/dab/coro/await --
 *   Code snippeted together as a work-around for clang-cl borking on the original
 *   awaitable_resume_t.h. I have added nothing of value to this file.
 */

#ifndef DAB_AWAIT_HXX
#define DAB_AWAIT_HXX


#include <coroutine>
#include <exception>
#include <type_traits>

#include <async/awaitable_result.h>
#include <async/event_signal.h>


namespace async::details {
    template<typename T>
    struct co_await_operator_or_self final {
        using type = T;
    };

    template<typename T>
        requires(requires(T awaitable) { awaitable.operator co_await(); })
    struct co_await_operator_or_self<T> final {
        using type = decltype(std::declval<T>().operator co_await());
    };

    template<typename T>
        requires(requires(T awaitable) { operator co_await(awaitable); })
    struct co_await_operator_or_self<T> final {
        using type = decltype(operator co_await(std::declval<T>()));
    };

    template<typename T>
    using co_await_t = typename co_await_operator_or_self<T>::type;

    template<typename T>
    struct awaitable_resume final {
        using type = decltype(std::declval<co_await_t<T>>().await_resume());
    };
}

namespace async {
    template<typename T>
    using awaitable_resume_t = typename details::awaitable_resume<T>::type;
}

// © Microsoft Corporation. All rights reserved.
namespace async::details {
    template<typename T>
    struct get_task_promise;

    template<typename T>
    struct get_task final {
        using promise_type = get_task_promise<T>;

        explicit get_task(std::coroutine_handle<get_task_promise<T>> handle) : m_handle{handle} { }

        get_task(const get_task&) = delete;
        get_task(get_task&&) noexcept = delete;

        ~get_task() noexcept { m_handle.destroy(); }

        get_task&
        operator=(const get_task&) = delete;
        get_task&
        operator=(get_task&&) noexcept = delete;

        T
        get() const;

    private:
        std::coroutine_handle<get_task_promise<T>> m_handle;
    };

    struct get_task_final_suspend final {
        constexpr get_task_final_suspend(event_signal& done) noexcept : m_done{done} { }

        [[nodiscard]] constexpr bool
        await_ready() const noexcept { return false; }

        void
        await_suspend(std::coroutine_handle<>) const noexcept { m_done.set(); }

        constexpr void
        await_resume() const noexcept { }

    private:
        event_signal& m_done;
    };

    template<typename T>
    struct get_task_promise final {
        get_task_promise() : m_result{}, m_done{} { }

        get_task<T>
        get_return_object() {
            return get_task<T>{std::coroutine_handle<get_task_promise<T>>::from_promise(*this)};
        }

        constexpr std::suspend_never
        initial_suspend() const noexcept { return {}; }

        constexpr get_task_final_suspend
        final_suspend() noexcept { return {m_done}; }

        void
        unhandled_exception() noexcept { m_result.set_exception(std::current_exception()); }

        void
        return_value(T value) noexcept { m_result.set_value(std::forward<T>(value)); }

        T
        get() {
            m_done.wait();
            return m_result();
        }

    private:
        awaitable_result<T> m_result;
        event_signal m_done;
    };

    template<>
    struct get_task_promise<void> final {
        get_task<void>
        get_return_object() {
            return get_task<void>{std::coroutine_handle<get_task_promise<void>>::from_promise(*this)};
        }

        constexpr std::suspend_never
        initial_suspend() const noexcept { return {}; }

        constexpr get_task_final_suspend
        final_suspend() noexcept { return {m_done}; }

        void
        unhandled_exception() noexcept { m_resultException = std::current_exception(); }

        constexpr void
        return_void() const noexcept { }

        void
        get() const {
            m_done.wait();

            if (m_resultException) {
                std::rethrow_exception(m_resultException);
            }
        }

    private:
        std::exception_ptr m_resultException;
        event_signal m_done;
    };

    template<typename T>
    T
    get_task<T>::get() const {
        return m_handle.promise().get();
    }
}

namespace async {
    template<typename Awaitable>
    inline awaitable_resume_t<Awaitable>
    awaitable_get(Awaitable awaitable) {
        using T = awaitable_resume_t<Awaitable>;

        struct factory final {
            static details::get_task<T>
            create(Awaitable awaitable) { co_return co_await std::move(awaitable); }
        };

        return factory::create(std::move(awaitable)).get();
    }
}

#endif
