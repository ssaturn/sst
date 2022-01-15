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
using int8 = char;
using int16 = short;
using int32 = int;
using int64 = long long;

// unsigned
using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

using size_t = std::size_t;
using byte = unsigned char;
using ushort = uint16;

// string
using wchar = WCHAR;

#if(0x0600 <= WINVER)
using tick_count_t = uint64;
#else
using tick_count_t = uint32;
#endif