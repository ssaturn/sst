//#include "stdafx.h"
//#include "proactor.h"
//#include "async_completion_token.h"
//#include "packet_header.h"
//#include "io_object.h"
//#include <process.h>
//
//
//namespace sst::network
//{
//	proactor::proactor()
//	{
//		time_out_ = INFINITE;
//		completion_port_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 0);
//		if (completion_port_ == nullptr)
//		{
//			assert(false);
//		}
//	}
//
//	proactor::~proactor()
//	{
//		CloseHandle(completion_port_);
//	}
//
//	UINT WINAPI proactor::thread_proc(void* proactor)
//	{
//		static_cast<class proactor*>(proactor)->proc();
//
//		return 0;
//	}
//
//	void proactor::register_socket(const SOCKET socket) const
//	{
//		::CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket), completion_port_, 0, 0);
//	}
//
//	void proactor::post_private(const DWORD id, async_completion_token* act) const
//	{
//		BOOL ret = ::PostQueuedCompletionStatus(completion_port_, 0, id, static_cast<LPOVERLAPPED>(act));
//		ret;
//	}
//
//	void proactor::proc() const
//	{
//		while (true)
//		{
//			static constexpr ULONG num_of_pending_recv = 64;
//
//			OVERLAPPED_ENTRY overlapped_entries[num_of_pending_recv];
//			ULONG num_of_entry = 0;
//
//			if (const BOOL status = ::GetQueuedCompletionStatusEx(completion_port_, overlapped_entries,
//			                                                      num_of_pending_recv, &num_of_entry, time_out_,
//			                                                      false); status == TRUE)
//			{
//				for (ULONG i = 0; i < num_of_entry; ++i)
//				{
//					const auto act = reinterpret_cast<async_completion_token*>(overlapped_entries[i].lpOverlapped);
//					auto obj = reinterpret_cast<io_object*>(overlapped_entries[i].lpCompletionKey);
//					if (obj)
//					{
//						obj->handle_completion(act, overlapped_entries[i].dwNumberOfBytesTransferred);
//					}
//					
//					if (act == nullptr)
//					{
//						std::cout << "ProcEvents() Ov NULL Error!!! err : " << WSAGetLastError() << std::endl;
//						continue;
//					}
//				}
//			}
//			else
//			{
//				const DWORD error = WSAGetLastError();
//				for (ULONG i = 0; i < num_of_entry; ++i)
//				{
//					auto* act = reinterpret_cast<async_completion_token*>(overlapped_entries[i].lpOverlapped);
//					if (act == nullptr)
//					{
//						std::cout << "ProcEvents() Ov NULL Error!!! err : " << WSAGetLastError() << std::endl;
//						continue;
//					}
//
//					act->error(error);
//				}
//			}
//		}
//	}
//
//	bool proactor::register_object(io_object* obj) const
//	{
//		return (CreateIoCompletionPort(obj->get_handle(), completion_port_, reinterpret_cast<ULONG_PTR>(obj), 0) != nullptr);
//	}
//}
