#pragma once

#include <functional>
#include <vector>
#include <queue>
#include <chrono>

#include "common.h"

namespace sst
{
	class job_timer
	{
	public:
		explicit job_timer(const std::function<void()>& func, const uint64 expire_tick)
			: expire_tick(expire_tick)
			, job(func)
		{

		}

		uint64 expire_tick{ 0 };
		std::function<void()> job{};

		struct compare
		{
			bool operator()(const job_timer& lhs, const job_timer& rhs) const
			{
				return (lhs.expire_tick > rhs.expire_tick);
			}
		};
	};

	class job_timer_queue
	{
	public:
		enum
		{
			slot_max = 4096,			// 1분 조금 넘는 시간을 담을 수 있다.
			slot_tick_interval = 15,
		};

		using time_slot = std::vector<job_timer*>;
		using timer_queue = std::priority_queue<job_timer, std::vector<job_timer>, job_timer::compare>;

		void push(job_timer* job, const int64 after)
		{
			const uint64 expire_tick = GetTickCount64() + after;
			const int64 tick_diff = expire_tick > nex_tick_ ? expire_tick - nex_tick_ : 0;
			int64 after_slot = tick_diff / slot_tick_interval;

			// 현재 슬롯은 사용하고 있는 중일 수 있기 때문에 넣지 않는다.
			if (after_slot == 0)
			{
				++after_slot;
			}

			if (after_slot < slot_max)
			{
				const DWORD insert_slot = (current_ + after_slot) % slot_max;
				slots_[insert_slot].push_back(job);
			}
			else
			{
				// SLOT_MAX * SLOT_TICK_INTERVAL 이후 일어나는 Job, ex) 65536ms 이후 Job
				//queue_.push( JobTimer( job, expireTick ) );
			}
		}

	private:
		// short term
		time_slot slots_[slot_max];
		
		// long term
		timer_queue	queue_{};

		uint16 current_{ 0 };
		uint64 nex_tick_{ 0 };
	};


}
