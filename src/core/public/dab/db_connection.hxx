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
 * src/core/public/dab/db_connection --
 *   A db_connection represents a connection to a given database.
 *   Each backend provides their own implementation.
 */

#ifndef DAB_DB_CONNECTION_HXX
#define DAB_DB_CONNECTION_HXX

#include <memory>
#include <stdexcept>
#include <string_view>

#include <dab/fwd/db_command.hxx>
#include <dab/fwd/db_transaction.hxx>
#include <dab/isolation_level.hxx>

namespace dab {
    struct db_connection_exception : std::runtime_error {
        using std::runtime_error::runtime_error;
    };
    struct db_native_connection_exception : db_connection_exception {
        using db_connection_exception::db_connection_exception;
    };

    struct db_connection {
        db_connection(const db_connection& cp) = delete;
        db_connection(db_connection&& mv) noexcept = delete;

        db_connection&
        operator=(const db_connection& cp) = delete;
        db_connection&
        operator=(db_connection&& mv) noexcept = delete;

        virtual ~db_connection() noexcept = default;

        virtual std::unique_ptr<db_transaction>
        create_transaction(isolation_level lvl = isolation_level::READ_COMMITTED) = 0;

        virtual std::unique_ptr<db_dql_command>
        query(std::string_view query) = 0;

        virtual std::unique_ptr<db_dml_command>
        modify(std::string_view query) = 0;

        virtual std::unique_ptr<db_ddl_command>
        define(std::string_view query) = 0;

        virtual std::unique_ptr<db_dcl_command>
        control(std::string_view query) = 0;

    protected:
        db_connection() noexcept = default;
    };
}

#endif
