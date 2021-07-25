#pragma once

#include <Windows.h>


namespace sst::threading
{
	struct cpu_usage
	{
		double total{};
		double user{};
		double kernel{};
	};

	class thread_usage
	{
	public:
		explicit thread_usage(HANDLE handle);
		thread_usage() = delete;
		~thread_usage();

		double get_performance();

	private:
		HANDLE handle_{ INVALID_HANDLE_VALUE };
		ULONGLONG last_check_tick_{};
		ULONGLONG last_kernel_time_{};
		ULONGLONG last_user_time_{};
	};
}
