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
 * src/core/public/dab/db_result_row --
 *   
 */
#ifndef DAB_DB_RESULT_SET_HXX
#define DAB_DB_RESULT_SET_HXX

#include <memory>

#include <dab/typing/db_value.hxx>
#include <dab/typing/type_id.hpp>

namespace dab {
    struct db_result_invalid_arity final : std::runtime_error {
        db_result_invalid_arity(std::size_t req,
                                std::size_t real);
    };

    struct db_result_row {
        db_result_row(const db_result_row& cp) = delete;
        db_result_row(db_result_row&& mv) noexcept = delete;
        db_result_row&
        operator=(const db_result_row& cp) = delete;
        db_result_row&
        operator=(db_result_row&& mv) noexcept = delete;

        virtual ~db_result_row() noexcept = default;

        [[nodiscard]] std::unique_ptr<db_value>
        get_column(int idx) {
            return do_get_column(idx);
        }

        [[nodiscard]] std::size_t
        get_column_count() const noexcept {
            return _column_count;
        }

        template<class... Columns>
        [[nodiscard]] std::tuple<Columns...>
        get_as_tuple() {
            if (_column_count != sizeof...(Columns))
                throw db_result_invalid_arity(sizeof...(Columns), _column_count);

            return [this]<std::size_t... Is>(std::index_sequence<Is...>) {
                return std::make_tuple(do_get_column(Is)->template get_as<Columns>()...);
            }(std::make_index_sequence<sizeof...(Columns)>());
        }

        [[nodiscard]] virtual bool
        has_row() = 0;

    protected:
        virtual std::unique_ptr<db_value>
        do_get_column(int idx) = 0;

        explicit db_result_row(std::size_t column_count)
             : _column_count(column_count) { }

    private:
        std::size_t _column_count;
    };
}

#endif
