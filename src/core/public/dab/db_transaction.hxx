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
 * src/core/public/dab/db_transaction --
 *   A db_transaction represents a database transaction.
 */

#ifndef DAB_DB_TRANSACTION_HXX
#define DAB_DB_TRANSACTION_HXX

#include <cassert>
#include <stdexcept>

#include <async/task.h>

#include <dab/fwd/db_connection.hxx>
#include <dab/isolation_level.hxx>

namespace dab {
    struct transaction_finished_exception : std::runtime_error {
        explicit transaction_finished_exception()
             : std::runtime_error("transaction already committed/rolled back") { }
    };

    struct db_transaction {
        db_transaction(const db_transaction& cp) = delete;
        db_transaction(db_transaction&& mv) noexcept = delete;
        db_transaction&
        operator=(const db_transaction& cp) = delete;
        db_transaction&
        operator=(db_transaction&& mv) noexcept = delete;

        virtual ~db_transaction() = default;

        isolation_level
        get_isolation_level() const noexcept {
            return _isolation_level;
        }

        const db_connection*
        get_connection() const noexcept {
            return _connection;
        }

        [[nodiscard]] bool
        is_finished() const noexcept { return _finished; }

        async::task<void>
        commit() {
            if (_finished) throw transaction_finished_exception();
            co_await do_commit();
            _finished = true;
        }

        async::task<void>
        rollback() {
            if (_finished) throw transaction_finished_exception();
            co_await do_rollback();
            _finished = true;
        }

    protected:
        explicit db_transaction(db_connection* connection, isolation_level lvl)
             : _connection(connection),
               _isolation_level(lvl) {
            assert(_connection != nullptr);
        }

        virtual async::task<void>
        do_commit() = 0;

        virtual async::task<void>
        do_rollback() = 0;

    private:
        db_connection* _connection;
        isolation_level _isolation_level;
        bool _finished = false;
    };
}

#endif
