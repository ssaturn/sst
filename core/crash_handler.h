#pragma once


__declspec(thread) extern void*	l_debug_hint_this_ptr_of_this_thread;

namespace crash_handler
{
	class post_job_for_dump
	{
	public:
		virtual void wait_for() = 0;
	};

	void install();
	void install_for_thread();

	void set_post_job_for_dump( post_job_for_dump* post_job );
	void add_exclude_suspend_thread( DWORD thread_id );

	inline void	crash()
	{
		int* temp = nullptr;
		*temp = 0x44560349;
	}
}

#define ASSERT_CRASH(expr) \
	{ \
		if ( !(expr) ) \
		{ \
			__if_exists (this) \
			{ \
				LDebugHintThisPtrOfThisThread = (void*) this; \
			} \
			crash_handler::crash(); \
		} \
	}

