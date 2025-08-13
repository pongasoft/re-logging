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

cmake_minimum_required(VERSION 3.17)

set(re-logging_MAJOR_VERSION 2)
set(re-logging_MINOR_VERSION 0)
set(re-logging_PATCH_VERSION 0)
set(re-logging_VERSION "${re-logging_MAJOR_VERSION}.${re-logging_MINOR_VERSION}.${re-logging_PATCH_VERSION}")

set(re-logging_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(re-logging_SOURCE_DIR "${re-logging_ROOT_DIR}/src/cpp/logging")
set(re-logging_INCLUDE_DIRS "${re-logging_SOURCE_DIR}")

# Defines the files to include for logging (they will be included in the Recon build ONLY)
set(re-logging_SOURCES
    ${re-logging_SOURCE_DIR}/logging.h
    ${re-logging_SOURCE_DIR}/logging.cpp
    )
