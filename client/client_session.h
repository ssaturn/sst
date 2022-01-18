#pragma once

#include <thread>
#include <core/logger.h>
#include <core/session.h>
#include <core/sender.h>
#include <core/packet_header.h>
#include <core/memory/object_counter.h>
#include <shared/packet/packet_generated.h>


class client_session final : public sst::network::session, sst::memory::object_counter<client_session>
{
public:
	void on_connected() override
	{
		log_debug << " connected";

		using namespace  std::chrono_literals;
		
		// protobuf
		/*share::request_login req_login;
		req_login.set_user_id("2323123");
		req_login.set_user_pwd("1234");
		send<share::protocol::req_login>(&req_login);*/

		flatbuffers::FlatBufferBuilder fbb;
		packet::Createrequest_loginDirect(fbb, 1, "UserName", "UserPassword");
		send<protocol::LOGIN_REQUEST>(fbb);
	}

	size_t on_receive_data(const byte* buf, const size_t length) override
	{
		/*const std::string str(reinterpret_cast<const char*>(buf), length);
		std::cout << "recv data : " << str << std::endl;*/
		sst::packet_header header;
		memcpy_s(&header, sizeof(header), buf, sizeof(header));

		//if (header.type == share::protocol::)

		return length;
	}
};
