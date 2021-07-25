//#pragma once
//
//#include "dispatcher.h"
//#include "io_object.h"
//#include "packet_callback.h"
//
//
//
//namespace sst::network
//{
//	class proactor
//	{
//		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(proactor)
//	
//	public:
//		proactor();
//		~proactor();
//		
//		static UINT	WINAPI thread_proc(void* proactor);
//
//		void register_socket(SOCKET socket) const;
//		bool register_object(io_object* obj) const;
//		void post_private(DWORD id, struct async_completion_token* act) const;
//		void proc() const;
//		
//		
//	private:
//		HANDLE completion_port_{ INVALID_HANDLE_VALUE };
//		uint32 time_out_{ 0 };
//		uint32 num_of_thread_{ 0 };
//
//		dispatcher<callback, USHRT_MAX> dispatcher_{};
//	};
//	
//}