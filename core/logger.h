#pragma once

#include "Logging.h"
#include "log_publisher.h"


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
}

#define LOG() sst::line_logger(__FILEW__, __FUNCTIONW__, __LINE__)

extern sst::log_initializer* g_log_initializer;