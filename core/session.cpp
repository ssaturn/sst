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
	session::session()
		: io_socket()
	{
		register_actor<sender>(this);
		register_actor<receiver>(this);
		register_actor<prereceiver>(this);
		register_actor<disconnector>(this);
		register_actor<connector>(this);
		register_actor<acceptor>(this);
	}

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
