module;

//#include "stdafx.h"
#include "common.h"
#include <MSWSock.h>


export module sst.win_sock;


namespace sst::win_sock
{
	export extern BOOL disconnect_ex(SOCKET socket, LPOVERLAPPED overlapped, DWORD flags, DWORD reserved)
	{
		static LPFN_DISCONNECTEX lpfn_disconnect_ex = nullptr;

		if (lpfn_disconnect_ex == nullptr)
		{
			const SOCKET garbage_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			GUID guid = WSAID_DISCONNECTEX;
			DWORD bytes = 0;

			if (SOCKET_ERROR == WSAIoctl(garbage_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid),
										 &lpfn_disconnect_ex, sizeof(lpfn_disconnect_ex), &bytes, nullptr, nullptr))
			{
				/*GConsolePrinter->OutStdErr(RED, L"Fatal Error! : cannot proceed on WASIoctl. %d\n", WSAGetLastError());
				_ASSERTCRASH(false);*/
				return FALSE;
			}

			closesocket(garbage_socket);
		}

		return lpfn_disconnect_ex(socket, overlapped, flags, reserved);
	}

	export extern BOOL connect_ex(SOCKET h_socket, const struct sockaddr* name, int name_len,
								  PVOID send_buffer, DWORD send_data_length, LPDWORD bytes_sent, LPOVERLAPPED overlapped)
	{
		static LPFN_CONNECTEX lpfn_connect_ex = nullptr;

		if (lpfn_connect_ex == nullptr)
		{
			const SOCKET garbage_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			GUID guid = WSAID_CONNECTEX;
			DWORD bytes = 0;

			if (SOCKET_ERROR == WSAIoctl(garbage_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid),
										 &lpfn_connect_ex, sizeof(lpfn_connect_ex), &bytes, nullptr, nullptr))
			{
				/*GConsolePrinter->OutStdErr(RED, L"Fatal Error! : cannot proceed on WASIoctl. %d\n", WSAGetLastError());
				_ASSERTCRASH(false);*/
				return FALSE;
			}

			closesocket(garbage_socket);
		}

		return lpfn_connect_ex(h_socket, name, name_len, send_buffer, send_data_length, bytes_sent, overlapped);
	}

	export inline bool start_up()
	{
		const WORD version_requested = MAKEWORD(2, 2);
		WSADATA wsa_data;

		if (const int error_status = WSAStartup(version_requested, &wsa_data); error_status != 0)
		{
			return false;
		}

		if ((LOBYTE(wsa_data.wVersion) != LOBYTE(version_requested)) ||
			(HIBYTE(wsa_data.wVersion) != HIBYTE(version_requested)))
		{
			return false;
		}

		disconnect_ex(0, nullptr, 0, 0);
		connect_ex(0, nullptr, 0, nullptr, 0, nullptr, nullptr);

		return true;
	}


}