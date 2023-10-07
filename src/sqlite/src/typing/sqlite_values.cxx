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
 * src/sqlite/src/typing/sqlite_values --
 *   
 */

#include <algorithm>
#include <iterator>
#include <ranges>

#include <dab/sqlite/typing/sqlite_values.hxx>

#include <sqlite3.h>

void
dab::sqlite_int_value::do_get_into(int* result) {
    *result = sqlite3_value_int(static_cast<sqlite3_value*>(_value));
}

void
dab::sqlite_int_value::do_get_into(unsigned* result) {
    *result = static_cast<unsigned>(sqlite3_value_int(static_cast<sqlite3_value*>(_value)));
}

void
dab::sqlite_int_value::do_get_into(long* result) {
    *result = sqlite3_value_int(static_cast<sqlite3_value*>(_value));
}

void
dab::sqlite_int_value::do_get_into(unsigned long* result) {
    *result = static_cast<unsigned long>(sqlite3_value_int(static_cast<sqlite3_value*>(_value)));
}

void
dab::sqlite_int_value::do_get_into(long long* result) {
    *result = sqlite3_value_int(static_cast<sqlite3_value*>(_value));
}

void
dab::sqlite_int_value::do_get_into(unsigned long long* result) {
    *result = static_cast<unsigned long long>(sqlite3_value_int(static_cast<sqlite3_value*>(_value)));
}

dab::sqlite_int_value::sqlite_int_value(void* value)
     : _value(sqlite3_value_dup(static_cast<sqlite3_value*>(value))) { }

dab::sqlite_int_value::~sqlite_int_value() noexcept {
    sqlite3_value_free(static_cast<sqlite3_value*>(_value));
}

void
dab::sqlite_int64_value::do_get_into(long long* result) {
    *result = sqlite3_value_int(static_cast<sqlite3_value*>(_value));
}

void
dab::sqlite_int64_value::do_get_into(unsigned long long* result) {
    *result = static_cast<unsigned long long>(sqlite3_value_int(static_cast<sqlite3_value*>(_value)));
}

dab::sqlite_int64_value::sqlite_int64_value(void* value)
     : _value(sqlite3_value_dup(static_cast<sqlite3_value*>(value))) { }

dab::sqlite_int64_value::~sqlite_int64_value() noexcept {
    sqlite3_value_free(static_cast<sqlite3_value*>(_value));
}

void
dab::sqlite_text_value::do_get_into(std::string* result) {
    auto typed_value = static_cast<sqlite3_value*>(_value);
    auto buf_sz = sqlite3_value_bytes(typed_value);
    auto* buf = sqlite3_value_text(typed_value);
    result->clear();
    result->reserve(static_cast<std::size_t>(buf_sz));
    std::ranges::transform(buf, buf + buf_sz, std::back_inserter(*result), [](unsigned char x) {
        return static_cast<char>(x);
    });
}

dab::sqlite_text_value::sqlite_text_value(void* value)
     : _value(sqlite3_value_dup(static_cast<sqlite3_value*>(value))) { }

dab::sqlite_text_value::~sqlite_text_value() noexcept {
    sqlite3_value_free(static_cast<sqlite3_value*>(_value));
}

void
dab::sqlite_double_value::do_get_into(double* result) {
    *result = sqlite3_value_double(static_cast<sqlite3_value*>(_value));
}

void
dab::sqlite_double_value::do_get_into(long double* result) {
    *result = sqlite3_value_double(static_cast<sqlite3_value*>(_value));
}

dab::sqlite_double_value::sqlite_double_value(void* value)
     : _value(sqlite3_value_dup(static_cast<sqlite3_value*>(value))) { }

dab::sqlite_double_value::~sqlite_double_value() noexcept {
    sqlite3_value_free(static_cast<sqlite3_value*>(_value));
}
