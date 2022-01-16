// Client.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "io_thread.h"
#include "client_session.h"
#include "windows.h"

#include <core/connector.h>
#include <core/vector3d.h>
#include <functional>
#include <chrono>
#include <thread>
#include <source_location>

#include <core/windows.h>
#include <core/trait_util/class_instance_counter.h>
#include <core/logger.h>
#include <core/type_trait_util.h>

import sst;
import sst.network;
import sst.threading.thread_pool;
//import sst.logger;


using namespace sst::network;
using namespace std::chrono_literals;


class TestClass
{
public:
	void* operator new(std::size_t count)
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

	int result_value;
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
	log_debug << "log test!!!! " << SST_VSTR(value);

	[[maybe_unused]] auto p1 = new TestClass;
	[[maybe_unused]] auto p2 = new TestClass;
	[[maybe_unused]] auto p3 = new TestClass;
	[[maybe_unused]] auto p4 = new TestClass;
	[[maybe_unused]] auto p5 = new TestClass;
	[[maybe_unused]] auto p6 = new TestClass2;
	[[maybe_unused]] auto p7 = new TestClass2;
	[[maybe_unused]] auto p8 = new TestClass2;
	[[maybe_unused]] auto p9 = new TestClass2;
	
	log_debug << sst::is_member_object_result_value<TestClass>;
	static_assert(sst::is_member_object_result_value<TestClass>, "");
	if constexpr(sst::is_member_object_result_value<TestClass>)
	{
		log_debug << "has member value result_value";
	}

	const client::windows windows("gui_windows", 8000);
	windows.start();


	sst::win_sock::start_up();
	
	proactor<iocp> proactor;
	auto thread_pool = std::make_shared<sst::threading::thread_pool>(2);
	thread_pool->make_thread<io_thread>(&proactor);
	thread_pool->run();

	client_session session;
	proactor.register_object(&session);

	sst::ipv4_address addr("127.0.0.1", 7777);
	log_debug << L"request connect(" << addr.get_ip_string() << ":" << addr.get_port() << ")";
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

