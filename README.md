Introduction
------------

This project is a clone of [loguru](https://github.com/emilk/loguru) containing only the relevant files + extension
apis for Rack Extension development.

Install
-------

* Include `re-logging.cmake` in the project
* Add `${re-logging_INCLUDE_DIRS}` to `target_include_directories()`
* Because of the rack extension environment/limitations, `loguru.cpp` can only be compiled in native builds

The project is then included like this:

```cpp
#include <logging.h>
```

It is strongly recommended to use [re-cmake](https://github.com/pongasoft/re-cmake) for building a rack extension.

Check [RE Quickstart](https://pongasoft.com/re-quickstart/index.html) for a quick and convenient way to generate a blank rack extension that use `re-logging`.

Usage
-----

In addition to all the built-in loguru features, this projects adds:

* A define `JBOX_LOGVALUES` to replace `JBOX_TRACEVALUES` (provide a simpler API)
* `loguru::init_for_re()` which can be used for the rack extension itself
* `loguru::init_for_test()` which can be used when writing unit test to use exceptions instead of aborting
* `loguru::add_preamble_handler()` to further tweak the output of loguru

Release notes
-------------

#### 1.0.2 - 2024/10/08

- Does not fail anymore when included via `FetchContent_MakeAvailable`

#### 1.0.1 - 2022/02/01

- Fix for Windows 10 build

#### 1.0.0 - 2022/01/24

- First release.

License
-------

- This project is released under the terms of the [Apache 2.0 license](LICENSE.txt)

- This project is a clone of [loguru](https://github.com/emilk/loguru) which is distributed with the following license:
  > This software is in the public domain. Where that dedication is not recognized, you are granted a perpetual, irrevocable license to copy, modify and distribute it as you see fit

- This project uses the Rack extension SDK released under the [Rack Extension License agreement](RE_License.txt)
