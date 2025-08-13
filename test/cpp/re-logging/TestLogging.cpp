/*
 * Copyright (c) 2022 pongasoft
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 *
 * @author Yan Pujante
 */

#include <gtest/gtest.h>
#include <logging.h>

TEST(Logging, init_for_test)
{
  // init_for_test changes abort into exception that can be caught
  RE_LOGGING_INIT_FOR_TEST("[Logging.init_for_test]");
  ASSERT_THROW(ABORT_F("should throw"), std::runtime_error);
  DLOG_F(INFO, "output 1");
  DLOG_F(INFO, "output 2");
  ASSERT_THROW(DLOG_F(FATAL, "output fatal"), std::runtime_error);
}

TEST(Logging, init_for_re)
{
  // init_for_re displays the name of the re
  RE_LOGGING_INIT_FOR_RE("[My RE]");
  printf("INFO...\n");
  DLOG_F(VERBOSE, "output verbose");
  DLOG_F(INFO, "output info");
  DLOG_F(WARNING, "output warning");
  DLOG_F(ERROR, "output error");
  RE_LOGGING_SET_VERBOSITY(WARNING);
  printf("WARNING...\n");
  DLOG_F(VERBOSE, "output verbose");
  DLOG_F(INFO, "output info");
  DLOG_F(WARNING, "output warning");
  DLOG_F(ERROR, "output error");
  RE_LOGGING_SET_VERBOSITY(ERROR);
  RE_LOGGING_KEEP_FILE_PATH();
  printf("ERROR...\n");
  DLOG_F(VERBOSE, "output verbose");
  DLOG_F(INFO, "output info");
  DLOG_F(WARNING, "output warning");
  DLOG_F(ERROR, "output error");
  RE_LOGGING_SET_VERBOSITY(VERBOSE);
  RE_LOGGING_STRIP_FILE_PATH();
  printf("VERBOSE...\n");
  DLOG_F(VERBOSE, "output verbose");
  DLOG_F(INFO, "output info");
  DLOG_F(WARNING, "output warning");
  DLOG_F(ERROR, "output error");
}