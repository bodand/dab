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
 * src/sqlite/test/connection_string_builder --
 *   Tests for the connection string builder for the Sqlite DAB backend.
 */

#include <algorithm>
#include <ranges>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <daB/sqlite/sqlite_connection_string_builder.hxx>
#include <magic_enum.hpp>

#include <fmt/format.h>

using Catch::Matchers::ContainsSubstring;

TEST_CASE("Sqlite connection string builder sets path to db in connection string") {
    dab::sqlite_connection_string_builder string_builder;

    constexpr const static auto path = "test-path.db";
    string_builder.set_path(path);

    CHECK_THAT(string_builder.connection_string(),
               ContainsSubstring(path));
}


TEST_CASE("Sqlite connection string builder sets vfs implementation in connection string") {
    dab::sqlite_connection_string_builder string_builder;

    constexpr const static auto path = "unix-dot";
    string_builder.set_vfs(path);

    CHECK_THAT(string_builder.connection_string(),
               ContainsSubstring(fmt::format("vfs={}", path)));
}

TEST_CASE("Sqlite connection string builder sets connection mode in connection string") {
    std::ranges::for_each(magic_enum::enum_entries<dab::sqlite_connection_mode>(),
                          [&](auto&& entry) {
                              auto&& [val, name] = std::forward<decltype(entry)>(entry);
                              dab::sqlite_connection_string_builder string_builder;
                              string_builder.set_mode(std::forward<decltype(val)>(val));
                              CHECK_THAT(string_builder.connection_string(), ContainsSubstring("mode="));
                          });
}

TEST_CASE("Sqlite connection string builder sets cache in connection string") {
    std::ranges::for_each(magic_enum::enum_entries<dab::sqlite_cache_behavior>(),
                          [&](auto&& entry) {
                              auto&& [val, name] = std::forward<decltype(entry)>(entry);
                              dab::sqlite_connection_string_builder string_builder;
                              string_builder.set_cache_behavior(std::forward<decltype(val)>(val));
                              CHECK_THAT(string_builder.connection_string(), ContainsSubstring("cache="));
                          });
}

TEST_CASE("Sqlite connection string builder sets locking in connection string") {
    SECTION("with locking disabled") {
        dab::sqlite_connection_string_builder string_builder;
        string_builder.set_locking(false);
        CHECK_THAT(string_builder.connection_string(), ContainsSubstring("nolock=1"));
    }

    SECTION("with locking enabled") {
        dab::sqlite_connection_string_builder string_builder;
        string_builder.set_locking(true);
        CHECK_THAT(string_builder.connection_string(), ContainsSubstring("nolock=0"));
    }
}

TEST_CASE("Sqlite connection string builder sets powersafe override mode in connection string") {
    SECTION("with psow disabled") {
        dab::sqlite_connection_string_builder string_builder;
        string_builder.set_powersafe_overwrite(false);
        CHECK_THAT(string_builder.connection_string(), ContainsSubstring("psow=0"));
    }

    SECTION("with psow enabled") {
        dab::sqlite_connection_string_builder string_builder;
        string_builder.set_powersafe_overwrite(true);
        CHECK_THAT(string_builder.connection_string(), ContainsSubstring("psow=1"));
    }
}

TEST_CASE("Sqlite connection string builder sets immutable mode in connection string") {
    SECTION("with psow disabled") {
        dab::sqlite_connection_string_builder string_builder;
        string_builder.set_immutable(false);
        CHECK_THAT(string_builder.connection_string(), ContainsSubstring("immutable=0"));
    }

    SECTION("with psow enabled") {
        dab::sqlite_connection_string_builder string_builder;
        string_builder.set_immutable(true);
        CHECK_THAT(string_builder.connection_string(), ContainsSubstring("immutable=1"));
    }
}

TEST_CASE("Sqlite connection string builder sets journal mode in connection string") {
    std::ranges::for_each(magic_enum::enum_entries<dab::sqlite_journal_mode>(),
                          [&](auto&& entry) {
                              auto&& [val, name] = std::forward<decltype(entry)>(entry);
                              dab::sqlite_connection_string_builder string_builder;
                              string_builder
                                     .set_path("asd")
                                     .set_journaling(std::forward<decltype(val)>(val));
                              CHECK_THAT(string_builder.connection_string(),
                                         ContainsSubstring(fmt::format("Journal Mode={}", name)));
                          });
}

TEST_CASE("Sqlite connection string builder sets multiple settings in connection string") {
    dab::sqlite_connection_string_builder string_builder;
    string_builder
           .set_locking(true)
           .set_immutable(true);
    CHECK_THAT(string_builder.connection_string(),
               ContainsSubstring("immutable=1")
                      && ContainsSubstring("nolock=0"));
}

TEST_CASE("Sqlite connection string builder can query elements") {
    dab::sqlite_connection_string_builder string_builder;
    string_builder.set_locking(true);
    CHECK(string_builder.get("nolock") == "0");
}

TEST_CASE("Sqlite connection string builder throws on querying non-existent elements") {
    dab::sqlite_connection_string_builder string_builder;
    CHECK_THROWS_AS(string_builder.get("nolock"), std::out_of_range);
}
