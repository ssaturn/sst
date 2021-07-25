#pragma once

#include "common.h"

namespace sst
{
	template<int32 MaxState>
	class fsm
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(fsm)

	public:
		class state
		{
			DISALLOW_SPECIAL_MEMBER_FUNCTIONS(state)

		public:
			virtual ~state() = default;
		
		protected:
			state()
			{
				Initialize();
			}
			

			int64 GetStateTickCount() const { return GetTickCount64() - start_tick_; }

			virtual void OnEnter() = 0;
			virtual void OnTick() = 0;
			virtual void OnExit() = 0;
			virtual int32 EvaluateStateTransition() = 0;

			void Initialize()
			{
				start_tick_ = GetTickCount64();
			}

		private:
			int64 start_tick_{ 0 };
		};

		explicit fsm(int32 current_state);
		fsm();
		~fsm();

		void tick();
		template<typename StateTy, typename ... Args> requires std::is_base_of_v<state, StateTy>
		void register_state(int32 state_id, Args&& ... args)
		{
			assert(state_id >= 0 && state_id < MaxState);
			assert(fsm_state_[state_id] == NULL);

			fsm_state_[state_id] = new StateTy(std::forward<decltype(args)>(args)...);
		}

		state* current();

	private:
		state* fsm_state_[MaxState]; ///< 각 상태들.
		int32 current_state_{ 0 };
	};



	template <int32 MaxState>
	fsm<MaxState>::fsm()
	{
		memset(fsm_state_, 0, sizeof(fsm_state_));
	}

	template <int32 MaxState>
	fsm<MaxState>::~fsm()
	{
		for (int i = 0; i < MaxState; i++)
		{
			if (fsm_state_[i])
			{
				delete fsm_state_[i];
			}
		}
	}

	template <int32 MaxState>
	fsm<MaxState>::fsm(const int32 current_state) : current_state_(current_state)
	{
		memset(fsm_state_, 0, sizeof(fsm_state_));
	}

	template <int32 MaxState>
	void fsm<MaxState>::tick()
	{
		int32 transition_state = fsm_state_[current_state_]->EvaluateStateTransition();
		if (current_state_ != transition_state)
		{
			do
			{
				assert(transition_state >= 0 && transition_state < MaxState);

				fsm_state_[current_state_]->OnExit();
				current_state_ = transition_state;
				fsm_state_[current_state_]->Initialize();
				fsm_state_[current_state_]->OnEnter();
				fsm_state_[current_state_]->OnTick();

				transition_state = fsm_state_[current_state_]->EvaluateStateTransition();
			} while (current_state_ != transition_state);
		}
		else
		{
			fsm_state_[current_state_]->OnTick();
		}
	}

	template <int32 MaxState>
	typename fsm<MaxState>::state* fsm<MaxState>::current()
	{
		return fsm_state_[current_state_];
	}
}
