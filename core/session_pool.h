#pragma once

#include "session.h"
#include <concurrent_queue.h>
#include "common.h"



namespace sst::network
{
	template<typename SessionTy, std::size_t MaxCount>
	class session_pool
	{
	public:
		session_pool();
		~session_pool();

		bool create();
		auto acquire()-> SessionTy*;
		void release(session* session);
		[[nodiscard]] bool empty() const;

	protected:
		concurrency::concurrent_queue<SessionTy*> queue_{};
	};

	template <typename SessionTy, std::size_t MaxCount>
	session_pool<SessionTy, MaxCount>::session_pool()
	{
	}

	template <typename SessionTy, std::size_t MaxCount>
	session_pool<SessionTy, MaxCount>::~session_pool()
	{
		SessionTy* session_ptr = nullptr;
		while(queue_.try_pop(session_ptr))
		{
			delete session_ptr;
			session_ptr = nullptr;
		}
	}

	template <typename SessionTy, std::size_t MaxCount>
	bool session_pool<SessionTy, MaxCount>::create()
	{
		for (auto i = 0; i < MaxCount; ++i)
		{
			auto session = new SessionTy();
			if (session == nullptr)
			{
				return false;
			}

			queue_.push(session);
		}

		return true;
	}

	template <typename SessionTy, std::size_t MaxCount>
	SessionTy* session_pool<SessionTy, MaxCount>::acquire()
	{
		if (SessionTy * session_ptr = nullptr; queue_.try_pop(session_ptr))
		{
			return session_ptr;
		}

		return new SessionTy();
	}

	template <typename SessionTy, std::size_t MaxCount>
	void session_pool<SessionTy, MaxCount>::release(session* session)
	{
		queue_.push(session);
	}

	template <typename SessionTy, std::size_t MaxCount>
	bool session_pool<SessionTy, MaxCount>::empty() const
	{
		return queue_.empty();
	}
}
