#include "stdafx.h"
#include "thread_usage.h"


namespace
{
	ULONGLONG convert_file_time_to_integer(const FILETIME* file_time)
	{
		return (static_cast<ULONGLONG>(file_time->dwHighDateTime) << 32) + file_time->dwLowDateTime;
	}
}

namespace sst::threading
{
	thread_usage::thread_usage(HANDLE handle)
		: handle_(handle)
	{
		if (handle_ == nullptr)
		{
			handle_ = ::GetCurrentThread();
		}
	}

	thread_usage::~thread_usage()
	{
		if (handle_ != nullptr)
		{
			::CloseHandle(handle_);
		}
	}

	double thread_usage::get_performance()
	{
		cpu_usage usage;

		if (handle_ != nullptr)
		{
			FILETIME creation_time{};
			FILETIME exit_time{};
			FILETIME kernel_time{};
			FILETIME user_time{};
			const auto current_tick = ::GetTickCount64();

			if (::GetThreadTimes(handle_, &creation_time, &exit_time, &kernel_time, &user_time) != FALSE)
			{
				const ULONGLONG current_kernel = convert_file_time_to_integer(&kernel_time);
				if (const ULONGLONG current_user = convert_file_time_to_integer(&user_time); last_check_tick_ == 0)
				{
					last_check_tick_ = current_tick;
					last_kernel_time_ = current_kernel;
					last_user_time_ = current_user;
				}
				else
				{
					const ULONGLONG user = current_user - last_user_time_;
					const ULONGLONG kernel = current_kernel - last_kernel_time_;
					const ULONGLONG total = user + kernel;

					usage.total = static_cast<double>(total) / (100. * (current_tick - last_check_tick_));
					if (total > 0)
					{
						//usage.user = (static_cast<double>(user) / total) * 100.;
						//usage.kernel = (static_cast<double>(kernel) / total) * 100.;
					}

					last_check_tick_ = current_tick;
					last_kernel_time_ = current_kernel;
					last_user_time_ = current_user;
				}
			}
		}

		return usage.total;
	}
}
