#include "stdafx.h"
#include "session.h"
#include "acceptor.h"
#include "connector.h"
#include "disconnector.h"
#include "prereceiver.h"
#include "receiver.h"
#include "sender.h"


namespace sst::network
{


	void session::initialize()
	{
	}

	void session::set_peer_name()
	{
		SOCKADDR_IN peer_addr{};
		int32 peer_addr_len{};
		if (getpeername(get_socket(), reinterpret_cast<SOCKADDR*>(&peer_addr), &peer_addr_len) == SOCKET_ERROR)
		{
			DWORD err = WSAGetLastError();
			err;
			return;
		}

		peer_addr_ = ipv4_address(peer_addr);
	}
}
