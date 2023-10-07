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
# Originally created: 2023-09-27.
#
# src/core/CMakeLists.txt --
#   The CMakeLists file for managing the core library of dab.

project(Dab.Core CXX)

find_package(tl-generator CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_library(Dab.Core STATIC
            public/dab/fwd/db_transaction.hxx
            public/dab/fwd/db_connection.hxx
            public/dab/fwd/db_connection_string_builder.hxx
            public/dab/fwd/db_command.hxx
            public/dab/typing/db_value.hxx
            public/dab/typing/type_id.hpp
            public/dab/db_command.hxx
            public/dab/db_connection_string_builder.hxx
            public/dab/db_connection.hxx
            public/dab/isolation_level.hxx
            public/dab/db_transaction.hxx
            public/dab/db_factory.hxx
            public/dab/db_result_row.hxx
            src/typing/db_value.cxx
            src/typing/type_id.cpp
            src/db_connection_string_builder.cxx
            src/db_connection.cxx
            src/db_command.cxx
            src/db_factory.cxx
            src/db_result_row.cxx
            src/db_transaction.cxx)

target_include_directories(Dab.Core
                           PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
                           PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(Dab.Core
                      PUBLIC tl::generator fmt::fmt
                      PRIVATE internal-coverage internal-warnings internal-lto)
target_compile_features(Dab.Core PUBLIC cxx_std_20)

list(APPEND PROJECT_TARGETS Dab.Core)

add_subdirectory(test)
