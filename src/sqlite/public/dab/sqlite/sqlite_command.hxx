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
 * src/sqlite/public/dab/sqlite/sqlite_command --
 *   
 */

#ifndef DAB_SQLITE_COMMAND_HXX
#define DAB_SQLITE_COMMAND_HXX

#include <dab/db_command.hxx>
#include <tl/generator.hpp>

namespace dab {
    struct sqlite_dql_command final : db_dql_command {
        sqlite_dql_command(void* db_raw, std::string_view sql);

        ~sqlite_dql_command() noexcept override;

    protected:
        tl::generator<std::unique_ptr<db_result_row>>
        do_execute() override;

    private:
        void* _dql;
    };

    struct sqlite_dml_command final : db_dml_command {
        sqlite_dml_command(void* db_raw, std::string_view sql);

        async::task<std::size_t>
        execute() override;

        ~sqlite_dml_command() noexcept override;

    protected:
        tl::generator<std::unique_ptr<db_result_row>>
        do_execute_returning() override;

    private:
        void* _db;
        void* _dml;
    };

    struct sqlite_dcl_command final : db_dcl_command {
        sqlite_dcl_command(void* db_raw, std::string_view sql);

        async::task<void>
        execute() override;

        ~sqlite_dcl_command() noexcept override;

    private:
        void* _dcl;
    };

    struct sqlite_ddl_command final : db_ddl_command {
        sqlite_ddl_command(void* db_raw, std::string_view sql);

        async::task<void>
        execute() override;

        ~sqlite_ddl_command() noexcept override;

    private:
        void* _ddl;
    };
}

#endif
