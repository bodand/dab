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
# src/sqlite/CMakeLists.txt --
#   The SQLite backend for DAB.

find_package(unofficial-sqlite3 CONFIG)

if (NOT TARGET unofficial::sqlite3::sqlite3)
    return()
endif ()

find_package(Boost REQUIRED)

project(Dab.Sqlite CXX)

add_library(Dab.Sqlite STATIC
            include/dab/sqlite/connection_string.hxx
            public/dab/sqlite/typing/sqlite_values.hxx
            public/dab/sqlite/sqlite_command.hxx
            public/dab/sqlite/sqlite_connection.hxx
            public/dab/sqlite/sqlite_connection_string_builder.hxx
            public/dab/sqlite/sqlite_result_row.hxx
            public/dab/sqlite/sqlite_factory.hxx
            public/dab/sqlite/sqlite_transaction.hxx
            src/private/connection_string.cxx
            src/typing/sqlite_values.cxx
            src/sqlite_command.cxx
            src/sqlite_connection.cxx
            src/sqlite_connection_string_builder.cxx
            src/sqlite_result_row.cxx
            src/sqlite_factory.cxx
            src/sqlite_transaction.cxx)

target_include_directories(Dab.Sqlite
                           PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
                           PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(Dab.Sqlite
                      PUBLIC Dab.Core
                      PRIVATE
                      unofficial::sqlite3::sqlite3
                      Boost::boost
                      internal-warnings
                      internal-lto
                      internal-coverage)
target_compile_features(Dab.Sqlite PUBLIC cxx_std_20)

list(APPEND PROJECT_TARGETS Dab.Sqlite)

add_subdirectory(test)
