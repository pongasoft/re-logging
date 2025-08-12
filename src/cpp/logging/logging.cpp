/*
 * Copyright (c) 2025 pongasoft
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

#include "logging.h"
#include <exception>

namespace re::logging {

static impl::Verbosity kVerbosity = impl::Verbosity::INFO;
static std::optional<std::string> kPrefix{};
static bool kFatalThrowsException{};

//------------------------------------------------------------------------
// init_for_test
//------------------------------------------------------------------------
void init_for_test(char const *iPrefix)
{
  if(iPrefix)
    kPrefix = iPrefix;
  kFatalThrowsException = true;
}

//------------------------------------------------------------------------
// init_for_re
//------------------------------------------------------------------------
void init_for_re(char const *iREName)
{
  if(iREName)
    kPrefix = iREName;
}

namespace impl {

//------------------------------------------------------------------------
// setVerbosity
//------------------------------------------------------------------------
void setVerbosity(Verbosity iVerbosity)
{
  kVerbosity = iVerbosity;
}

//------------------------------------------------------------------------
// fatal
//------------------------------------------------------------------------
void fatal()
{
  if(kFatalThrowsException)
    throw std::runtime_error("Fatal");
  else
    ::abort();
}

//------------------------------------------------------------------------
// verbosityToString
//------------------------------------------------------------------------
constexpr char const *verbosityToString(Verbosity iVerbosity)
{
  switch(iVerbosity)
  {
    case Verbosity::FATAL: return "FATL";
    case Verbosity::ERROR: return "ERR ";
    case Verbosity::WARNING: return "WARN";
    case Verbosity::INFO: return "INFO";
    case Verbosity::VERBOSE: return "VRBO";
    default: return "INVALID";
  }
}

//------------------------------------------------------------------------
// log
//------------------------------------------------------------------------
void log(Verbosity iVerbosity, char const *iFile, int iLine, std::string_view iMessage)
{
  if(iVerbosity <= kVerbosity)
  {
    if(kPrefix)
      re::logging::impl::printf("%s | %s | %s:%d | %s\n", verbosityToString(iVerbosity), kPrefix.value().c_str(), iFile, iLine, iMessage);
    else
      re::logging::impl::printf("%s | %s:%d | %s\n", verbosityToString(iVerbosity), iFile, iLine, iMessage);
  }

  if(iVerbosity == Verbosity::FATAL)
    fatal();
}

} // namespace impl

} // namespace re::logging