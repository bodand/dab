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
 * Originally created: 2023-10-01.
 *
 * src/sqlite/test/ddl --
 *   Tests about the DDL execution functionality of Dab.Sqlite.
 */

#include <catch2/catch_test_macros.hpp>
#include <dab/coro/await.hxx>
#include <dab/db_command.hxx>
#include <dab/sqlite/sqlite_connection.hxx>

TEST_CASE("ddl command can create new table") {
    dab::sqlite_connection conn("Uri=file:test_dirty.db?mode=rw");
    auto cmd = conn.define(R"(CREATE TABLE "test" ( "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT );)");
    async::awaitable_get(cmd->execute());


    for (auto check = conn.query(R"(
SELECT "name"
  FROM "sqlite_master"
 WHERE "type" = 'table')");
         const auto& [name] : check->execute_to_tuple<std::string>()) {
        if (name == "test") {
            SUCCEED();
            return;
        }
    }
    FAIL("\"test\" table did not get inserted");
}

TEST_CASE("ddl command can delete table") {
    dab::sqlite_connection conn("Uri=file:test_dirty.db?mode=rw");
    auto cmd = conn.define(R"(DROP TABLE "department";)");
    async::awaitable_get(cmd->execute());

    for (auto check = conn.query(R"(
SELECT "name"
  FROM "sqlite_master"
 WHERE "type" = 'table')");
         const auto& [name] : check->execute_to_tuple<std::string>()) {
        if (name == "department") {
            FAIL("\"department\" was not deleted");
            return;
        }
    }
    SUCCEED();
}
