#include "stdafx.h"
#include "log_publisher.h"
#include "logging.h"
#include "string_helper.h"


namespace sst
{
	HANDLE std_out_handle = INVALID_HANDLE_VALUE;

	const WORD color_table[static_cast<size_t>(sst::log_level_t::max)] =
	{
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,	// WHITE	// default
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // WHITE	// debug
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // WHITE	// info
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,					// YELLOW	// warning
		FOREGROUND_RED | FOREGROUND_INTENSITY,										// RED		// error
		FOREGROUND_RED | FOREGROUND_INTENSITY,										// RED		// fatal
	};

	void SetConsoleColor(sst::log_level_t log_level)
	{
		if (std_out_handle == INVALID_HANDLE_VALUE)
		{
			std_out_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
		}

		::SetConsoleTextAttribute(std_out_handle, color_table[static_cast<size_t>(log_level)]);
	}

	void ResetConsoleColor()
	{
		if (std_out_handle == INVALID_HANDLE_VALUE)
		{
			std_out_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
		}

		::SetConsoleTextAttribute(std_out_handle, color_table[0]);
	}
}

namespace sst
{
	log_console_publisher::log_console_publisher()
		: filter_(log_level_t::all)
	{
		ResetConsoleColor();
	}



	void log_console_publisher::publish(async_log* log)
	{
		if (log->log_level < filter_)
		{
			return;
		}

		SYSTEMTIME time;
		::GetLocalTime(&time);

		switch (log->log_level)
		{
		case log_level_t::debug:
			debug(L"[%s]\t%s",
				log_level_string[static_cast<size_t>(log->log_level)],
				log->log_text.c_str());
			break;
		case log_level_t::info:
			info(L"[%s]\t%s",
				log_level_string[static_cast<size_t>(log->log_level)],
				log->log_text.c_str());
			break;
		case log_level_t::warning:
			warn(L"[%s]\t%s",
				log_level_string[static_cast<size_t>(log->log_level)],
				log->log_text.c_str());
			break;
		case log_level_t::error:
			error(L"[%s]\t%s",
				log_level_string[static_cast<size_t>(log->log_level)],
				log->log_text.c_str());
			break;
		case log_level_t::fatal:
			fatal(L"[%s]\t%s",
				log_level_string[static_cast<size_t>(log->log_level)],
				log->log_text.c_str());
			break;
		default:
			error(L"[%s]\t%s",
				log_level_string[static_cast<size_t>(log_level_t::max)],
				log->log_text.c_str());
			break;
		}
	}

	void log_console_publisher::debug(const wchar_t* format, ...)
	{
		if (format == nullptr)
			return;

		SetConsoleColor(log_level_t::debug);

		va_list ap;
		va_start(ap, format);
		::vwprintf_s(format, ap);
		va_end(ap);

		::fflush(stdout);

		ResetConsoleColor();
	}

	void log_console_publisher::info(const wchar_t* format, ...)
	{
		if (format == nullptr)
			return;

		SetConsoleColor(log_level_t::info);

		va_list ap;
		va_start(ap, format);
		::vwprintf_s(format, ap);
		va_end(ap);

		::fflush(stdout);

		ResetConsoleColor();
	}

	void log_console_publisher::warn(const wchar_t* format, ...)
	{
		if (format == nullptr)
		{
			return;
		}


		SetConsoleColor(log_level_t::warning);

		va_list ap;
		va_start(ap, format);
		::vwprintf_s(format, ap);
		va_end(ap);

		::fflush(stdout);

		ResetConsoleColor();
	}

	void log_console_publisher::error(const wchar_t* format, ...)
	{
		if (format == nullptr)
		{
			return;
		}

		SetConsoleColor(log_level_t::error);

		va_list ap;
		va_start(ap, format);
		::vwprintf_s(format, ap);
		va_end(ap);

		::fflush(stdout);

		ResetConsoleColor();
	}

	void log_console_publisher::fatal(const wchar_t* format, ...)
	{
		if (format == nullptr)
		{
			return;
		}

		SetConsoleColor(log_level_t::fatal);

		va_list ap;
		va_start(ap, format);
		::vwprintf_s(format, ap);
		va_end(ap);

		::fflush(stdout);

		ResetConsoleColor();

		//Dump::Crash();

		::ExitProcess(1);
	}

	void log_console_publisher::filter(const log_level_t level)
	{
		filter_ = level;
	}

	log_file_publisher::log_file_publisher(const HANDLE log_file)
		: log_file_(log_file)
	{
	}

	log_file_publisher::~log_file_publisher()
	{
		close();
	}

	void log_file_publisher::publish(async_log* log)
	{
		auto level_string_idx = static_cast<size_t>(log->log_level);
		if (0 > level_string_idx || level_string_idx >= _countof(log_level_string))
		{
			level_string_idx = static_cast<size_t>(log_level_t::max);
		}

		SYSTEMTIME time;
		::GetLocalTime(&time);

		wchar_t logHeader[1024] = { 0, };
		swprintf_s(logHeader, L"[%s]\t%04d-%02d-%02d %02d:%02d:%02d(%03d)\t%s(%d)\t",
			log_level_string[level_string_idx],
			time.wYear, time.wMonth, time.wDay,
			time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
			(log->log_function != nullptr) ? log->log_function : L"Unknown_Function", log->log_line);

		std::wstring logText(logHeader);
		logText += log->log_text;

		if (log_file_ != INVALID_HANDLE_VALUE)
		{
			const auto write_bytes = static_cast<DWORD>(sizeof(wchar_t) * logText.size());
			DWORD written_bytes = 0;
			::WriteFile(log_file_, logText.c_str(), write_bytes, &written_bytes, nullptr);
		}
	}

	void log_file_publisher::close()
	{
		if (log_file_ != INVALID_HANDLE_VALUE)
		{
			::FlushFileBuffers(log_file_);
			::CloseHandle(log_file_);

			log_file_ = INVALID_HANDLE_VALUE;
		}
	}

}

