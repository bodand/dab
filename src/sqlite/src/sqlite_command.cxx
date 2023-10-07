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
 * src/sqlite/src/sqlite_command --
 *   
 */

#include "dab/sqlite/sqlite_command.hxx"

#include <cassert>

#include <sqlite3.h>

#include "dab/sqlite/sqlite_result_row.hxx"

// DQL ////////////////////////////////////////////////////////////////////////////////////////////

tl::generator<std::unique_ptr<dab::db_result_row>>
dab::sqlite_dql_command::do_execute() {
    assert(_dql);

    auto* stmt = static_cast<sqlite3_stmt*>(_dql);
    while (true) {
        auto res = sqlite3_step(stmt);
        switch (res) {
        case SQLITE_ROW: {
            std::unique_ptr<dab::db_result_row> ret = std::make_unique<sqlite_result_row>(stmt);
            co_yield ret;
            break;
        }
        case SQLITE_DONE: {
            goto end;
        }
        case SQLITE_BUSY:
            throw db_locking_exception();
        default:
            throw db_error_exception(sqlite3_errmsg(sqlite3_db_handle(stmt)));
        }
    }

end:
    std::unique_ptr<dab::db_result_row> ret = std::make_unique<sqlite_result_row>();
    co_yield ret;
}

dab::sqlite_dql_command::sqlite_dql_command(void* db_raw, std::string_view sql) {
    assert(db_raw);

    auto* db = static_cast<sqlite3*>(db_raw);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.data(), static_cast<int>(sql.size()), &stmt, nullptr);
    _dql = stmt;

    assert(_dql);
}

dab::sqlite_dql_command::~sqlite_dql_command() noexcept {
    assert(_dql);
    auto* stmt = static_cast<sqlite3_stmt*>(_dql);
    sqlite3_finalize(stmt);
}

// DML ////////////////////////////////////////////////////////////////////////////////////////////

async::task<std::size_t>
dab::sqlite_dml_command::execute() {
    assert(_db);
    assert(_dml);

    auto* db = static_cast<sqlite3*>(_db);
    auto* stmt = static_cast<sqlite3_stmt*>(_dml);
    auto res = sqlite3_step(stmt);
    switch (res) {
    case SQLITE_DONE: {
        co_return static_cast<std::size_t>(sqlite3_changes64(db));
    }
    case SQLITE_BUSY:
        throw db_locking_exception();
    default:
        throw db_error_exception(sqlite3_errmsg(sqlite3_db_handle(stmt)));
    }
}

tl::generator<std::unique_ptr<dab::db_result_row>>
dab::sqlite_dml_command::do_execute_returning() {
    assert(_db);
    assert(_dml);

    auto* stmt = static_cast<sqlite3_stmt*>(_dml);
    while (true) {
        auto res = sqlite3_step(stmt);
        switch (res) {
        case SQLITE_ROW: {
            std::unique_ptr<dab::db_result_row> ret = std::make_unique<sqlite_result_row>(stmt);
            co_yield ret;
            break;
        }
        case SQLITE_DONE: {
            goto end;
        }
        case SQLITE_BUSY:
            throw db_locking_exception();
        default:
            throw db_error_exception(sqlite3_errmsg(sqlite3_db_handle(stmt)));
        }
    }

end:
    std::unique_ptr<dab::db_result_row> ret = std::make_unique<sqlite_result_row>();
    co_yield ret;
}

dab::sqlite_dml_command::sqlite_dml_command(void* db_raw, std::string_view sql)
     : _db(db_raw) {
    assert(db_raw);

    auto* db = static_cast<sqlite3*>(db_raw);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.data(), static_cast<int>(sql.size()), &stmt, nullptr);
    _dml = stmt;

    assert(_dml);
}

dab::sqlite_dml_command::~sqlite_dml_command() noexcept {
    assert(_dml);
    auto* stmt = static_cast<sqlite3_stmt*>(_dml);
    sqlite3_finalize(stmt);
}

// DCL ////////////////////////////////////////////////////////////////////////////////////////////

async::task<void>
dab::sqlite_dcl_command::execute() {
    assert(_dcl);

    auto* stmt = static_cast<sqlite3_stmt*>(_dcl);
    auto res = sqlite3_step(stmt);
    switch (res) {
    case SQLITE_DONE:
        co_return;
    case SQLITE_BUSY:
        throw db_locking_exception();
    default:
        throw db_error_exception(sqlite3_errmsg(sqlite3_db_handle(stmt)));
    }
}

dab::sqlite_dcl_command::~sqlite_dcl_command() noexcept {
    assert(_dcl);
    auto* stmt = static_cast<sqlite3_stmt*>(_dcl);
    sqlite3_finalize(stmt);
}

dab::sqlite_dcl_command::sqlite_dcl_command(void* db_raw, std::string_view sql) {
    assert(db_raw);

    auto* db = static_cast<sqlite3*>(db_raw);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.data(), static_cast<int>(sql.size()), &stmt, nullptr);
    _dcl = stmt;

    assert(_dcl);
}

// DDL ////////////////////////////////////////////////////////////////////////////////////////////

async::task<void>
dab::sqlite_ddl_command::execute() {
    assert(_ddl);

    auto* stmt = static_cast<sqlite3_stmt*>(_ddl);
    auto res = sqlite3_step(stmt);
    switch (res) {
    case SQLITE_DONE:
        co_return;
    case SQLITE_BUSY:
        throw db_locking_exception();
    default:
        throw db_error_exception(sqlite3_errmsg(sqlite3_db_handle(stmt)));
    }
}

dab::sqlite_ddl_command::~sqlite_ddl_command() noexcept {
    assert(_ddl);
    auto* stmt = static_cast<sqlite3_stmt*>(_ddl);
    sqlite3_finalize(stmt);
}

dab::sqlite_ddl_command::sqlite_ddl_command(void* db_raw, std::string_view sql) {
    assert(db_raw);

    auto* db = static_cast<sqlite3*>(db_raw);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.data(), static_cast<int>(sql.size()), &stmt, nullptr);
    _ddl = stmt;

    assert(_ddl);
}
