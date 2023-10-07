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
 * src/sqlite/src/sqlite_transaction --
 *   
 */

#include <dab/sqlite/sqlite_transaction.hxx>

#include "dab/db_command.hxx"
#include "dab/db_connection.hxx"

async::task<void>
dab::sqlite_transaction::do_commit() {
    auto cmd = _connection.query("COMMIT;");
    for ([[maybe_unused]] const auto& _ : cmd->execute()) {
        continue;
    }
    co_return;
}

async::task<void>
dab::sqlite_transaction::do_rollback() {
    auto cmd = _connection.query("ROLLBACK;");
    for ([[maybe_unused]] const auto& _ : cmd->execute()) {
        continue;
    }
    co_return;
}


dab::sqlite_transaction::sqlite_transaction(dab::sqlite_connection* connection)
     : db_transaction(connection, isolation_level::SERIALIZABLE),
       _connection(*connection) {
    auto cmd = _connection.query("BEGIN TRANSACTION;");
    for ([[maybe_unused]] const auto& _ : cmd->execute()) {
        continue;
    }
}
