#pragma once

#include "logger.h"
#include "io_socket.h"
#include "packet_header.h"
//#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include "sender.h"
//#include <shared/packet/protocol.pb.h>
//#include <shared/packet/protocol_generated.h>
//#include <shared/packet/packet_generated.h>

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

		/*template<protocol::id ProtocolId, typename PacketTy>
		bool send(PacketTy* packet);*/


	protected:
		bool is_connected_{ false };
		ipv4_address peer_addr_{};
	};

	//template <protocol::id ProtocolId, typename PacketTy>
	//bool session::send(PacketTy* packet)
	//{
		//auto name = fbBuilder.get().CreateString(serverRegister->colonyName()->c_str());
		//packet::Createrequest_login()
		//auto response = CreateAgentServerRegisterResponse(fbBuilder.get(), ErrorCode::OK, serverId, name);
		//fbBuilder.get().Finish(response);

	//}

	/*template <share::protocol ProtocolId, typename PacketTy>
	bool session::send(PacketTy* packet)
	{
		;
		const int buf_size = packet->ByteSize();
		const size_t packet_size = buf_size + sizeof(sst::packet_header);

		try
		{
			byte* output_buffer = new char[packet_size];
			const sst::packet_header header{
				.size = buf_size,
				.type = static_cast<uint16>(ProtocolId)
			};
			memcpy_s(output_buffer, sizeof(sst::packet_header), &header, sizeof(sst::packet_header));

			google::protobuf::io::ArrayOutputStream os(output_buffer + sizeof(sst::packet_header), buf_size);
			packet->SerializeToZeroCopyStream(&os);
			get<sst::network::sender>()->proc(output_buffer, packet_size);
		}
		catch (std::exception& e)
		{
			log_debug << e.what();
			return false;
		}

		return true;
	}*/
}
