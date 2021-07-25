#pragma once

#include <queue>
#include <chrono>

namespace sst
{
	enum class behavior_type
	{
		BT_NONE = 0,
		BT_BAGIC,
	};

	enum class behavior_end_reason
	{
		BER_NONE = 0,
		BER_NORMAL,
		BER_INTERNAL_ERROR,
		BER_MAX
	};
	using BER = behavior_end_reason;

	class behavior_owner
	{
	public:
		behavior_owner() {}
		virtual ~behavior_owner() {}
	};

	// no thread-safe
	class behavior
	{
	public:
		friend class BehaviorExecutor;

		using behavior_queue = std::deque<behavior*>;
		behavior() = delete;
		behavior(behavior_owner* owner, behavior_type type);
		virtual ~behavior();

		void OnBegin() {}
		void OnProcess() {}
		void OnEnd(behavior_end_reason reason) { reason; }

		void add_pending(behavior* behavior) { pending_.push_back(behavior); }
		bool done() const;
		behavior_end_reason end_reason() const { return end_reason_; }
		behavior_type type() const { return type_; }
		const behavior_queue& get_pending() const { return pending_; }

	protected:
		virtual void onBegin() = 0;
		virtual void onProcess() = 0;
		virtual void onEnd(behavior_end_reason reason) = 0;

		void SetEnd(int64 after, behavior_end_reason reason);

	private:
		behavior_owner* owner_{ nullptr };
		behavior_queue pending_{};
		bool done_{ false };
		behavior_type type_{ behavior_type::BT_BAGIC };
		behavior_end_reason end_reason_{ behavior_end_reason::BER_NONE };
		std::chrono::system_clock::time_point end_tick_{};
	};
}