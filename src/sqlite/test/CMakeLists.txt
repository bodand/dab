# dab project
#
# Copyright (c) 2023 András Bodor <bodand@pm.me>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# - Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# - Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# - Neither the name of the copyright holder nor the names of its contributors
#   may be used to endorse or promote products derived from this software
#   without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Originally created: 2023-09-29.
#
# src/sqlite/test/CMakeLists.txt --
#   CMake module handling the test-code for the SQLite DAB backend.

add_covered_test(NAME "Dab.Sqlite.Tests" CATCH_OWNMAIN
                 SOURCES
                 main.cxx
                 connection_string_builder.test.cxx
                 connection.test.cxx
                 database_creation.test.cxx
                 dql.test.cxx
                 ddl.test.cxx
                 LIBRARIES magic_enum::magic_enum Dab.Core Dab.Sqlite)

add_custom_command(TARGET "Dab.Sqlite.Tests" POST_BUILD
                   COMMAND "${CMAKE_COMMAND}" -E "copy_if_different"
                   "${CMAKE_CURRENT_SOURCE_DIR}/test.db"
                   "${CMAKE_CURRENT_BINARY_DIR}")
