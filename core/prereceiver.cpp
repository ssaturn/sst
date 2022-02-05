#include "stdafx.h"
#include "prereceiver.h"
#include "session.h"
#include "proactor.h"
#include <MSWSock.h>

#include "receiver.h"


namespace sst::network
{
	prereceiver::prereceiver(session* owner)
		: actor(owner)
	{
	}

	void prereceiver::proc()
	{
		const auto token = new async_completion_token(this);

		DWORD recv_bytes = 0;
		DWORD flags = 0;
		token->wsabuf.buf = new char[65535];
		token->wsabuf.len = 0;
		if (SOCKET_ERROR == WSARecv(get_owner()->get_socket(), &(token->wsabuf), 1, &recv_bytes, &flags, token, nullptr))
		{
			if (const size_t error_code = WSAGetLastError(); error_code != WSA_IO_PENDING)
			{
				//ErrorHandler(errorCode);
				delete token;
				return;
			}
		}
		std::cout << "pre_receive!!" << std::endl;

		
	}

	void prereceiver::complete([[maybe_unused]] async_completion_token* token, [[maybe_unused]] DWORD bytes_transferred)
	{
		get_owner()->get<receiver>()->proc();
		std::cout << "prereceiver::complete" << std::endl;
	}

	void prereceiver::error([[maybe_unused]] async_completion_token* token, [[maybe_unused]] DWORD error)
	{
		std::cout << "prereceiver::error" << std::endl;
	}
}
