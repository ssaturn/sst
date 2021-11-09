#pragma once

namespace sst::network
{
	class iocp
	{
	public:
		static SOCKET alloc_wsa_socket();
	};

	inline SOCKET iocp::alloc_wsa_socket()
	{
		return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	}
}
