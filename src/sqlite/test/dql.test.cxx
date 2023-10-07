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
 * Originally created: 2023-10-02.
 *
 * src/sqlite/test/dql --
 *   Tests for DQL commands of Dab.Sqlite.
 */

#include <algorithm>
#include <ranges>

#include <catch2/catch_test_macros.hpp>
#include <dab/coro/await.hxx>
#include <dab/db_command.hxx>
#include <dab/sqlite/sqlite_connection.hxx>

TEST_CASE("dql command can query into row objects") {
    dab::sqlite_connection conn("Uri=file:test.db?mode=rw");
    for (auto check = conn.query(R"(
SELECT *
  FROM "department")");
         const auto& row : check->execute()) {
        CHECK(row.get_column_count() == 2);
    }
}

TEST_CASE("dql command can query into tuple objects") {
    dab::sqlite_connection conn("Uri=file:test.db?mode=rw");
    for (auto check = conn.query(R"(
SELECT "id", "name"
  FROM "department")");
         const auto& [id, name] : check->execute_to_tuple<long long, std::string>()) {
        CHECK(id > 0);
        CHECK_FALSE(name.empty());
    }
}

TEST_CASE("dql throws on invalid result tuple arity") {
    dab::sqlite_connection conn("Uri=file:test.db?mode=rw");
    auto check = conn.query(R"(
SELECT "id", "name"
  FROM "department")");
    CHECK_THROWS_AS((std::ranges::for_each(check->execute_to_tuple<int, std::string, int>(), [](auto&& ignore) {
                        std::ignore = ignore;
                    })),
                    dab::db_result_invalid_arity);
}

TEST_CASE("dql throws on incompatible result type") {
    dab::sqlite_connection conn("Uri=file:test.db?mode=rw");
    auto check = conn.query(R"(
SELECT "id", "name"
  FROM "department")");
    CHECK_THROWS_AS((std::ranges::for_each(check->execute_to_tuple<int, int>(), [](auto&& ignore) {
                        std::ignore = ignore;
                    })),
                    dab::db_invalid_type_exception);
}
