// Client.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "io_thread.h"
#include "client_session.h"
//#include "windows.h"
#include <core/windows.h>

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


template<typename T>
class object_counter
{
protected:
	object_counter()
	{
		sst::class_instance_counter::infos[sst::class_indexer<T>::index].count.fetch_add(1);
	}

	virtual ~object_counter()
	{
		sst::class_instance_counter::infos[sst::class_indexer<T>::index].count.fetch_sub(1);
	}
};

class TestClass : object_counter<TestClass>
{
public:
	void* operator new(std::size_t count)
	{
		//sst::class_instance_counter::infos[sst::class_indexer<TestClass>::index].count.fetch_add(1);
		return ::operator new(count);
	}
	// custom placement delete
	void operator delete(void* ptr)
	{
		//sst::class_instance_counter::infos[sst::class_indexer<TestClass>::index].count.fetch_sub(1);
		::operator delete(ptr);
	}

	int result_value;
};


class TestClass2 : object_counter<TestClass2>
{
public:
	void* operator new(const std::size_t count)
	{
		return ::operator new(count);
	}

	void operator delete(void* ptr)
	{
		::operator delete(ptr);
	}
};


class options_description;

class options_description_easy_init
{
public:
	explicit options_description_easy_init(options_description* owner)
		: owner_(owner)
	{
		
	}

	options_description_easy_init&
		operator()([[maybe_unused]] const char* name,
			[[maybe_unused]] const char* description)
	{
		return *this;
	}

	options_description_easy_init&
		operator()([[maybe_unused]] const char* name)
	{
		return *this;
	}

	options_description_easy_init&
		operator()([[maybe_unused]] const char* name, [[maybe_unused]] int a, [[maybe_unused]] const char* description)
	{
		return *this;
	}

private:
	options_description* owner_;
};

class options_description
{
public:
	options_description_easy_init add_options()
	{
		return options_description_easy_init(this);
	}
};



int main()
{
	options_description desc;
	desc.add_options()
		("name")
		("name2")
		("name3", "desc3");

	int value = 0;
	log_debug << "log test!!!! " << SST_VSTR(value);

	[[maybe_unused]] auto p1 = std::make_shared<TestClass>();
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

	/*const client::windows windows("gui_windows", 8000);
	windows.start();*/

	const sst::gui::windows windows("gui_windows_thread", 8000);
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

