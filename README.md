Introduction
------------

This project started as a clone of [loguru](https://github.com/emilk/loguru). Since 2.0.0, this is a very stripped-down version of loguru
that works well in the context of a Rack Extension.
See [here](https://github.com/pongasoft/re-logging/issues/1) for more details.

Install
-------

* Include `re-logging.cmake` in the project
* Add `${re-logging_INCLUDE_DIRS}` to `target_include_directories()`
* Because of the rack extension environment/limitations, `logging.cpp` can only be compiled in native builds

The project is then included like this:

```cpp
#include <logging.h>
```

It is strongly recommended to use [re-cmake](https://github.com/pongasoft/re-cmake) for building a rack extension.

Check [RE Quickstart](https://pongasoft.com/re-quickstart/index.html) for a quick and convenient way to generate a blank rack extension that use `re-logging`.

Usage
-----

This project offers a set of macros:

<table>
<tr>
<th>Macro</th><th>Description</th>
</tr>
<tr>
<td><code>DLOG_F(verbosity, ...)</code></td>
<td>The main macro used for logging (note that the name was kept from loguru).
Using verbosity <code>FATAL</code> will cause the message to be displayed and the program aborted.</td>
</tr>
<tr>
<td><code>RE_LOGGING_INIT_FOR_RE(re_name)</code></td><td>This macro can be called when the device is created to make the output nicer</td>
</tr>
<tr>
<td><code>RE_LOGGING_INIT_FOR_TEST(prefix)</code></td><td>This macro can be used from tests to replace "abort" with exception (which can be checked)</td>
</tr>
<tr>
<td><code>RE_LOGGING_SET_VERBOSITY(verbosity)</code></td>
<td>This macro changes the verbosity level. Possible values are:
<ul>
<li><code>FATAL</code></li>
<li><code>ERROR</code></li>
<li><code>WARNING</code></li>
<li><code>INFO</code> (default)</li>
<li><code>VERBOSE</code></li>
</ul>
</td>
</tr>
<tr>
<td><code>RE_LOGGING_STRIP_FILE_PATH()</code></td><td>This macro strips the file path from the file leaving just the filename (default)</td>
</tr>
<tr>
<td><code>RE_LOGGING_KEEP_FILE_PATH()</code></td><td>This macro keeps the file path from the file</td>
</tr>
<tr>
<td><code>ABORT_F(...)</code></td>
<td>Logs the message and aborts. Note that if <code>RE_LOGGING_INIT_FOR_TEST</code> is called, then it will throw a catchable exception instead of aborting which is useful for testing purposes</td>
</tr>
<tr>
<td><code>JBOX_LOGVALUES(iMessage, ...)</code></td>
<td>Allows writing simpler code
<pre>
// using JBOX_TRACEVALUES
TJBox_Value instanceIDValue = JBox_MakeNumber(JBox_GetNumber(iParams[0]));
TJBox_Value array[1];
array[0] = instanceIDValue;
JBOX_TRACEVALUES("instance ID = ^0", array, 1);

// using JBOX_LOGVALUES
JBOX_LOGVALUES("instance ID = ^0", iParams[0]));
</pre>
</td>
</tr>
</table>

Release notes
-------------

### 2.0.0 - 2025/08/14

- Removed most of the loguru implementation in favor of a lighter implementation ([#1])
- Only the major loguru APIs were kept (`DLOG_F` and `ABORT_F`)
- Added `RE_LOGGING_SET_VERBOSITY`, `RE_LOGGING_STRIP_FILE_PATH`, `RE_LOGGING_KEEP_FILE_PATH` macros
- Replaced `loguru::init_for_re` (now deprecated) with the macro `RE_LOGGING_INIT_FOR_RE`
- Replaced `loguru::init_for_test` (now deprecated) with the macro `RE_LOGGING_INIT_FOR_TEST`

#### 1.0.2 - 2024/10/08

- Does not fail anymore when included via `FetchContent_MakeAvailable`

#### 1.0.1 - 2022/02/01

- Fix for Windows 10 build

#### 1.0.0 - 2022/01/24

- First release.

License
-------

- This project is released under the terms of the [Apache 2.0 license](LICENSE.txt)

- This project is a very stripped-down version of [loguru](https://github.com/emilk/loguru) which is distributed with the following license:
  > This software is in the public domain. Where that dedication is not recognized, you are granted a perpetual, irrevocable license to copy, modify and distribute it as you see fit

- This project uses the Rack extension SDK released under the [Rack Extension License agreement](RE_License.txt)
