#pragma once

#include <array>
#include <MSWSock.h>
#include <mswsockdef.h>
#include "io_model.h"


namespace sst::network
{
	class rio
	{
		static RIO_EXTENSION_FUNCTION_TABLE rio_extension_function_table_;
		static std::array<RIO_CQ, 10> rio_completion_queue_;
		
	public:
		static SOCKET alloc_wsa_socket();
		static bool register_function_table(const SOCKET socket)
		{
			GUID function_table_id = WSAID_MULTIPLE_RIO;
			DWORD dw_bytes = 0;
			if (WSAIoctl(socket, SIO_GET_MULTIPLE_EXTENSION_FUNCTION_POINTER, &function_table_id, sizeof(GUID),
			             &rio_extension_function_table_, sizeof(rio_extension_function_table_), &dw_bytes, nullptr, nullptr))
			{
				return false;
			}

			return true;
				
		}
		
	};

	inline SOCKET rio::alloc_wsa_socket()
	{
		const auto socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_REGISTERED_IO);
		if (socket == INVALID_SOCKET)
		{
			// todo log
		}

		return socket;
	}
}
