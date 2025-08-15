/*
 * Copyright (c) 2022-2025 pongasoft
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
// All macros are noop when compiling in jbox mode
/////////////////////////////////////////////////////////////

#ifndef RE_LOGGING_DEFAULT_VERBOSITY
#define RE_LOGGING_DEFAULT_VERBOSITY INFO
#endif

#ifndef RE_LOGGING_STRIP_FILE_PATH
#define RE_LOGGING_STRIP_FILE_PATH 1
#endif

#if RE_LOGGING_ENABLED

#include <string>

/**
 * This macro can be called when the device is created to make the output nicer */
#define RE_LOGGING_INIT_FOR_RE(re_name) re::logging::impl::init_for_re(re_name)

/**
 * This macro can be used from tests to replace "abort" with exception (which can be checked) */
#define RE_LOGGING_INIT_FOR_TEST(prefix) re::logging::impl::init_for_test(prefix)

/**
 * This macro changes the verbosity level (default is INFO). Possible values are:
 * FATAL (which will also abort)
 * ERROR
 * WARNING
 * INFO
 * VERBOSE */
#define RE_LOGGING_SET_VERBOSITY(verbosity) re::logging::impl::setVerbosity(re::logging::impl::Verbosity::verbosity)

/**
 * This is the main macro used for logging.
 * Example usage:
 * ```cpp
 * DLOG_F(INFO, "framerate=%d", 48000);
 * ```
 * See `RE_LOGGING_SET_VERBOSITY` for possible verbosity values */
#define DLOG_F(verbosity, ...) re::logging::impl::log(re::logging::impl::Verbosity::verbosity, __FILE__, __LINE__, __VA_ARGS__)

/**
 * This macro checks that the expression is valid and aborts if not.
 * Example usage:
 * ```cpp
 * DCHECK_F(framerate == 41000, "wrong frame rate: %d", framerate);
 * ``` */
#define DCHECK_F(test, ...) ((test) == true) ? (void)0 : re::logging::impl::abort("CHECK FAILED:  " #test "  ", __FILE__, __LINE__, ##__VA_ARGS__)

/**
 * This set of macros are delegating to `DCHECK_F` and are being kept for backward compatibility with loguru */
#define DCHECK_NOTNULL_F(x, ...) DCHECK_F((x) != nullptr, ##__VA_ARGS__)
#define DCHECK_EQ_F(a, b, ...)   DCHECK_F((a) == (b), ##__VA_ARGS__)
#define DCHECK_NE_F(a, b, ...)   DCHECK_F((a) != (b), ##__VA_ARGS__)
#define DCHECK_LT_F(a, b, ...)   DCHECK_F((a) < (b), ##__VA_ARGS__)
#define DCHECK_LE_F(a, b, ...)   DCHECK_F((a) <= (b), ##__VA_ARGS__)
#define DCHECK_GT_F(a, b, ...)   DCHECK_F((a) > (b), ##__VA_ARGS__)
#define DCHECK_GE_F(a, b, ...)   DCHECK_F((a) >= (b), ##__VA_ARGS__)

/**
 * Logs the message and aborts.
 * Note that if `RE_LOGGING_INIT_FOR_TEST` is called, then it will throw a catchable exception instead of aborting which
 * is useful for testing purposes */
#define ABORT_F(...) re::logging::impl::abort("ABORT: ", __FILE__, __LINE__, ##__VA_ARGS__)

#else

// noop when logging is not enabled at compilation time

#define RE_LOGGING_INIT_FOR_RE(re_name)
#define RE_LOGGING_INIT_FOR_TEST(prefix)
#define RE_LOGGING_SET_VERBOSITY(verbosity)
#define DLOG_F(verbosity, ...)
#define DCHECK_F(test, ...)
#define DCHECK_NOTNULL_F(x, ...)
#define DCHECK_EQ_F(a, b, ...)
#define DCHECK_NE_F(a, b, ...)
#define DCHECK_LT_F(a, b, ...)
#define DCHECK_LE_F(a, b, ...)
#define DCHECK_GT_F(a, b, ...)
#define DCHECK_GE_F(a, b, ...)
#define ABORT_F(...)

#endif // RE_LOGGING_ENABLED

#if DEBUG

/**
 * Allows writing simpler code:
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
void init_for_re(char const *iREName = nullptr);
void init_for_test(char const *iPrefix = nullptr);

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
  if(size_s <= 0) { throw std::runtime_error("Error during formatting."); }
  auto size = static_cast<size_t>( size_s );
  auto buf = std::make_unique<char[]>(size);
  snprintf(buf.get(), size_s, format.data(), args ...);
  return {buf.get(), buf.get() + size - 1}; // We don't want the '\0' inside
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

[[noreturn]] void fatal();

void doLog(Verbosity iVerbosity, char const *iFile, int iLine, std::string_view iMessage);

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
  impl::doLog(iVerbosity, iFile, iLine, sprintf(format, std::forward<Args>(args)...));
  if(iVerbosity == Verbosity::FATAL)
    fatal();
}

//------------------------------------------------------------------------
// abort
//------------------------------------------------------------------------
[[noreturn]] inline void abort(char const *iInfo, char const *iFile, int iLine)
{
  impl::doLog(re::logging::impl::Verbosity::FATAL, iFile, iLine, iInfo);
  fatal();
}

//------------------------------------------------------------------------
// abort
//------------------------------------------------------------------------
template<typename ... Args>
[[noreturn]] inline void abort(char const *iInfo, char const *iFile, int iLine, std::string_view format, Args ... args)
{
  impl::doLog(re::logging::impl::Verbosity::FATAL, iFile, iLine,
              sprintf(std::string("%s") + format.data(), iInfo, std::forward<Args>(args)...));
  fatal();
}

} // namespace re::logging::impl

#endif // RE_LOGGING_ENABLED

/////////////////////////////////////////////////////////////
// Deprecation (kept for backward compatibility)
/////////////////////////////////////////////////////////////

#if RE_LOGGING_ENABLED

namespace loguru {
[[deprecated("Use RE_LOGGING_INIT_FOR_RE instead")]]
inline void init_for_re(char const *iREName = nullptr) { RE_LOGGING_INIT_FOR_RE(iREName); }
[[deprecated("Use RE_LOGGING_INIT_FOR_TEST instead")]]
inline void init_for_test(char const *iPrefix = nullptr) { RE_LOGGING_INIT_FOR_TEST(iPrefix); }
}

#endif // RE_LOGGING_ENABLED

#endif // PONGASOFT_RE_LOGGING_H