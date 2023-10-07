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
 * src/core/public/dab/db_factory --
 *   The polymorphic factory class for generating db_* types.
 *   Each backend provides their own specific db_factory that generates the
 *   correctly typed elements for their own hierarchy.
 */

#ifndef DAB_DB_FACTORY_HXX
#define DAB_DB_FACTORY_HXX

#include <cassert>
#include <concepts>
#include <memory>

#include <dab/db_connection_string_builder.hxx>
#include <dab/fwd/db_connection.hxx>
#include <dab/fwd/db_transaction.hxx>

#include <async/task.h>

namespace dab {
    struct db_factory {
        db_factory(const db_factory&) = delete;

        db_factory(db_factory&&) noexcept = delete;
        db_factory&
        operator=(const db_factory&) = delete;

        db_factory&
        operator=(db_factory&&) noexcept = delete;
        virtual ~db_factory() = default;

        template<class Fn>
        void
        configure_connection(Fn&& fn)
            requires(std::invocable<Fn, db_connection_string_builder&>)
        {
            std::invoke(std::forward<Fn>(fn), *_connection_string_builder);
        }

        virtual async::task<std::unique_ptr<db_connection>>
        create_connection() = 0;

    protected:
        explicit db_factory(std::unique_ptr<db_connection_string_builder>&& connection_string_builder)
             : _connection_string_builder(std::move(connection_string_builder)) {
            assert(_connection_string_builder);
        }

    private:
        std::unique_ptr<db_connection_string_builder> _connection_string_builder;
    };
}

#endif
