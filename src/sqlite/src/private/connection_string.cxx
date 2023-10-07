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
 * Originally created: 2023-09-30.
 *
 * src/sqlite/src/private/connection_string --
 *   
 */

#include <sstream>
#include <string>
#include <unordered_map>

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/karma_string.hpp>
#include <dab/sqlite/connection_string.hxx>

namespace x3 = boost::spirit::x3;
namespace karma = boost::spirit::karma;
namespace fsn = boost::fusion;

std::string
dab::sqlite::create_from_value_set(const std::unordered_map<std::string, std::string>& values) {
    using karma::ascii::space;
    using karma::ascii::string;

    std::string str;
    str.reserve(128);
    karma::generate(std::back_inserter(str), //
                    (string << "=" << string) % ';',
                    values);
    return str;
}

std::unordered_map<std::string, std::string>
dab::sqlite::parse_to_value_set(std::string_view connection_string) {
    using x3::ascii::char_;
    std::unordered_map<std::string, std::string> ret;

    auto adder = [&ret](auto& ctx) {
        auto&& payload = x3::_attr(ctx);
        ret.insert_or_assign(fsn::at_c<0>(payload), fsn::at_c<1>(payload));
    };

    x3::parse(connection_string.begin(), connection_string.end(), //
              (+(char_ - "=") >> "=" >> +(char_ - ";"))[adder] % ";");

    return ret;
}
