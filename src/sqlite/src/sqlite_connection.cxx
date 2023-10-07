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
 * src/sqlite/src/sqlite_connection --
 *   
 */

#include <memory>
#include <string_view>
#include <tuple>

#include <dab/db_command.hxx>
#include <dab/db_connection.hxx>
#include <dab/db_transaction.hxx>
#include <dab/isolation_level.hxx>
#include <dab/sqlite/connection_string.hxx>
#include <dab/sqlite/sqlite_command.hxx>
#include <dab/sqlite/sqlite_connection.hxx>
#include <dab/sqlite/sqlite_transaction.hxx>

#include <fmt/format.h>
#include <sqlite3.h>

std::unique_ptr<dab::db_transaction>
dab::sqlite_connection::create_transaction(isolation_level lvl) {
    std::ignore = lvl; // SQLite always runs in serializable mode
    return std::make_unique<sqlite_transaction>(this);
}

std::unique_ptr<dab::db_dql_command>
dab::sqlite_connection::query(std::string_view query) {
    return std::make_unique<sqlite_dql_command>(_db, query);
}

std::unique_ptr<dab::db_dml_command>
dab::sqlite_connection::modify(std::string_view query) {
    return std::make_unique<sqlite_dml_command>(_db, query);
}

std::unique_ptr<dab::db_ddl_command>
dab::sqlite_connection::define(std::string_view query) {
    return std::make_unique<sqlite_ddl_command>(_db, query);
}

std::unique_ptr<dab::db_dcl_command>
dab::sqlite_connection::control(std::string_view query) {
    return std::make_unique<sqlite_dcl_command>(_db, query);
}

dab::sqlite_connection::sqlite_connection(std::string_view conn_string) {
    auto cfg = dab::sqlite::parse_to_value_set(conn_string);

    auto uri = [&cfg]() {
        if (auto it = cfg.find("Uri");
            it != cfg.end()) {
            return it->second;
        }
        throw db_connection_exception("sqlite connection requires Uri set in connection string");
    }();

    sqlite3* db;
    if (auto res = sqlite3_open_v2(uri.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, nullptr);
        !db || res != SQLITE_OK) {
        if (!db) {
            throw db_native_connection_exception("cannot connect to or create SQLite database: check memory?");
        }
        else {
            std::string_view errmsg = sqlite3_errmsg(db);
            sqlite3_free(db);
            throw db_native_connection_exception(fmt::format(FMT_STRING("sqlite exception: {}"), errmsg));
        }
    }
    _db = db;

    if (auto journal = cfg.find("Journal Mode");
        journal != cfg.end()) {
        sqlite3_exec(db, fmt::format(FMT_STRING("PRAGMA journal_mode={};"), cfg["Journal Mode"]).c_str(), nullptr, nullptr, nullptr);
    }
}

dab::sqlite_connection::~sqlite_connection() noexcept {
    if (_db) {
        auto* db = static_cast<sqlite3*>(_db);
        sqlite3_close(db);
    }
}
