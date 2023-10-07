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
 * src/sqlite/src/sqlite_connection_string_builder --
 *   
 */

#include <optional>
#include <sstream>
#include <string>
#include <unordered_set>

#include <dab/sqlite/connection_string.hxx>
#include <dab/sqlite/sqlite_connection_string_builder.hxx>

namespace {
    [[nodiscard]] std::optional<std::string_view>
    try_get(const std::unordered_map<std::string, std::string>& values, const std::string& key) {
        if (auto it = values.find(key);
            it != values.end()) {
            return std::string_view{it->second};
        }
        return {};
    }

    void
    append_option(std::ostream& os, bool& have_optional, std::string_view name, std::optional<std::string_view> value) {
        if (value) {
            if (have_optional) {
                os << "&";
            }
            else {
                os << "?";
            }
            have_optional = true;
            os << name << "=" << *value;
        }
    }
}

std::string
dab::sqlite_connection_string_builder::connection_string() {
    std::unordered_set<std::string_view> uri_components{"vfs", "mode", "cache", "nolock", "immutable"};

    std::ostringstream ss;
    ss << "file:" << _values["path"];
    _values.erase("path");
    bool have_optional = false;
    for (const auto& comp : uri_components) {
        auto str_key = std::string{comp.data(), comp.size()};
        append_option(ss, have_optional, comp, try_get(_values, str_key));
        _values.erase(str_key);
    }
    _values.try_emplace("Uri", ss.str());

    return dab::sqlite::create_from_value_set(_values);
}

dab::sqlite_connection_string_builder&
dab::sqlite_connection_string_builder::add(std::string_view key, std::string_view value) {
    _values.insert_or_assign(std::string{key.data(), key.size()},
                             std::string{value.data(), value.size()});
    return *this;
}

std::string_view
dab::sqlite_connection_string_builder::get(std::string_view key) {
    if (auto it = _values.find({key.data(), key.size()});
        it != _values.end()) {
        return it->second;
    }
    throw std::out_of_range("invalid connection string component queried");
}

