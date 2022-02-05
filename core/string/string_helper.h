#pragma once

#include "../common.h"
//#include "../logger.h"
#include <Windows.h>
#include <string>
#include <concepts>


namespace sst::string
{
	class helper
	{
	public:
		//static std::string to_string_with_delimiter(std::integral auto&& number, const size_t range, const char delimiter = ',')
		//{
		//	try
		//	{
		//		std::string str = std::to_string(number);
		//		const std::size_t length = str.size();
		//		if (length <= range)
		//		{
		//			return str;
		//		}

		//		const std::size_t resize = length + length / range;
		//		str.resize(resize);
		//		// 1,000,000,000
		//		for(auto i = 0llu; i < resize; ++i)
		//		{
		//			const auto str_tail = length - 1; // 10 / 9
		//			const auto r_str_tail = resize - i - 1; // 13 - 0 - 1 // 12
		//			
		//			if (i > 0 && i % range == 0) // 0, 1, 2, 3, 4
		//			{							 // 000,
		//				str[r_str_tail] = delimiter;
		//				continue;
		//			}

		//			str[r_str_tail] = str[str_tail]; // 0, 1, 2, 3
		//		}
		//	}
		//	catch (std::exception&& e)
		//	{
		//		//log_debug() << e.what();
		//		return {};
		//	}

		//	return {};
		//}

		static int multi_to_wide(const char* multi_string, wchar_t* wide_string, const int32 count)
		{
			return ::MultiByteToWideChar(CP_ACP, 0, multi_string, -1, wide_string, count);
		}

		static int wide_to_multi(const wchar_t* wide_string, char* multi_string, const int32 count)
		{
			return ::WideCharToMultiByte(CP_ACP, 0, wide_string, -1, multi_string, count, nullptr, nullptr);
		}

		static int utf8_to_wide(const char* utf8_string, wchar_t* wide_string, const int32 count)
		{
			return ::MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, wide_string, count);
		}

		static int wide_to_utf8(const wchar_t* wide_string, char* utf8_string, const int32 count)
		{
			return ::WideCharToMultiByte(CP_UTF8, 0, wide_string, -1, utf8_string, count, nullptr, nullptr);
		}

		static std::wstring utf8_to_wide(const char* utf8_string)
		{
			const auto need_len = ::MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, nullptr, 0);
			if (need_len <= 0)
			{
				return {};
			}

			wchar_t wide_string[1024] = { 0, };
			::MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, wide_string, need_len);

			std::wstring result(wide_string);

			return result;
		}

		static std::string wide_to_utf8(const wchar_t* wide_string)
		{
			const auto need_len = ::WideCharToMultiByte(CP_UTF8, 0, wide_string, -1, nullptr, 0, nullptr, nullptr);
			if (need_len <= 0)
			{
				return {};
			}

			char utf8_string[1024] = { 0, };
			::WideCharToMultiByte(CP_UTF8, 0, wide_string, -1, utf8_string, need_len, nullptr, nullptr);

			std::string result(utf8_string);

			return result;
		}
	};
}


