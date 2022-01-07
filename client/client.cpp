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
#include <core/class_instance_counter.h>

import sst;
import sst.network;
import sst.threading.thread_pool;
//import sst.logger;


using namespace sst::network;
using namespace std::chrono_literals;


class TestClass
{
public:
	void* operator new(const std::size_t count)
	{
		sst::class_instance_counter::infos[sst::class_indexer<TestClass>::index].count.fetch_add(1);
		return ::operator new(count);
	}
	// custom placement delete
	void operator delete(void* ptr)
	{
		sst::class_instance_counter::infos[sst::class_indexer<TestClass>::index].count.fetch_sub(1);
		::operator delete(ptr);
	}
};


class TestClass2
{
public:
	void* operator new(const std::size_t count)
	{
		sst::class_instance_counter::infos[sst::class_indexer<TestClass2>::index].count.fetch_add(1);
		return ::operator new(count);
	}
	// custom placement delete
	void operator delete(void* ptr)
	{
		sst::class_instance_counter::infos[sst::class_indexer<TestClass2>::index].count.fetch_sub(1);
		::operator delete(ptr);
	}
};



int main()
{
	int value = 0;
	log_debug << "log test!!!!" << SST_VSTR(value);

	[[maybe_unused]] auto p1 = new TestClass();
	[[maybe_unused]] auto p2 = new TestClass();
	[[maybe_unused]] auto p3 = new TestClass();
	[[maybe_unused]] auto p4 = new TestClass();
	[[maybe_unused]] auto p5 = new TestClass();
	[[maybe_unused]] auto p7 = new TestClass2();
	delete p1;
	delete p2;
	delete p3;
	delete p4;





	
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

