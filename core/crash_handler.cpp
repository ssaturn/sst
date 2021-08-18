#include "stdafx.h"
#include "crash_handler.h"
#include <vector>
#include <signal.h>
#include <DbgHelp.h>
#include <Psapi.h>
#include <TlHelp32.h>

#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "Psapi.lib")

namespace
{
	crash_handler::post_job_for_dump*	g_postJobForDump = nullptr;
	std::vector<DWORD>		g_threadFilter;

	class thread_suspender
	{
	public:
		explicit thread_suspender( const std::vector<DWORD>& filter )
		{
			Suspend( filter );
		}

		~thread_suspender()
		{
			Resume();
		}

	private:
		std::vector<HANDLE>	thread_handles_;

		void Suspend( const std::vector<DWORD>& filter )
		{
			const DWORD this_thread_id = GetCurrentThreadId();
			const HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 );

			THREADENTRY32 thread_entry32;
			thread_entry32.dwSize = sizeof( thread_entry32 );
			if ( Thread32First( snapshot, &thread_entry32 ) )
			{
				do
				{
					// 이 로직이 돌아가는 스레드는 멈추지 않는다.
					if ( thread_entry32.th32ThreadID == this_thread_id )
						continue;

					if ( IsExist( filter, thread_entry32.th32ThreadID ) == true )
						continue;

					HANDLE thread_handle = ::OpenThread( THREAD_ALL_ACCESS, FALSE, thread_entry32.th32ThreadID );
					if ( thread_handle == NULL )
						continue;

					if ( ::SuspendThread( thread_handle ) == (DWORD)-1 )
						continue;

					thread_handles_.push_back( thread_handle );

				} while ( Thread32Next( snapshot, &thread_entry32 ) );
			}

			::CloseHandle( snapshot );
		}

		void Resume()
		{
			for ( auto& thread : thread_handles_ )
			{
				if ( thread != NULL )
				{
					::ResumeThread( thread );
					::CloseHandle( thread );
				}
			}

			thread_handles_.clear();
		}

		bool IsExist( const std::vector<DWORD>& filter, DWORD threadId ) const
		{
			return std::find( filter.begin(), filter.end(), threadId ) != filter.end();
		}

	};
}

void create_mini_dump( EXCEPTION_POINTERS* exception_info, const wchar_t* dump_file_name, MINIDUMP_TYPE dump_type )
{
	HANDLE dump_file = CreateFileW( dump_file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( dump_file != INVALID_HANDLE_VALUE )
	{
		const HANDLE process = GetCurrentProcess();
		const DWORD process_id = GetCurrentProcessId();
		const DWORD thread_id = GetCurrentThreadId();

		MINIDUMP_EXCEPTION_INFORMATION mini_dump_info;
		::memset( &mini_dump_info, 0, sizeof( mini_dump_info ) );
		mini_dump_info.ThreadId = thread_id;
		mini_dump_info.ExceptionPointers = exception_info; 
		mini_dump_info.ClientPointers = FALSE;

		MiniDumpWriteDump( process, process_id, dump_file, dump_type, exception_info != NULL ? &mini_dump_info : NULL, NULL, NULL );

		::FlushFileBuffers( dump_file );
		::CloseHandle( dump_file );
	}
}

LONG WINAPI exception_filter_function( EXCEPTION_POINTERS* exception )
{
	static long	is_running_exception_filter = 0;
	if ( InterlockedIncrement( &is_running_exception_filter ) != 1 )
	{
		return EXCEPTION_EXECUTE_HANDLER;
	}

	thread_suspender thread_suspender( g_threadFilter );

	if ( ::IsDebuggerPresent() == FALSE )
	{
		wchar_t process_name[MAX_PATH] = { 0, };
		wchar_t path[MAX_PATH] = { 0, };
		if ( ::GetModuleFileNameExW( GetCurrentProcess(), NULL, path, _countof( path ) ) == 0 )
		{
			::wcscpy_s( process_name, _countof( process_name ), L"Unknown" );
		}
		else
		{
			::_wsplitpath_s( path, NULL, 0, NULL, 0, process_name, _countof( process_name ), NULL, 0 );
		}

		SYSTEMTIME time;
		::GetLocalTime( &time );

		wchar_t dump_file_name[MAX_PATH] = { 0, };
		swprintf_s( dump_file_name, L"./%s.%04d-%02d-%02d_%02d_%02d_%02d.dmp",
			process_name,
			time.wYear, time.wMonth, time.wDay,
			time.wHour, time.wMinute, time.wSecond );

		create_mini_dump( exception, dump_file_name, MiniDumpWithFullMemory );
	}

	if ( g_postJobForDump != nullptr )
	{
		g_postJobForDump->wait_for();
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

void pure_call_handler()
{
	crash_handler::crash();
}

void invalid_parameter_handler( const wchar_t * expression, const wchar_t * function, const wchar_t * file, unsigned int line, uintptr_t reserved )
{
	(expression);
	(function);
	(file);
	(line);
	(reserved);
	crash_handler::crash();
}

void signal_handler( int signum )
{
	switch ( signum )
	{
	case SIGABRT:	// 비정상적인 종료 조건, 예를 들어 
	case SIGFPE:	// 0으로 나누기 같은 잘못 된 산술 연산
	case SIGILL:	// 잘못 된 명령 같이 잘못 된 프로그램 이미지
	case SIGSEGV:	// 잘못 된 메모리 액세스(분할 오류)
	case SIGTERM:	// 프로그램 전송 종료 요청
		crash_handler::crash();
		break;
	}
}

void set_signal_handlers()
{
	signal( SIGABRT, signal_handler );
	signal( SIGFPE, signal_handler );
	signal( SIGILL, signal_handler );
	signal( SIGSEGV, signal_handler );
	signal( SIGTERM, signal_handler );
}

void StdTerminateHandlerOfThisThread()
{
	crash_handler::crash();
}

void StdUnexpectedHandlerOfThisThread()
{
	crash_handler::crash();
}

void crash_handler::install()
{
	// 처리 안된 예외
	::SetUnhandledExceptionFilter( exception_filter_function );
	// 순수가상함수 콜
	::_set_purecall_handler( pure_call_handler );
	// 인자값 유효성 체커
	::_set_invalid_parameter_handler( invalid_parameter_handler );
	// 신호처리를 위해서 사용
	set_signal_handlers();

	::_CrtSetReportMode( _CRT_ASSERT, 0 );
}

void crash_handler::install_for_thread()
{
	std::set_terminate( StdTerminateHandlerOfThisThread );
	set_unexpected( StdUnexpectedHandlerOfThisThread );
}

void crash_handler::set_post_job_for_dump( post_job_for_dump* postJob )
{
	g_postJobForDump = postJob;
}

void crash_handler::add_exclude_suspend_thread( DWORD threadId )
{
	g_threadFilter.push_back( threadId );
}
