/* dab project
 *
 * Copyright (c) 2023 András Bodor <bodand@pm.me>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * - Neither the name of the copyright holder nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Originally created: 2023-09-29.
 *
 * src/core/public/dab/typing/db_value --
 *   Type for handling changing untyped database values into C++ types.
 *   The implementation uses an heavily micro-optimized acyclic visitor.
 */

#ifndef DAB_DB_VALUE_HXX
#define DAB_DB_VALUE_HXX

#include <concepts>
#include <stdexcept>
#include <type_traits>

#include "dab/typing/type_id.hpp"

namespace dab {
    struct db_invalid_type_exception : std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    struct db_value {
        virtual ~db_value() noexcept = default;

        template<class T>
        T
        get_as()
            requires(std::default_initializable<T>)
        {
            if (T ret;
                retrieve_as(&ret)) return ret;
            throw db_invalid_type_exception("db_value is requested as incompatible type");
        }

        template<class T>
        bool
        retrieve_as(T* result) {
            return do_get_as(static_cast<void*>(result), type_id::id_of<T>());
        }

    protected:
        virtual bool
        do_get_as(void* result, type_id type) = 0;
    };

    template<class T>
    struct db_typed_value_impl : virtual db_value {
        virtual void
        do_get_into(T* result) = 0;
    };

    template<class... Ts>
    struct db_typed_value : db_typed_value_impl<Ts>... {
    protected:
        bool
        do_get_as(void* result, type_id type) final {
            return (try_get<Ts>(result, type) || ...);
        }

    private:
        template<class T>
        bool
        try_get(void* result, type_id type) {
            if (type_id::id_of<T>() != type) return false;
            static_cast<db_typed_value_impl<T>*>(this)->do_get_into(static_cast<T*>(result));
            return true;
        }
    };
}

#endif
