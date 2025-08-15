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
#include <chrono>

namespace re::logging::impl {

static Verbosity kVerbosity = Verbosity::INFO;
static std::optional<std::string> kPrefix{};
static bool kFatalThrowsException{};
static bool kStripFilePath{true};
static auto const kStartTime = std::chrono::steady_clock::now();

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
  else
    kPrefix = std::nullopt;
  kFatalThrowsException = false;
}

//------------------------------------------------------------------------
// setVerbosity
//------------------------------------------------------------------------
void setVerbosity(Verbosity iVerbosity)
{
  kVerbosity = iVerbosity;
}

//------------------------------------------------------------------------
// setStripFilePath
//------------------------------------------------------------------------
void setStripFilePath(bool iStripFilePath)
{
  kStripFilePath = iStripFilePath;
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
// filename
//------------------------------------------------------------------------
const char *filename(const char *path)
{
  for(auto ptr = path; *ptr; ++ptr)
  {
    if(*ptr == '/' || *ptr == '\\')
    {
      path = ptr + 1;
    }
  }
  return path;
}

//------------------------------------------------------------------------
// uptimeInSeconds
//------------------------------------------------------------------------
static double uptimeInSeconds()
{
  auto uptime =
    std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - kStartTime).count();
  return static_cast<double>(uptime) / 1000.0;
}

//------------------------------------------------------------------------
// currentTimeToString
//------------------------------------------------------------------------
static std::string currentTimeToString()
{
  auto now = std::chrono::system_clock::now();
  auto ms_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
  auto sec_since_epoch = time_t(ms_since_epoch / 1000);
  tm time_info{};
  localtime_r(&sec_since_epoch, &time_info);
  return impl::sprintf("%04d-%02d-%02d %02d:%02d:%02d.%03lld",
                       1900 + time_info.tm_year, 1 + time_info.tm_mon, time_info.tm_mday,
                       time_info.tm_hour, time_info.tm_min, time_info.tm_sec, ms_since_epoch % 1000);
}

//------------------------------------------------------------------------
// log
//------------------------------------------------------------------------
void log(Verbosity iVerbosity, char const *iFile, int iLine, std::string_view iMessage)
{
  if(iVerbosity <= kVerbosity)
  {
    if(kStripFilePath)
      iFile = filename(iFile);

    if(kPrefix)
      re::logging::impl::printf("%s (%8.3fs) | %s | %s | %s:%d | %s\n",
                                currentTimeToString(),
                                uptimeInSeconds(),
                                verbosityToString(iVerbosity),
                                kPrefix.value().c_str(),
                                iFile,
                                iLine,
                                iMessage);
    else
      re::logging::impl::printf("%s (%8.3fs) | %s | %s:%d | %s\n",
                                currentTimeToString(),
                                uptimeInSeconds(),
                                verbosityToString(iVerbosity),
                                iFile,
                                iLine,
                                iMessage);
  }

  if(iVerbosity == Verbosity::FATAL)
    fatal();
}

} // namespace re::logging::impl