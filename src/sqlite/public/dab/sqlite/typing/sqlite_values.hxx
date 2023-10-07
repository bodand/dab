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
 * src/sqlite/public/dab/sqlite/typing/sqlite_values --
 *   
 */
#ifndef DAB_SQLITE_VALUES_HXX
#define DAB_SQLITE_VALUES_HXX

#include <string>
#include <type_traits>

#include <dab/typing/db_value.hxx>

namespace dab {
    struct sqlite_int_value final : db_typed_value<int,
                                                   unsigned,
                                                   long,
                                                   unsigned long,
                                                   long long,
                                                   unsigned long long> {
        explicit sqlite_int_value(void* value);

        void
        do_get_into(int* result) override;
        void
        do_get_into(unsigned* result) override;
        void
        do_get_into(long* result) override;
        void
        do_get_into(unsigned long* result) override;
        void
        do_get_into(long long* result) override;
        void
        do_get_into(unsigned long long* result) override;

        ~sqlite_int_value() noexcept override;

    private:
        void* _value;
    };

    struct sqlite_int64_value final : db_typed_value<long long,
                                                     unsigned long long> {
        explicit sqlite_int64_value(void* value);

        void
        do_get_into(long long* result) override;
        void
        do_get_into(unsigned long long* result) override;

        ~sqlite_int64_value() noexcept override;

    private:
        void* _value;
    };

    struct sqlite_text_value final : db_typed_value<std::string> {
        explicit sqlite_text_value(void* value);

        void
        do_get_into(std::string* result) override;

        ~sqlite_text_value() noexcept override;

    private:
        void* _value;
    };

    struct sqlite_double_value final : db_typed_value<double, long double> {
        explicit sqlite_double_value(void* value);

        void
        do_get_into(double* result) override;

        void
        do_get_into(long double* result) override;

        ~sqlite_double_value() noexcept override;

    private:
        void* _value;
    };

    // TODO: blob value
}

#endif
