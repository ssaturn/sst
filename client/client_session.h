#pragma once

#include <thread>

#include "../core/session.h"
#include "../core/sender.h"


class client_session final : public sst::network::session
{
public:
	void on_connected() override
	{
		std::cout << __FILE__ << " connected" << std::endl;

		using namespace  std::chrono_literals;
		
		std::string message = "Hello Server!!";
		get<sst::network::sender>()->send(reinterpret_cast<const byte*>(message.c_str()), message.size());
	}

	size_t on_receive_data(const byte* buf, const size_t length) override
	{
		const std::string str(reinterpret_cast<const char*>(buf), length);
		std::cout << "recv data : " << str << std::endl;

		return length;
	}
};
