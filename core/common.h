#pragma once

#include <WinSock2.h>
#include <array>
#include <cstdarg>	
#include <limits>
#include <process.h>
#include <string>

// c++ header
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <utility>

// c header
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <crtdbg.h>
#include <ctime>
#include <tchar.h>

#include "crash_handler.h"
#include "macro.h"

// signed
using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

// unsigned
using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using size_t = std::size_t;
using byte = std::uint8_t;
using ushort = uint16;

// string
using wchar = WCHAR;

#if(0x0600 <= WINVER)
using tick_count_t = std::uint64_t;
#else
using tick_count_t = std::uint32_t;
#endif