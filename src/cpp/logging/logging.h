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

#pragma once

#ifndef PONGASOFT_RE_LOGGING_H
#define PONGASOFT_RE_LOGGING_H

// Implementation note: when doing jbox builds, logging.cpp cannot be compiled because it relies on includes
// which are not part of the RE SDK due to sandboxing, as a result, we disable logging in this instance
#if LOCAL_NATIVE_BUILD && DEBUG
// local native build => debugging enabled
#define RE_LOGGING_ENABLED 1
#else
// jbox build => debugging disabled
#define RE_LOGGING_ENABLED 0
#endif // LOCAL_NATIVE_BUILD

#include <JukeboxTypes.h>

/////////////////////////////////////////////////////////////
// API
/////////////////////////////////////////////////////////////


#if RE_LOGGING_ENABLED

#include <string>

namespace re::logging {

/**
 * This function can be called when the device is created to make the output nicer */
void init_for_re(char const *iREName = nullptr);

/**
 * This function can be used from tests to replace aborts with exception (which can be checked) */
void init_for_test(char const *iPrefix = nullptr);

}

#define RE_LOGGING_SET_VERBOSITY(verbosity) re::logging::impl::setVerbosity(re::logging::impl::Verbosity::verbosity)
#define DLOG_F(verbosity, ...) re::logging::impl::log(re::logging::impl::Verbosity::verbosity, __FILE__, __LINE__, __VA_ARGS__)
#define ABORT_F(...) re::logging::impl::abort(__FILE__, __LINE__, __VA_ARGS__)

#else

// noop when logging is not enabled at compilation time

#define RE_LOGGING_SET_VERBOSITY(verbosity)
#define DLOG_F(verbosity, ...)
#define ABORT_F(...)

#endif // RE_LOGGING_ENABLED

#if DEBUG

/**
 * Allow to write simpler code:
 *
 * ```cpp
 *    // using JBOX_TRACEVALUES
 * 		TJBox_Value instanceIDValue = JBox_MakeNumber(JBox_GetNumber(iParams[0]));
 *		TJBox_Value array[1];
 *		array[0] = instanceIDValue;
 *		JBOX_TRACEVALUES("instance ID = ^0", array, 1);
 *
 *    // using JBOX_LOGVALUES
 *		JBOX_LOGVALUES("instance ID = ^0", iParams[0]));
 * ```
 */
#define JBOX_LOGVALUES(iMessage, ...) re::logging::impl::JBox_LogValues(__FILE__, __LINE__, iMessage, __VA_ARGS__)
#else
#define JBOX_LOGVALUES(iMessage, ...)
#endif

/////////////////////////////////////////////////////////////
// Implementation (do not use directly)
/////////////////////////////////////////////////////////////

#if DEBUG
namespace re::logging::impl {

template<typename... Args>
inline void JBox_LogValues(const char iFile[], TJBox_Int32 iLine, char const *iMessage, Args&& ...iValues)
{
  TJBox_Value values[sizeof...(iValues)] { iValues... };
  JBox_TraceValues(iFile, iLine, iMessage, values, sizeof...(iValues));
}

}
#endif // DEBUG

#if RE_LOGGING_ENABLED

namespace re::logging::impl {

enum class Verbosity : int
{
  FATAL = -3,
  ERROR = -2,
  WARNING = -1,
  INFO = 0,
  VERBOSE = 1,
};

void setVerbosity(Verbosity iVerbosity);

template<typename T>
constexpr auto printf_arg(T const &t) { return t; }

// Handles std::string without having to call c_str all the time
template<>
inline auto printf_arg<std::string>(std::string const &t) { return t.c_str(); }

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
#endif

/*
 * Copied from https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf */
template<typename ... Args>
std::string _sprintf(std::string_view format, Args ... args)
{
  int size_s = snprintf(nullptr, 0, format.data(), args ...) + 1; // Extra space for '\0'
  if(size_s <= 0)
  { throw std::runtime_error("Error during formatting."); }
  auto size = static_cast<size_t>( size_s );
  auto buf = std::make_unique<char[]>(size);
  snprintf(buf.get(), size_s, format.data(), args ...);
  return {buf.get(), buf.get() + size - 1}; // We don't want the '\0' inside
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

[[noreturn]] void fatal();

void log(Verbosity iVerbosity, char const *iFile, int iLine, std::string_view iMessage);

//------------------------------------------------------------------------
// sprintf -> std::string
//------------------------------------------------------------------------
template<typename ... Args>
inline std::string sprintf(std::string_view format, Args ... args)
{
  return _sprintf(format, printf_arg(args)...);
}

//------------------------------------------------------------------------
// printf
//------------------------------------------------------------------------
template<typename ... Args>
inline void printf(std::string_view format, Args ... args)
{
  ::printf(format.data(), printf_arg(args)...);
}

//------------------------------------------------------------------------
// log
//------------------------------------------------------------------------
template<typename ... Args>
inline void log(Verbosity iVerbosity, char const *iFile, int iLine, std::string_view format, Args ... args)
{
  impl::log(iVerbosity, iFile, iLine, sprintf(format, std::forward<Args>(args)...));
}

//------------------------------------------------------------------------
// abort
//------------------------------------------------------------------------
template<typename ... Args>
[[noreturn]] inline void abort(char const *iFile, int iLine, std::string_view format, Args ... args)
{
  impl::log(re::logging::impl::Verbosity::FATAL, iFile, iLine, format, std::forward<Args>(args)...);
  fatal();
}

} // namespace re::logging::impl

#endif // RE_LOGGING_ENABLED

/////////////////////////////////////////////////////////////
// Deprecation
/////////////////////////////////////////////////////////////

#if RE_LOGGING_ENABLED

namespace loguru {
[[deprecated("Use re::logging::init_for_re instead")]]
inline void init_for_re(char const *iREName = nullptr) { re::logging::init_for_re(iREName); }
[[deprecated("Use re::logging::init_for_test instead")]]
inline void init_for_test(char const *iPrefix = nullptr) { re::logging::init_for_test(iPrefix); }
}

#endif // RE_LOGGING_ENABLED

#endif // PONGASOFT_RE_LOGGING_H