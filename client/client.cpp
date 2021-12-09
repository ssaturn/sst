// Client.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "io_thread.h"
#include "client_session.h"
#include <core/connector.h>
#include <core/vector3d.h>
#include <functional>
#include <chrono>
#include <thread>
#include <source_location>
#include <core/logger.h>

import sst;
import sst.network;
import sst.threading;
//import sst.logger;

using namespace sst::network;
using namespace std::chrono_literals;

int main()
{
	int value = 0;
	sst::log_debug() << "log test!!!!" << SST_VSTR(value);
	
	sst::win_sock::start_up();
	
	proactor<iocp> proactor;
	auto thread_pool = std::make_shared<sst::threading::thread_pool>(2);
	thread_pool->make_thread<io_thread>(&proactor);
	thread_pool->run();
	
	client_session session;
	proactor.register_object(&session);

	sst::ipv4_address addr("127.0.0.1", 7777);
	session.get<connector>()->proc(addr);

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

