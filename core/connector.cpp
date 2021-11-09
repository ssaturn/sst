#include "stdafx.h"
#include "connector.h"
#include "async_completion_token.h"
#include "prereceiver.h"
#include "session.h"
#include <MSWSock.h>


namespace sst::network
{
	connector::connector(actor_owner* owner)
		: actor(owner)
	{
	}

	void connector::proc(const ipv4_address& addr)
	{
		const auto socket = get_owner<session>()->get_socket();
		BOOL opt_val = TRUE;
		if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&opt_val), sizeof(BOOL)) == SOCKET_ERROR)
		{
			std::cout << "error : bind" << std::endl;
			return;
		}

		SOCKADDR_IN address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port = htons(0);

		if (::bind(socket, reinterpret_cast<SOCKADDR*>(&address), sizeof(address)) == SOCKET_ERROR)
		{
			std::cout << "error : bind" << std::endl;
			return;
		}

		if (win_sock::connect_ex(socket, addr.get_sock_addr(), sizeof(SOCKADDR_IN), nullptr,
								 0, nullptr, new async_completion_token(this)) == FALSE)
		{
			if (const int32 error_code = WSAGetLastError(); WSA_IO_PENDING != error_code)
			{
				std::cout << "error : " << error_code << std::endl;
			}
		}
	}


	void connector::complete([[maybe_unused]] async_completion_token* token, [[maybe_unused]] DWORD bytes_transferred)
	{
		if (setsockopt(get_owner<session>()->get_socket(), SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0) == SOCKET_ERROR)
		{
			[[maybe_unused]] DWORD err = WSAGetLastError();
			return;
		}
		
		get_owner<session>()->get<prereceiver>()->proc();
		get_owner<session>()->on_connected();
	}

	void connector::error([[maybe_unused]] async_completion_token* token, [[maybe_unused]] DWORD error)
	{
	}
}
