#pragma once

#include <string>


namespace sst
{
	enum class log_level_t : uint16
	{
		all,

		debug,
		info,
		warning,
		error,
		fatal,

		max,
	};

	static const wchar_t* log_level_string[] =
	{
		L"ALL",
		L"DEBUG",
		L"INFO",
		L"WARNING",
		L"ERROR",
		L"FATAL",
		L"UNKNOWN",
	};

	class async_log
	{
	public:

		const wchar_t* log_key{ nullptr };
		log_level_t		log_level{};
		time_t			log_time{};

		const wchar_t* log_file{ nullptr };
		const wchar_t* log_function{ nullptr };
		int				log_line{ 0 };

		std::wstring	log_text{};

		bool			need_finish{ false };

		async_log(const wchar_t* key, const log_level_t level, const time_t time, const wchar_t* file,
				  const wchar_t* function, const int32 line, const std::wstring& text, const bool finish)
			: log_key(key)
			, log_level(level)
			, log_time(time)
			, log_file(file)
			, log_function(function)
			, log_line(line)
			, log_text(text)
			, need_finish(finish)
		{
		}
		async_log(const wchar_t* key, const log_level_t level, const time_t time, const wchar_t* file,
				  const wchar_t* function, const int32 line, const wchar_t* text, const bool finish)
			: log_key(key)
			, log_level(level)
			, log_time(time)
			, log_file(file)
			, log_function(function)
			, log_line(line)
			, log_text(text)
			, need_finish(finish)
		{
		}
	};

}
