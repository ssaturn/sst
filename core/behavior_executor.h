#pragma once

#include "Behavior.h"

namespace sst
{
	class behavior_executor
	{
	public:
		explicit behavior_executor(behavior_owner* owner);
		virtual ~behavior_executor();

		bool start(behavior* behavior);
		void update();
		void end(behavior_end_reason reason);

		void clear();
		bool cancel(behavior_end_reason reason);
		void add_pend(behavior* behavior);
		behavior* get_current() const { return current_; }
		bool doing_something() const;
		bool doing(behavior_type type) const;

	private:
		void _Start_pending();
		void _Do_end(behavior_end_reason reason);


		behavior::behavior_queue pending_{};
		behavior* current_{ nullptr };
		behavior_owner* owner_{ nullptr };
	};
}