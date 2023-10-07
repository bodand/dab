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
 * src/sqlite/src/sqlite_result_row --
 *   
 */

#include "dab/sqlite/sqlite_result_row.hxx"

#include <cassert>
#include <memory>

#include <dab/sqlite/typing/sqlite_values.hxx>

#include <sqlite3.h>

dab::sqlite_result_row::sqlite_result_row(void* stmt)
     : db_result_row(static_cast<size_t>(sqlite3_column_count(static_cast<sqlite3_stmt*>(stmt)))),
       _stmt(stmt) { }

bool
dab::sqlite_result_row::has_row() {
    return _stmt != nullptr;
}

dab::sqlite_result_row::sqlite_result_row()
     : db_result_row(0), _stmt(nullptr) { }

std::unique_ptr<dab::db_value>
dab::sqlite_result_row::do_get_column(int idx) {
    assert(_stmt != nullptr);

    auto* stmt = static_cast<sqlite3_stmt*>(_stmt);
    auto* val = sqlite3_column_value(stmt, idx);
    switch (sqlite3_column_type(stmt, idx)) {
    case SQLITE_INTEGER:
        return std::make_unique<sqlite_int_value>(val);
    case SQLITE_FLOAT:
        return std::make_unique<sqlite_double_value>(val);
    case SQLITE_BLOB:
        return nullptr; // TODO
    case SQLITE_TEXT:
        return std::make_unique<sqlite_text_value>(val);
    case SQLITE_NULL:
        return nullptr;
    default:
#ifdef __GNUG__
        __builtin_unreachable();
#elif defined(_MSC_VER)
        __assume(0);
#endif
    }
}
