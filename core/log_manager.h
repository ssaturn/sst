#pragma once


#include "thread.h"
#include "crash_handler.h"
#include "log_publisher.h"
#include <concurrent_queue.h>
#include <vector>
#include <map>
#include <thread>
#include <shared_mutex>



namespace sst
{
	class log_manager final : public threading::thread, public crash_handler::post_job_for_dump
	{
	public:
		explicit log_manager(const wchar_t* app_name);
		~log_manager() override;

		void add_publisher(const wchar_t* log_key, log_publisher* publisher);
		void push(const wchar_t* log_key, log_level_t log_level, time_t log_time, const wchar_t* log_file
				  , const wchar_t* log_function, int log_line, const wchar_t* log_text);
		void push(const wchar_t* log_key, log_level_t log_level, time_t log_time, const wchar_t* log_file
				  , const wchar_t* log_function, int log_line, const std::wstring& log_text);

	private:
		enum
		{
			FINISHED_TIMEOUT = 10 * 1000,
		};

		typedef std::vector<log_publisher*> publishers;
		typedef std::map<const wchar_t*, publishers> publishers_map;

		publishers* FindAndMake(const wchar_t* log_key);
		log_file_publisher* MakeFilePublisher(const wchar_t* log_key);

		bool setup() override;	// thread
		void run() override;		// thread
		void cleanup() override; // thread
		void wait_for() override; // Dump

		std::shared_mutex lock_{};
		std::wstring app_name_{};
		std::thread thread_{};
		publishers_map logs_{};
		Concurrency::concurrent_queue<async_log*> log_queue_{};
	};
}

extern sst::log_manager* g_log_manager;
