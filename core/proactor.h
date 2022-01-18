#pragma once

#include "async_completion_token.h"
#include "common.h"
#include "io_object.h"
#include "packet_header.h"
#include "memory/object_counter.h"


namespace sst::network
{
	template<typename IoModel> 
	class proactor : memory::object_counter<proactor<IoModel>>
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(proactor)

	public:
		proactor();
		~proactor();

		static UINT	WINAPI thread_proc(void* proactor);

		void register_socket(SOCKET socket) const;
		bool register_object(io_object* obj) const;
		void post_private(DWORD id, struct async_completion_token* act) const;
		void proc() const;

	private:
		HANDLE completion_port_{ INVALID_HANDLE_VALUE };
		uint32 time_out_{ 0 };

		std::shared_ptr<IoModel> io_;
	};

	template<typename IoModel>
	proactor<IoModel>::proactor()
	{
		time_out_ = INFINITE;
		completion_port_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 0);
		if (completion_port_ == nullptr)
		{
			assert(false);
		}
	}

	template<typename IoModel>
	proactor<IoModel>::~proactor()
	{
		CloseHandle(completion_port_);
	}

	template<typename IoModel>
	UINT WINAPI proactor<IoModel>::thread_proc(void* _proactor)
	{
		static_cast<class proactor<IoModel>*>(_proactor)->proc();

		return 0;
	}

	template<typename IoModel>
	void proactor<IoModel>::register_socket(const SOCKET socket) const
	{
		::CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket), completion_port_, 0, 0);
	}

	template<typename IoModel>
	void proactor<IoModel>::post_private(const DWORD id, async_completion_token* act) const
	{
		BOOL ret = ::PostQueuedCompletionStatus(completion_port_, 0, id, static_cast<LPOVERLAPPED>(act));
		ret;
	}

	template<typename IoModel>
	void proactor<IoModel>::proc() const
	{
		while (true)
		{
			static constexpr ULONG num_of_pending_recv = 64;

			OVERLAPPED_ENTRY overlapped_entries[num_of_pending_recv];
			ULONG num_of_entry = 0;

			if (const BOOL status = ::GetQueuedCompletionStatusEx(completion_port_, overlapped_entries,
																  num_of_pending_recv, &num_of_entry, time_out_,
																  false); status == TRUE)
			{
				for (ULONG i = 0; i < num_of_entry; ++i)
				{
					const auto act = reinterpret_cast<async_completion_token*>(overlapped_entries[i].lpOverlapped);
					auto obj = reinterpret_cast<io_object*>(overlapped_entries[i].lpCompletionKey);
					if (obj)
					{
						obj->handle_completion(act, overlapped_entries[i].dwNumberOfBytesTransferred);
					}

					if (act == nullptr)
					{
						std::cout << "ProcEvents() Ov NULL Error!!! err : " << WSAGetLastError() << std::endl;
						continue;
					}
				}
			}
			else
			{
				const DWORD error = WSAGetLastError();
				for (ULONG i = 0; i < num_of_entry; ++i)
				{
					auto* act = reinterpret_cast<async_completion_token*>(overlapped_entries[i].lpOverlapped);
					if (act == nullptr)
					{
						std::cout << "ProcEvents() Ov NULL Error!!! err : " << WSAGetLastError() << std::endl;
						continue;
					}

					act->error(error);
				}
			}
		}
	}

	template<typename IoModel>
	bool proactor<IoModel>::register_object(io_object* obj) const
	{
		return (CreateIoCompletionPort(obj->get_handle(), completion_port_, reinterpret_cast<ULONG_PTR>(obj), 0) != nullptr);
	}
}
