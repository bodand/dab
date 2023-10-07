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
 * src/sqlite/public/dab/sqlite/sqlite_connection_string_builder --
 *   
 */
#ifndef DAB_SQLITE_CONNECTION_STRING_BUILDER_HXX
#define DAB_SQLITE_CONNECTION_STRING_BUILDER_HXX

#include <unordered_map>

#include <dab/db_connection_string_builder.hxx>

namespace dab {
    enum class sqlite_connection_mode {
        READ,
        READ_WRITE,
        READ_WRITE_CREATE,
        MEMORY
    };

    enum class sqlite_journal_mode {
        DELETE,
        TRUNCATE,
        PERSIST,
        MEMORY,
        WAL,
        OFF
    };

    enum class sqlite_cache_behavior {
        SHARED,
        PRIVATE
    };

    struct sqlite_connection_string_builder final : db_connection_string_builder {
        [[nodiscard]] std::string
        connection_string() override;

        sqlite_connection_string_builder&
        add(std::string_view key, std::string_view value) override;

        [[nodiscard]] std::string_view
        get(std::string_view key) override;

        sqlite_connection_string_builder&
        set_path(std::string_view value) {
            constexpr const static auto kv_key = "path";
            return add(kv_key, value);
        }

        sqlite_connection_string_builder&
        set_vfs(std::string_view value) {
            constexpr const static auto kv_key = "vfs";
            return add(kv_key, value);
        }

        sqlite_connection_string_builder&
        set_cache_behavior(sqlite_cache_behavior behavior) {
            constexpr const static auto kv_key = "cache";
            switch (behavior) {
                using enum sqlite_cache_behavior;
            case SHARED: return add(kv_key, "shared");
            case PRIVATE: return add(kv_key, "private");
            }
        }

        sqlite_connection_string_builder&
        set_mode(sqlite_connection_mode mode) {
            constexpr const static auto kv_key = "mode";
            switch (mode) {
                using enum dab::sqlite_connection_mode;
            case READ: return add(kv_key, "ro");
            case READ_WRITE: return add(kv_key, "rw");
            case READ_WRITE_CREATE: return add(kv_key, "rwc");
            case MEMORY: return add(kv_key, "memory");
            }
        }

        sqlite_connection_string_builder&
        set_powersafe_overwrite(bool psow = false) {
            constexpr const static auto kv_key = "psow";
            if (psow) return add(kv_key, "1");
            return add(kv_key, "0");
        }

        sqlite_connection_string_builder&
        set_locking(bool lock = true) {
            constexpr const static auto kv_key = "nolock";
            if (!lock) return add(kv_key, "1");
            return add(kv_key, "0");
        }

        sqlite_connection_string_builder&
        set_immutable(bool imm = true) {
            constexpr const static auto kv_key = "immutable";
            if (imm) return add(kv_key, "1");
            return add(kv_key, "0");
        }

        sqlite_connection_string_builder&
        set_journaling(sqlite_journal_mode journal_mode) {
            constexpr const static auto kv_key = "Journal Mode";
            switch (journal_mode) {
                using enum dab::sqlite_journal_mode;
            case DELETE: return add(kv_key, "DELETE");
            case TRUNCATE: return add(kv_key, "TRUNCATE");
            case PERSIST: return add(kv_key, "PERSIST");
            case MEMORY: return add(kv_key, "MEMORY");
            case WAL: return add(kv_key, "WAL");
            case OFF: return add(kv_key, "OFF");
            }
        }

    private:
        std::unordered_map<std::string, std::string> _values{};
    };
}

#endif
