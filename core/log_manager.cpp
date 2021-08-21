#include "stdafx.h"
#include "log_manager.h"
#include "file_helper.h"
#include <shared_mutex>

sst::log_manager* g_log_manager = nullptr;

namespace sst
{
	log_manager::log_manager(const wchar_t* app_name)
		: threading::thread("LogThread", 1)
		, app_name_(app_name)
	{
		// 덤프 중에도 로그 스레드는 계속 실행 되도록 하자.
		crash_handler::add_exclude_suspend_thread(get_os_id());

		start();
	}

	log_manager::~log_manager()
	{
		wait_for();
		cleanup();
	}

	void log_manager::add_publisher(const wchar_t* log_key, log_publisher* publisher)
	{
		publishers* publishers = FindAndMake(log_key);
		if (publishers != nullptr)
		{
			publishers->push_back(publisher);
		}
	}

	void log_manager::push(const wchar_t* log_key, const log_level_t log_level, const time_t log_time,
	                       const wchar_t* log_file, const wchar_t* log_function, const int32 log_line, const wchar_t* log_text)
	{
		log_queue_.push(new async_log(log_key, log_level, log_time, log_file, log_function, log_line, log_text, false));
	}

	void log_manager::push(const wchar_t* log_key, const log_level_t log_level, const time_t log_time,
	                       const wchar_t* log_file, const wchar_t* log_function, const int32 log_line,
	                       const std::wstring& log_text)
	{
		log_queue_.push(new async_log(log_key, log_level, log_time, log_file, log_function, log_line, log_text, false));
	}

	void log_manager::wait_for()
	{
		if (get_handle() != nullptr)
		{
			wchar_t* null_key = nullptr;
			const auto log = new async_log(null_key, log_level_t::info, ::time(nullptr), __FILEW__, __FUNCTIONW__,
			                               __LINE__, L"", true);
			log_queue_.push(log);

			::WaitForSingleObject(get_handle(), FINISHED_TIMEOUT);
			::CloseHandle(get_handle());
		}
	}

	log_manager::publishers* log_manager::FindAndMake(const wchar_t* log_key)
	{
		log_manager::publishers* publishers = nullptr;
		{
			
			std::shared_lock lock(lock_);

			auto pos = logs_.find(log_key);
			if (pos != logs_.end())
			{
				publishers = &pos->second;
			}
		}

		if (publishers == nullptr)
		{
			std::unique_lock lock(lock_);

			// 더블 체크
			auto pos = logs_.find(log_key);
			if (pos != logs_.end())
			{
				publishers = &pos->second;
			}
			else
			{
				log_manager::publishers temp;

				if (log_file_publisher * log_file_publisher = MakeFilePublisher(log_key); log_file_publisher != nullptr)
				{
					temp.push_back(log_file_publisher);
				}

				logs_[log_key] = temp;

				auto iter = logs_.find(log_key);
				if (iter != logs_.end())
				{
					publishers = &iter->second;
				}
			}
		}

		return publishers;
	}

	log_file_publisher* log_manager::MakeFilePublisher(const wchar_t* log_key)
	{
		const wchar_t* LoggingFolder = L"./Logging";

		SYSTEMTIME time;
		::GetLocalTime(&time);

		::CreateDirectoryW(LoggingFolder, NULL);

		wchar_t fileName[MAX_PATH] = { 0, };
		swprintf_s(fileName, _countof(fileName), L"%s/%s(%s) %04d-%02d-%02d_%02d_%02d_%02d.log",
			LoggingFolder, app_name_.c_str(), log_key,
			time.wYear, time.wMonth, time.wDay,
			time.wHour, time.wMinute, time.wSecond);

		/*HANDLE logFile = filesystem::file_helper::open_utf16_file(fileName);
		if (logFile == INVALID_HANDLE_VALUE)
			return NULL;

		log_file_publisher* logFilePublisher = new log_file_publisher(logFile);
		return logFilePublisher;*/
		return nullptr;
	}

	bool log_manager::setup()
	{
		return true;
	}

	void log_manager::run()
	{
		async_log* log = nullptr;
		while (log_queue_.try_pop(log))
		{
			publishers* publishers = FindAndMake(log->log_key);
			if (publishers == nullptr)
			{
				// 제거 
				delete log;
				log = nullptr;
				continue;
			}

			for (auto& pub : *publishers)
			{
				if (pub)
				{
					pub->publish(log);
				}
			}

			delete log;
			log = nullptr;
		}
	}

	void log_manager::cleanup()
	{
		std::unique_lock lock(lock_);

		auto pos = logs_.begin();
		auto end = logs_.end();
		for (; pos != end; ++pos)
		{
			publishers& publishers = pos->second;

			for (auto& pub : publishers)
			{
				pub->close();

				log_file_publisher* logFilePublisher = dynamic_cast<log_file_publisher*>(pub);
				if (logFilePublisher)
				{
					delete logFilePublisher;
					pub = nullptr;

				}
			}
		}

		logs_.clear();
	}

}
