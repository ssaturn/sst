#include "stdafx.h"
#include "thread.h"


namespace sst::threading
{
	//////////////////////////////////////////////////////////////////////////
	// How to: Set a Thread Name in Native Code
	// https://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx

	const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
	typedef struct tagTHREADNAME_INFO
	{
		DWORD	dwType;		// Must be 0x1000.
		LPCSTR	szName;		// Pointer to name (in user addr space).
		DWORD	dwThreadID;	// Thread ID (-1=caller thread).
		DWORD	dwFlags;	// Reserved for future use, must be zero.
	} THREADNAME_INFO;
#pragma pack(pop)
	//////////////////////////////////////////////////////////////////////////

	unsigned int __stdcall thread::thread_proc(void* arg)
	{
		//Dump::InstallForthread();

		auto thread = static_cast<sst::threading::thread*>(arg);

		//////////////////////////////////////////////////////////////////////////
		// How to: Set a thread Name in Native Code
		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = thread->name_;
		info.dwThreadID = static_cast<DWORD>(-1);
		info.dwFlags = 0;

		__try
		{
			::RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), reinterpret_cast<ULONG_PTR*>(&info));
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
		//////////////////////////////////////////////////////////////////////////

		if (thread->setup())
		{
			thread->run();
		}

		thread->cleanup();

		return 0;
	}

	thread::thread(const std::string_view name, const thread_id_t thread_id)
		: id_(thread_id)
	{
		handle_ = reinterpret_cast<HANDLE>(::_beginthreadex(nullptr, 0, &thread_proc, this, CREATE_SUSPENDED, &os_id_));
		::sprintf_s(name_, _countof(name_), "%s(%d)", (name. != nullptr) ? name : "Thread", static_cast<int32>(id_));
	}

	thread::~thread()
	{
		if (handle_ != nullptr)
		{
			::CloseHandle(handle_);
			handle_ = nullptr;
		}
	}

	void thread::start() const
	{
		::ResumeThread(handle_);
	}

}
