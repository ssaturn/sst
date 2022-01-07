#pragma once

#include "logging.h"
#include "log_publisher.h"
#include <source_location>
#include <iostream>
#include <sstream>
#include <core/string_helper.h>

namespace sst
{
	class log_initializer
	{
		log_console_publisher	console_publisher_;

	public:
		log_initializer();
		~log_initializer();

		void filter(log_level_t level);

	};

	class line_logger
	{
		enum
		{
			MAX_LOG_STRING = 8 * 1024,
		};

		const wchar_t* file_{};
		const wchar_t* function_{};
		int32 line_{};

	public:

		line_logger(const wchar_t* file, const wchar_t* function, int line);

		line_logger& debug(const wchar_t* format, ...);
		line_logger& info(const wchar_t* format, ...);
		line_logger& warn(const wchar_t* format, ...);
		line_logger& error(const wchar_t* format, ...);
		line_logger& fatal(const wchar_t* format, ...);

	private:
		void PushLog(log_level_t level, const wchar_t* format, va_list ap);

	};

	struct slocal
	{
	public:
		explicit slocal(const std::source_location location = std::source_location::current())
			: location(location)
		{
		}

		std::source_location location;
	};
	
	
	template<log_level_t LogLevel>
	class logger
	{
	public:
		static constexpr size_t max_log_str_len = 8 * 1024;

		logger() = default;
		~logger()
		{
			std::wcout << wss_.str() << std::endl;
		}
		logger(logger&& other)
		{
			wss_ = std::move(other.wss_);
		}
		
		/*logger<LogLevel>& operator<<([[maybe_unused]] logger<LogLevel>& logger)
		{
			
			return *this;
		}*/

		logger<LogLevel>& operator<<([[maybe_unused]] std::wstring& string)
		{
			wss_ << string;
			return *this;
		}

		logger<LogLevel>& operator<<([[maybe_unused]] std::wstring_view& string)
		{
			wss_ << string;
			return *this;
		}

		logger<LogLevel>& operator<<(int& val)
		{
			wss_ << val;
			
			return *this;
		}
		

		logger<LogLevel>& operator<<(const wchar_t* str)
		{
			wss_ << str;
			return *this;
		}

		logger<LogLevel>& operator<<(const char* str)
		{
			wchar_t wstr[1024]{ 0 };
			string::helper::multi_to_wide(str, wstr, _countof(wstr));
			wss_ << wstr;
			
			return *this;
		}

		logger<LogLevel>& operator<<(const std::source_location& location)
		{
			wss_ << location.file_name() << "("
				<< location.line() << ":"
				<< location.column() << ") `"
				<< location.function_name() << "`: ";

			return *this;
		}

		logger<LogLevel>& operator<<(const slocal& sloc)
		{
			wss_ << sloc.location.file_name() << "("
				<< sloc.location.line() << ":"
				<< sloc.location.column() << ") `"
				<< sloc.location.function_name() << "`: ";

			return *this;
		}

	private:
		std::wstringstream wss_{};
	};

	

	template<log_level_t LLTy>
	logger<LLTy> log(const std::source_location location = std::source_location::current())
	{
		logger<LLTy> logger;
		logger << location;

		return std::move(logger);
	}

	/*extern logger<log_level_t::info> log_info;
	extern logger<log_level_t::warning> log_warn;
	extern logger<log_level_t::error> log_error;
	extern logger<log_level_t::fatal> log_fatal;*/
	//__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2_IMPORT logger<log_level_t::info> log_debug;
	
#define log_debug sst::log<sst::log_level_t::debug>()
#define log_info log<sst::log_level_t::info>()
#define log_warning log<sst::log_level_t::warning>()
#define log_error log<sst::log_level_t::error>()
#define log_fatal log<sst::log_level_t::fatal>()
}



extern sst::log_initializer* g_log_initializer;