#include "stdafx.h"
#include "logger.h"
#include "log_manager.h"
#include <Windows.h>
#include <string_view>

sst::log_initializer* g_log_initializer = nullptr;


namespace
{
	using namespace std::literals;
	
	constexpr std::wstring_view console_log_key = L"Console"sv;
}

namespace sst
{
	//logger<log_level_t::debug> log_debug{};
	/*logger<log_level_t::info> log_info{};
	logger<log_level_t::warning> log_warn{};
	logger<log_level_t::error> log_error{};
	logger<log_level_t::fatal> log_fatal{};*/
	
	log_initializer::log_initializer()
	{
		if (g_log_manager != nullptr)
		{
			g_log_manager->add_publisher(console_log_key.data(), &console_publisher_);
		}
	}

	log_initializer::~log_initializer()
	{
	}

	void log_initializer::filter(log_level_t level)
	{
		console_publisher_.filter(level);
	}

	line_logger::line_logger(const wchar_t* file, const wchar_t* function, int line)
		: file_(file)
		, function_(function)
		, line_(line)
	{
	}

	line_logger& line_logger::debug(const wchar_t* format, ...)
	{
		va_list ap;
		va_start(ap, format);

		PushLog(log_level_t::debug, format, ap);

		va_end(ap);
		return *this;
	}

	line_logger& line_logger::info(const wchar_t* format, ...)
	{
		va_list ap;
		va_start(ap, format);

		PushLog(log_level_t::info, format, ap);

		va_end(ap);
		return *this;
	}

	line_logger& line_logger::warn(const wchar_t* format, ...)
	{
		va_list ap;
		va_start(ap, format);

		PushLog(log_level_t::warning, format, ap);

		va_end(ap);
		return *this;
	}

	line_logger& line_logger::error(const wchar_t* format, ...)
	{
		va_list ap;
		va_start(ap, format);

		PushLog(log_level_t::error, format, ap);

		va_end(ap);
		return *this;
	}

	line_logger& line_logger::fatal(const wchar_t* format, ...)
	{
		va_list ap;
		va_start(ap, format);

		PushLog(log_level_t::fatal, format, ap);

		va_end(ap);

		//Dump::Crash();
		return *this;
	}

	void line_logger::PushLog(log_level_t level, const wchar_t* format, va_list ap)
	{
		if (format == nullptr || g_log_manager == nullptr)
		{
			return;
		}

		wchar_t text[MAX_LOG_STRING + 1];
		int ret = ::vswprintf_s(text, MAX_LOG_STRING, format, ap);
		if (ret >= 0)
		{
			int newLinePos = -1;
			if (ret == 0)
			{
				// 빈 문자열
				newLinePos = ret;
			}
			else
			{
				if (text[ret - 1] != L'\n')
				{
					// 끝문자가 '\n'이 아니면 끝에 추가한다.
					newLinePos = ret;
				}
				else
				{
					// 끝문자 다음 문자가 \r이 아니면 
					if (ret < 2 || text[ret - 2] != L'\r')
					{
						newLinePos = ret - 1;
					}
				}
			}

			if (newLinePos >= 0)
			{
				text[newLinePos] = L'\r';
				text[newLinePos + 1] = L'\n';
				text[newLinePos + 2] = L'\0';
			}

			g_log_manager->push(console_log_key.data(), level, ::time(nullptr), file_, function_, line_, text);
		}
		else
		{
			assert(false);
		}
	}

}


