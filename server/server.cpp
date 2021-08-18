// SProject.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <core/session.h>
#include "../core/sender.h"
#include "../core/thread.h"
#include "../core/listener.h"
#include "../core/fsm.h"
#include <chrono>
#include <thread>


using namespace std::chrono_literals;

import sst;
import sst.network;
import sst.threading;


class user_session final : public sst::network::session
{
public:
	void on_connected() override
	{
		std::cout << "connect finish" << std::endl;
		
		const std::string message = "Hello Server!!";
		get<sst::network::sender>()->send(reinterpret_cast<const byte*>(message.c_str()), message.size());
	}
	
	size_t on_receive_data(const byte* buf, const size_t length) override
	{
		const std::string str(reinterpret_cast<const char*>(buf), length);
		std::cout << "recv data : " << str << std::endl;

		return length;
	}
};

class io_thread final : public sst::threading::thread
{
public:
	explicit io_thread(class sst::network::proactor* proactor)
		: thread("io_thread", 0)
		, proactor_(proactor)
	{

	}

	bool setup() override { return true; }
	void run() override
	{
		while(true)
		{
			proactor_->proc();
		}
	}
	void cleanup() override {}

private:
	sst::network::proactor* proactor_{ nullptr };
};

class user_idle_state : public sst::fsm<5>::state
{
protected:
	void OnEnter() override {}
	void OnTick() override {}
	void OnExit() override {}
	int32 EvaluateStateTransition() override { return 0; }
};

class User : public sst::fsm<5>
{
public:
	User()
	{
		register_state<user_idle_state>(0);
	}
	
};



int main()
{
	sst::win_sock::start_up();

	sst::network::proactor proactor;
	sst::network::listener<user_session> listener(&proactor, 7777, 0);
	sst::threading::thread_pool thread_pool(64);
	
	thread_pool.make_thread<io_thread>(&proactor);
	thread_pool.run();
	listener.listen();
	std::cout << "server is ready....." << std::endl;
	
	while (true)
	{
		if (const auto r = getchar(); r == 'q')
		{
			break;
		}

		std::this_thread::sleep_for(1ms);
	}

	return 0;
}


