module;

#include "stdafx.h"
#include "string_helper.h"
#include <ws2tcpip.h>

export module sst.ipv4_address;

export namespace sst
{
	export class ipv4_address
	{
	public:
		ipv4_address();
		ipv4_address(const sockaddr* sa, size_t len);
		explicit ipv4_address(const SOCKADDR_IN& sock_addr);
		ipv4_address(const char* ip, uint16 port);
		ipv4_address(const wchar_t* ip, uint16 port);

		[[nodiscard]] auto get_sock_addr() const -> const sockaddr*;
		[[nodiscard]] auto get_sock_addr_len() const -> size_t;
		[[nodiscard]] auto get_ip_string() const -> std::wstring;
		[[nodiscard]] auto get_port() const -> uint16;

	private:
		void SetAddress(const char* ip, uint16 port);
		
		sockaddr_in sock_addr_{};
	};

	ipv4_address::ipv4_address()
	{
		::memset(&sock_addr_, 0, sizeof(sock_addr_));

		sock_addr_.sin_family = AF_INET;
		sock_addr_.sin_addr.s_addr = ::htonl(INADDR_ANY);
		sock_addr_.sin_port = ::htons(0);
	}

	ipv4_address::ipv4_address(const sockaddr* sa, const size_t len)
	{
		::memset(&sock_addr_, 0, sizeof(sock_addr_));

		if (sa != nullptr &&
			sa->sa_family == AF_INET &&
			len == sizeof(sock_addr_))
		{
			sock_addr_ = *(reinterpret_cast<const sockaddr_in*>(sa));
		}
	}

	ipv4_address::ipv4_address(const SOCKADDR_IN& sock_addr)
	{
		sock_addr_ = sock_addr;
	}

	ipv4_address::ipv4_address(const char* ip, const uint16 port)
	{
		SetAddress(ip, port);
	}

	ipv4_address::ipv4_address(const wchar_t* ip, const uint16 port)
	{
		char aip[INET_ADDRSTRLEN] = { 0, };
		string::helper::wide_to_multi(ip, aip, _countof(aip));
		SetAddress(aip, port);
	}

	const sockaddr* ipv4_address::get_sock_addr() const
	{
		return reinterpret_cast<const sockaddr*>(&sock_addr_);
	}

	size_t ipv4_address::get_sock_addr_len() const
	{
		return sizeof(sock_addr_);
	}

	std::wstring ipv4_address::get_ip_string() const
	{
		char aip[INET_ADDRSTRLEN] = { 0, };
		::inet_ntop(sock_addr_.sin_family, const_cast<in_addr*>(&sock_addr_.sin_addr), aip, sizeof(aip));

		wchar_t wip[INET_ADDRSTRLEN] = { 0, };
		string::helper::multi_to_wide(aip, wip, _countof(wip));
		return wip;
	}

	uint16 ipv4_address::get_port() const
	{
		return ::ntohs(sock_addr_.sin_port);
	}

	void ipv4_address::SetAddress(const char* ip, const uint16 port)
	{
		::memset(&sock_addr_, 0, sizeof(sock_addr_));

		unsigned long ipv4_addr = INADDR_NONE;
		if (::inet_pton(AF_INET, ip, &ipv4_addr) != 0)
		{
			sock_addr_.sin_family = AF_INET;
			sock_addr_.sin_addr.s_addr = ipv4_addr;
			sock_addr_.sin_port = ::htons(port);
		}
		else
		{
			addrinfo hints{};
			::memset(&hints, 0, sizeof(hints));
			hints.ai_flags = AI_PASSIVE;
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;

			char portNum[10] = { 0, };
			::_itoa_s(port, portNum, sizeof(portNum), 10);

			addrinfo* result = nullptr;
			if (::getaddrinfo(ip, portNum, &hints, &result) == 0)
			{
				for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next)
				{
					if (ptr->ai_family == AF_INET)
					{
						sock_addr_ = *(reinterpret_cast<const sockaddr_in*>(ptr->ai_addr));
						break;
					}
				}

				::freeaddrinfo(result);
			}
		}
	}

}