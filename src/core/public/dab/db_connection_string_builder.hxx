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
 * Originally created: 2023-09-27.
 *
 * src/core/public/dab/db_connection_string_builder --
 *   A db_connection_string_builder helps creating the database connection's
 *   connection string.
 */
#ifndef DAB_DB_CONNECTION_STRING_BUILDER_HXX
#define DAB_DB_CONNECTION_STRING_BUILDER_HXX

#include <string>

namespace dab {
    struct db_connection_string_builder;

    struct db_connection_string_builder_helper final {
        db_connection_string_builder_helper(db_connection_string_builder* parent,
                                            std::string_view key)
             : _key(key),
               _parent(parent) { }

        db_connection_string_builder_helper(const db_connection_string_builder_helper& cp) = delete;
        db_connection_string_builder_helper(db_connection_string_builder_helper&& mv) noexcept = delete;

        db_connection_string_builder_helper&
        operator=(const db_connection_string_builder_helper& cp) = delete;
        db_connection_string_builder_helper&
        operator=(db_connection_string_builder_helper&& mv) noexcept = delete;

        ~db_connection_string_builder_helper() = default;

        std::string_view // NOLINT(*-c-copy-assignment-signature, *-unconventional-assign-operator)
        operator=(std::string_view value);

        explicit(false) operator std::string_view() const;
        explicit(false) operator std::string_view();

    private:
        std::string_view _key;
        db_connection_string_builder* _parent;
    };

    struct db_connection_string_builder {
        db_connection_string_builder(const db_connection_string_builder& cp) = delete;
        db_connection_string_builder(db_connection_string_builder&& mv) = delete;

        db_connection_string_builder&
        operator=(const db_connection_string_builder& cp) = delete;
        db_connection_string_builder&
        operator=(db_connection_string_builder&& mv) = delete;

        virtual ~db_connection_string_builder() = default;

        virtual std::string
        connection_string() = 0;

        virtual db_connection_string_builder&
        add(std::string_view key, std::string_view value) = 0;

        virtual std::string_view
        get(std::string_view key) = 0;

        [[nodiscard("Returned helper object can be used to set the value")]] //
        db_connection_string_builder_helper
        operator[](std::string_view key) & {
            return {this, key};
        }

    protected:
        db_connection_string_builder() noexcept = default;
    };
}

#endif
