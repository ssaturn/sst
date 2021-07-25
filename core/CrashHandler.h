#pragma once


__declspec(thread) extern void*	LDebugHintThisPtrOfThisThread;

namespace crash_handler
{
	class PostJobForDump
	{
	public:
		virtual void WaitFor() = 0;
	};

	void Install();
	void InstallForThread();

	void SetPostJobForDump( PostJobForDump* postJob );
	void AddExcludeSuspendThread( DWORD threadId );

	inline void	Crash()
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
			CrashHandler::Crash(); \
		} \
	}

