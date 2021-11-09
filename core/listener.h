#pragma once

#include "acceptor.h"
#include "io_socket.h"
#include "session_pool.h"
#include "proactor.h"


namespace sst::network
{
	template<typename IoModel, typename SessionTy>
	class listener final : public io_socket
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(listener)

	public:
		explicit listener(proactor<IoModel>* proactor, uint16 port, const uint32 backlog);
		~listener() override = default;

		void listen();
		auto handle_completion(async_completion_token* token, DWORD bytes) -> void override;

	private:
		SOCKADDR_IN addr_{};
		uint32 backlog_{ 0 };
		session_pool<SessionTy, 100> session_pool_{};
		proactor<IoModel>* proactor_{ nullptr };
	};

	
	template<typename IoModel, typename SessionTy>
	listener<IoModel, SessionTy>::listener(proactor<IoModel>* proactor, const uint16 port, const uint32 backlog)
		: proactor_(proactor)
	{
		socket_ = IoModel::alloc_socket();

		addr_.sin_family = AF_INET;
		addr_.sin_addr.s_addr = htonl(INADDR_ANY);
		addr_.sin_port = htons(port);

		backlog_ = backlog;
	}

	template<typename IoModel, typename SessionTy>
	void listener<IoModel, SessionTy>::listen()
	{
		proactor_->register_object(this);
		
		BOOL opt_val = TRUE;
		const size_t opt_len = sizeof(BOOL);
		ipv4_address addr(addr_);

		if (::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&opt_val), opt_len) == SOCKET_ERROR)
		{
			return;
		}

		if (::bind(socket_, addr.get_sock_addr(), sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
		{
			return;
		}

		if constexpr (std::is_same_v<IoModel, class rio>)
		{
			//if (rio::register_function_table() == false)
			//{
			//	// error log
			//	return;
			//}
		}

		if (::listen(socket_, SOMAXCONN) == SOCKET_ERROR)
		{
			return;
		}

		const auto pre_accept_count = 10;
		for (size_t i = 0; i < pre_accept_count; ++i)
		{
			if (auto session = session_pool_.acquire(); session)
			{
				session->get<acceptor>()->accept(socket_);
			}
		}
	}

	template<typename IoModel, typename SessionTy>
	auto listener<IoModel, SessionTy>::handle_completion(async_completion_token* token, const DWORD bytes) -> void
	{
		proactor_->register_object(token->actor->get_owner<session>());
		token->actor->complete(token, bytes);
	}
}
