#pragma once

#include "io_socket.h"

import sst;

namespace sst::network
{
	class session : public io_socket, public actor_owner
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(session)
	
	public:
		session();
		~session() override = default;

		void initialize();

		/**
		 * \brief when complete accept, call this method
		 */
		void set_peer_name();
		void set_connect(const bool connect) { is_connected_ = connect; }

		virtual void on_connected() = 0;
		virtual size_t on_receive_data(const byte* buf, size_t length) = 0;

	protected:
		bool is_connected_{ false };
		ipv4_address peer_addr_{};
	};
}
