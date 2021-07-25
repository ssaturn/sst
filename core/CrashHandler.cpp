#include "stdafx.h"
#include "CrashHandler.h"
#include <vector>
#include <signal.h>
#include <DbgHelp.h>
#include <Psapi.h>
#include <TlHelp32.h>

#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "Psapi.lib")

namespace
{
	crash_handler::PostJobForDump*	g_postJobForDump = nullptr;
	std::vector<DWORD>		g_threadFilter;

	class ThreadSuspender
	{
	public:
		explicit ThreadSuspender( const std::vector<DWORD>& filter )
		{
			suspend( filter );
		}

		~ThreadSuspender()
		{
			resume();
		}

	private:
		std::vector<HANDLE>	m_threadHandles;

		void suspend( const std::vector<DWORD>& filter )
		{
			DWORD thisThreadId = GetCurrentThreadId();
			HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 );

			THREADENTRY32 threadEntry32;
			threadEntry32.dwSize = sizeof( threadEntry32 );
			if ( Thread32First( snapshot, &threadEntry32 ) )
			{
				do
				{
					// �� ������ ���ư��� ������� ������ �ʴ´�.
					if ( threadEntry32.th32ThreadID == thisThreadId )
						continue;

					if ( isExist( filter, threadEntry32.th32ThreadID ) == true )
						continue;

					HANDLE threadHandle = ::OpenThread( THREAD_ALL_ACCESS, FALSE, threadEntry32.th32ThreadID );
					if ( threadHandle == NULL )
						continue;

					if ( ::SuspendThread( threadHandle ) == (DWORD)-1 )
						continue;

					m_threadHandles.push_back( threadHandle );

				} while ( Thread32Next( snapshot, &threadEntry32 ) );
			}

			::CloseHandle( snapshot );
		}

		void resume()
		{
			for ( auto& thread : m_threadHandles )
			{
				if ( thread != NULL )
				{
					::ResumeThread( thread );
					::CloseHandle( thread );
				}
			}

			m_threadHandles.clear();
		}

		bool isExist( const std::vector<DWORD>& filter, DWORD threadId )
		{
			return std::find( filter.begin(), filter.end(), threadId ) != filter.end();
		}

	};
}

void CreateMiniDump( EXCEPTION_POINTERS* exceptionInfo, const wchar_t* dumpFileName, MINIDUMP_TYPE dumpType )
{
	HANDLE dumpFile = CreateFileW( dumpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( dumpFile != INVALID_HANDLE_VALUE )
	{
		HANDLE process = GetCurrentProcess();
		DWORD processId = GetCurrentProcessId();
		DWORD threadId = GetCurrentThreadId();

		MINIDUMP_EXCEPTION_INFORMATION miniDumpInfo;
		::memset( &miniDumpInfo, 0, sizeof( miniDumpInfo ) );
		miniDumpInfo.ThreadId = threadId;
		miniDumpInfo.ExceptionPointers = exceptionInfo; 
		miniDumpInfo.ClientPointers = FALSE;

		MiniDumpWriteDump( process, processId, dumpFile, dumpType, exceptionInfo != NULL ? &miniDumpInfo : NULL, NULL, NULL );

		::FlushFileBuffers( dumpFile );
		::CloseHandle( dumpFile );
	}
}

LONG WINAPI ExceptionFilterFunction( EXCEPTION_POINTERS* exception )
{
	static long	isRuningExceptionFilter = 0;
	if ( InterlockedIncrement( &isRuningExceptionFilter ) != 1 )
	{
		// �ϴ� ������ �ϳ��� ����.
		return EXCEPTION_EXECUTE_HANDLER;
	}

	ThreadSuspender threadSuspender( g_threadFilter );

	if ( ::IsDebuggerPresent() == FALSE )
	{
		wchar_t processName[MAX_PATH] = { 0, };
		wchar_t path[MAX_PATH] = { 0, };
		if ( ::GetModuleFileNameExW( GetCurrentProcess(), NULL, path, _countof( path ) ) == 0 )
		{
			::wcscpy_s( processName, _countof( processName ), L"Unknown" );
		}
		else
		{
			::_wsplitpath_s( path, NULL, 0, NULL, 0, processName, _countof( processName ), NULL, 0 );
		}

		SYSTEMTIME time;
		::GetLocalTime( &time );

		wchar_t dumpFileName[MAX_PATH] = { 0, };
		swprintf_s( dumpFileName, L"./%s.%04d-%02d-%02d_%02d_%02d_%02d.dmp",
			processName,
			time.wYear, time.wMonth, time.wDay,
			time.wHour, time.wMinute, time.wSecond );

		CreateMiniDump( exception, dumpFileName, MiniDumpWithFullMemory );
	}

	if ( g_postJobForDump != nullptr )
	{
		g_postJobForDump->WaitFor();
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

void PureCallHandler()
{
	crash_handler::Crash();
}

void InvalidParameterHandler( const wchar_t * expression, const wchar_t * function, const wchar_t * file, unsigned int line, uintptr_t reserved )
{
	(expression);
	(function);
	(file);
	(line);
	(reserved);
	crash_handler::Crash();
}

void SignalHandler( int signum )
{
	switch ( signum )
	{
	case SIGABRT:	// ���������� ���� ����, ���� ��� 
	case SIGFPE:	// 0���� ������ ���� �߸� �� ��� ����
	case SIGILL:	// �߸� �� ��� ���� �߸� �� ���α׷� �̹���
	case SIGSEGV:	// �߸� �� �޸� �׼���(���� ����)
	case SIGTERM:	// ���α׷� ���� ���� ��û
		crash_handler::Crash();
		break;
	}
}

void SetSignalHanlders()
{
	signal( SIGABRT, SignalHandler );
	signal( SIGFPE, SignalHandler );
	signal( SIGILL, SignalHandler );
	signal( SIGSEGV, SignalHandler );
	signal( SIGTERM, SignalHandler );
}

void StdTerminateHandlerOfThisThread()
{
	crash_handler::Crash();
}

void StdUnexpectedHandlerOfThisThread()
{
	crash_handler::Crash();
}

void crash_handler::Install()
{
	// ó�� �ȵ� ����
	::SetUnhandledExceptionFilter( ExceptionFilterFunction );
	// ���������Լ� ��
	::_set_purecall_handler( PureCallHandler );
	// ���ڰ� ��ȿ�� üĿ
	::_set_invalid_parameter_handler( InvalidParameterHandler );
	// ��ȣó���� ���ؼ� ���
	SetSignalHanlders();

	::_CrtSetReportMode( _CRT_ASSERT, 0 );
}

void crash_handler::InstallForThread()
{
	std::set_terminate( StdTerminateHandlerOfThisThread );
	set_unexpected( StdUnexpectedHandlerOfThisThread );
}

void crash_handler::SetPostJobForDump( PostJobForDump* postJob )
{
	g_postJobForDump = postJob;
}

void crash_handler::AddExcludeSuspendThread( DWORD threadId )
{
	g_threadFilter.push_back( threadId );
}
