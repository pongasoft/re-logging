# Copyright (c) 2022 pongasoft
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.
#
# @author Yan Pujante

# The purpose of this CMake file is to compile/test re-logging only
# When using re-logging, re-logging.cmake needs to be included instead

cmake_minimum_required(VERSION 3.17)

project(re-logging LANGUAGES CXX)

# Using C++17
set(CMAKE_CXX_STANDARD 17)

# Determines whether we are working on re-logging or using it
if(${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
  set(re-logging_DEV_BUILD TRUE)
else()
  set(re-logging_DEV_BUILD FALSE)
endif()

if(NOT re-logging_DEV_BUILD)
  message(FATAL_ERROR "Use re-logging.cmake in your project")
endif()

enable_testing()

# Using RE SDK version 4.3.0
set(RE_SDK_VERSION 4.3.0)

# Location of RE SDK: can be set when invoking cmake => cmake -D "RE_SDK_ROOT:PATH=/path/to/re_sdk"
if(APPLE)
  set(RE_SDK_ROOT "/Users/Shared/ReasonStudios/JukeboxSDK_${RE_SDK_VERSION}/SDK" CACHE PATH "Location of RE SDK")
else()
  set(RE_SDK_ROOT "C:/Users/Public/Documents/ReasonStudios/JukeboxSDK_${RE_SDK_VERSION}/SDK" CACHE PATH "Location of RE SDK")
endif()

# include google test
include(cmake/fetch-GoogleTest.cmake)
include(GoogleTest)

include(re-logging.cmake)

add_library(re-logging STATIC "${re-logging_SOURCES}")
target_include_directories(re-logging PUBLIC "${re-logging_INCLUDE_DIRS}" "${RE_SDK_ROOT}/API")
target_compile_definitions(re-logging PUBLIC LOCAL_NATIVE_BUILD=1 DEBUG=1)

set(target_test "re-logging_test")
set(re-logging_CPP_TST_DIR "${CMAKE_CURRENT_LIST_DIR}/test/cpp")
set(TEST_CASE_SOURCES
    "${re-logging_CPP_TST_DIR}/re-logging/TestLogging.cpp"
    )

add_executable("${target_test}" "${TEST_CASE_SOURCES}")
target_link_libraries("${target_test}" gtest_main re-logging)

gtest_discover_tests("${target_test}")
