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
  ASSERT_THROW(ABORT_F(), std::runtime_error);
  ASSERT_THROW(ABORT_F("should throw"), std::runtime_error);
  DLOG_F(INFO, "output 1");
  DLOG_F(INFO, "output 2");
  ASSERT_THROW(DLOG_F(FATAL, "output fatal"), std::runtime_error);
  ASSERT_THROW(DCHECK_F(2 != 2), std::runtime_error);
  ASSERT_THROW(DCHECK_F(2 != 2, "checking 2!=2"), std::runtime_error);
  ASSERT_THROW(DCHECK_F(2 != 2, "checking %d!=%d", 2, 2), std::runtime_error);
  re::logging::impl::reset();
}

TEST(Logging, init_for_re)
{
  // init_for_re displays the name of the re
  RE_LOGGING_INIT_FOR_RE("[My RE]");
  printf("INFO...\n");
  DLOG_F(VERBOSE, "output %s", "verbose");
  DLOG_F(INFO, "output info");
  DLOG_F(WARNING, "output warning");
  DLOG_F(ERROR, "output error");
  RE_LOGGING_SET_VERBOSITY(WARNING);
  printf("WARNING...\n");
  DLOG_F(VERBOSE, "output %s", "verbose");
  DLOG_F(INFO, "output info");
  DLOG_F(WARNING, "output warning");
  DLOG_F(ERROR, "output error");
  RE_LOGGING_SET_VERBOSITY(ERROR);
  printf("ERROR...\n");
  DLOG_F(VERBOSE, "output %s", "verbose");
  DLOG_F(INFO, "output info");
  DLOG_F(WARNING, "output warning");
  DLOG_F(ERROR, "output error");
  RE_LOGGING_SET_VERBOSITY(VERBOSE);
  printf("VERBOSE...\n");
  DLOG_F(VERBOSE, "output %s", "verbose");
  DLOG_F(INFO, "output info");
  DLOG_F(WARNING, "output warning");
  DLOG_F(ERROR, "output error");

  DCHECK_F(2 != 3);
  DCHECK_F(2 != 3, "checking 2!=3");
  DCHECK_F(2 != 3, "checking %d!=%d", 2, 3);

  printf("RE_LOGGING_INIT_FOR_TEST override\n");
  RE_LOGGING_INIT_FOR_TEST("[Override]");
  DLOG_F(INFO, "output info");

  printf("No prefix\n");
  RE_LOGGING_INIT_FOR_TEST(nullptr);
  RE_LOGGING_INIT_FOR_RE(nullptr);
  DLOG_F(INFO, "output info");
  re::logging::impl::reset();
}

TEST(Logging, checks)
{
  // init_for_re displays the name of the re
  RE_LOGGING_INIT_FOR_TEST("[Logging.checks]");
  DCHECK_F(2 != 3);
  DCHECK_F(2 != 3, "checking 2!=3");
  DCHECK_F(2 != 3, "checking %d!=%d", 2, 3);
  ASSERT_THROW(DCHECK_F(2 != 2), std::runtime_error);

  int a = 0;
  int b = 1;
  DCHECK_NOTNULL_F(&a);
  DCHECK_NOTNULL_F(&a, "valid pointer");
  DCHECK_NOTNULL_F(&a, "valid %s", "pointer");
  ASSERT_THROW(DCHECK_NOTNULL_F(nullptr), std::runtime_error);

  DCHECK_EQ_F(a, b - 1);
  ASSERT_THROW(DCHECK_EQ_F(a + 2, b - 3), std::runtime_error);

  DCHECK_NE_F(a, b);
  ASSERT_THROW(DCHECK_NE_F(a + 1, b), std::runtime_error);

  DCHECK_LT_F(a, b);
  ASSERT_THROW(DCHECK_LT_F(b, a), std::runtime_error);

  DCHECK_LE_F(a, b);
  DCHECK_LE_F(a, a);
  ASSERT_THROW(DCHECK_LE_F(b, a), std::runtime_error);

  DCHECK_GT_F(b, a);
  ASSERT_THROW(DCHECK_GT_F(a, b), std::runtime_error);

  DCHECK_GE_F(b, a);
  DCHECK_GE_F(a, a);
  ASSERT_THROW(DCHECK_GE_F(a, b), std::runtime_error);
  re::logging::impl::reset();
}