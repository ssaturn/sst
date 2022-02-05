#pragma once

#include "actor.h"


namespace sst::network
{
	class acceptor final : public actor
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(acceptor)
	
	public:
		static constexpr type type_value = type::acceptor;
		
		explicit acceptor(session* owner);
		acceptor() = delete;
		~acceptor() override = default;

		void set_listen_socket(SOCKET socket);
		void accept(SOCKET socket);
		void complete(struct async_completion_token* token, DWORD bytes_transferred) override;
		void error(struct async_completion_token* token, DWORD error) override;

	private:
		SOCKET listen_socket_{ INVALID_SOCKET };
	};
}
