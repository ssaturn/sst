#pragma once
#include <WinSock2.h>


namespace sst::network
{
	class io_model
	{
	public:
		virtual SOCKET alloc_listen_socket() = 0;
	};
}
