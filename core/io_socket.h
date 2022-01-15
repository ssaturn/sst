#pragma once

#include "async_completion_token.h"
#include "common.h"
#include "io_object.h"


namespace sst::network
{
	// network 관련해서 사용하기 위함. 거 무시냐 listener, session에서 사용할 꺼야
	class io_socket : public io_object
	{
	public:
		io_socket();
		~io_socket() override = default;

		[[nodiscard]] auto get_handle() -> HANDLE final { return reinterpret_cast<HANDLE>(get_socket()); }
		[[nodiscard]] auto get_socket() const->SOCKET;
		auto handle_completion(async_completion_token* token, const DWORD bytes_transferred) -> void override;

	protected:
		SOCKET socket_{ INVALID_SOCKET };
	};
	

	inline io_socket::io_socket()
		: socket_(WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED))
	{
	}

	inline auto io_socket::handle_completion(async_completion_token* token, const DWORD bytes_transferred) -> void
	{
		if (nullptr != token)
		{
			token->complete(bytes_transferred);
		}
		delete token;
	}

	inline auto io_socket::get_socket() const -> SOCKET
	{
		return socket_;
	}
}
