#pragma once

//#include <windows.h>
#include <WinSock2.h>
//#include <mstcpip.h>
//#include <ws2tcpip.h>
//#include <ws2ipdef.h>			// sockaddr_in6
//#include <iphlpapi.h>
#include <string>
#include <cstdio>				// for va_arg...
#include <cstdarg>				// for va_arg...
#include <limits>				// for IsAssignable().
#include <array>
#include <process.h>
//#include <direct.h>
//#include <pdh.h>
//#include <sqltypes.h>

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
#include <wchar.h>
#include <tchar.h>
#include <crtdbg.h>

#include "CrashHandler.h"
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