#pragma once

#include <Windows.h>
#include "Logging.h"

namespace sst
{
	class log_publisher
	{
	public:
		virtual ~log_publisher() {}

		virtual void publish(async_log* log) = 0;
		virtual void close() = 0;

	};

	class log_console_publisher final : public log_publisher
	{
		volatile log_level_t filter_;

	public:
		log_console_publisher();

		void	publish(async_log* log) override;
		void	close() override {}

		void	debug(const wchar_t* format, ...);
		void	info(const wchar_t* format, ...);
		void	warn(const wchar_t* format, ...);
		void	error(const wchar_t* format, ...);
		void	fatal(const wchar_t* format, ...);

		void	filter(log_level_t level);

	};

	class log_file_publisher : public log_publisher
	{
	private:
		HANDLE log_file_;

	public:
		explicit log_file_publisher(HANDLE log_file);
		~log_file_publisher();

		void publish(async_log* log) override;
		void close() override;

	};
}
