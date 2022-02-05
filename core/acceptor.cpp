#include "stdafx.h"
#include "acceptor.h"
#include "prereceiver.h"
#include "session.h"
#include <MSWSock.h>


namespace sst::network
{
	acceptor::acceptor(session* owner)
		: actor(owner)
	{
	}

	void acceptor::complete([[maybe_unused]] async_completion_token* token, [[maybe_unused]] DWORD bytes_transferred)
	{
		if (setsockopt(get_owner()->get_socket(), SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, reinterpret_cast<char*>(&listen_socket_), sizeof(SOCKET)) == SOCKET_ERROR)
		{
			[[maybe_unused]] DWORD err = WSAGetLastError();
			accept(listen_socket_);
			return;
		}
		
		std::cout << "accept complete!!" << std::endl;
		
		auto owner = get_owner();
		owner->set_peer_name();
		owner->set_connect(true);
		if (auto actor = owner->get<prereceiver>(); actor)
		{
			actor->proc();
		}

		owner->on_connected();
	}

	void acceptor::error([[maybe_unused]] async_completion_token* token, [[maybe_unused]] DWORD error)
	{
	}


	void acceptor::accept(const SOCKET socket)
	{
		listen_socket_ = socket;
		const auto owner = get_owner();
		DWORD bytes_received = 0;
		const auto token = new async_completion_token(this);

		do
		{
			// fix it prepare buffer
			//sizeof(SOCKADDR_IN) + 16;
			byte* buffer = new byte[128];
			if (::AcceptEx(listen_socket_, owner->get_socket(), buffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
						   &bytes_received, token) == FALSE)
			{
				switch (const auto error_code = WSAGetLastError(); error_code)
				{
				case WSA_IO_PENDING:
					break;
				case WSAEINVAL:
					{
						/// 다시 세션을 세팅하고 강제로 넣고
						//mServiceCoordinator->ResetSession(session);

						//DecreaseSessionCount();

						/// 일단 아래처럼 세션 날리게
						delete token;

						//GConsolePrinter->OutStdErr(WHITE, L"Process 10022 error. :)\n");
					}
					break;
				default:
					{
						[[maybe_unused]] auto error = WSAGetLastError();
						
						delete token;
					}
					break;
				}
			}
		} while (false);
	}
}
