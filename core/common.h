#pragma once


#include <WinSock2.h>
#include <string>
#include <cstdio>	
#include <cstdarg>	
#include <limits>
#include <array>
#include <process.h>

// c++ header
#include <iterator>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <utility>

// c header
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cassert>
#include <tchar.h>
#include <crtdbg.h>

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