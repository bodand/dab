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
 * src/core/public/dab/db_command --
 *   The command types provided by dab.
 *   They provide the different functionality provided by the different SQL
 *   sub-languages.
 */
#ifndef DAB_DB_COMMAND_HXX
#define DAB_DB_COMMAND_HXX

#include <memory>
#include <stdexcept>

#include <tl/generator.hpp>

#include <async/task.h>

#include "dab/db_result_row.hxx"

namespace dab {
    struct db_locking_exception : std::runtime_error {
        db_locking_exception() : std::runtime_error("could not acquire required locks on database") { }
    };

    struct db_error_exception : std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    struct db_dql_command {
        db_dql_command(const db_dql_command& cp) = delete;
        db_dql_command(db_dql_command&& mv) noexcept = delete;

        db_dql_command&
        operator=(const db_dql_command& cp) = delete;
        db_dql_command&
        operator=(db_dql_command&& mv) noexcept = delete;

        virtual ~db_dql_command() noexcept = default;

        template<class... Fields>
        [[nodiscard]] tl::generator<std::tuple<Fields...>>
        execute_to_tuple() {
            for (const auto& row : do_execute()) {
                if (!row->has_row()) break;

                auto ret = row->template get_as_tuple<Fields...>();
                co_yield ret;
            }
        }

        [[nodiscard]] tl::generator<const db_result_row&>
        execute() {
            for (const auto& row : do_execute()) {
                if (!row->has_row()) break;

                const auto& ret = *row;
                co_yield ret;
            }
        }

    protected:
        [[nodiscard]] virtual tl::generator<std::unique_ptr<db_result_row>>
        do_execute() = 0;

        db_dql_command() noexcept = default;
    };

    struct db_dml_command {
        db_dml_command(const db_dml_command& cp) = delete;
        db_dml_command(db_dml_command&& mv) noexcept = delete;

        db_dml_command&
        operator=(const db_dml_command& cp) = delete;
        db_dml_command&
        operator=(db_dml_command&& mv) noexcept = delete;

        virtual ~db_dml_command() noexcept = default;

        virtual async::task<std::size_t>
        execute() = 0;

        template<class... Fields>
        [[nodiscard]] tl::generator<std::tuple<Fields...>>
        execute_to_tuple() {
            for (auto row : co_await do_execute_returning()) {
                co_yield row->template get_as_tuple<Fields...>();
            }
        }

    protected:
        virtual tl::generator<std::unique_ptr<db_result_row>>
        do_execute_returning() = 0;

        db_dml_command() noexcept = default;
    };

    struct db_ddl_command {
        db_ddl_command(const db_ddl_command& cp) = delete;
        db_ddl_command(db_ddl_command&& mv) noexcept = delete;

        db_ddl_command&
        operator=(const db_ddl_command& cp) = delete;
        db_ddl_command&
        operator=(db_ddl_command&& mv) noexcept = delete;

        virtual ~db_ddl_command() noexcept = default;

        virtual async::task<void>
        execute() = 0;

    protected:
        db_ddl_command() noexcept = default;
    };

    struct db_dcl_command {
        db_dcl_command(const db_dcl_command& cp) = delete;
        db_dcl_command(db_dcl_command&& mv) noexcept = delete;

        db_dcl_command&
        operator=(const db_dcl_command& cp) = delete;
        db_dcl_command&
        operator=(db_dcl_command&& mv) noexcept = delete;

        virtual ~db_dcl_command() noexcept = default;

        virtual async::task<void>
        execute() = 0;

    protected:
        db_dcl_command() noexcept = default;
    };
}

#endif
