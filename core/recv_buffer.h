#pragma once

#include "common.h"


namespace sst::network
{
	class recv_buffer
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(recv_buffer)

	public:
		explicit recv_buffer(const size_t io_buffer_size);
		~recv_buffer();
		auto get_buffer()->std::tuple<CHAR*, ULONG>;
		[[nodiscard("process_len have to notify this buffer")]]
		auto get_process_buffer(DWORD bytes_of_transferred) -> std::pair<byte*, int32 /* process length */>;
		void set_processed(size_t bytes_of_processed);

	private:
		size_t size_{ 0 };
		byte* buf_{ nullptr };
		int32 start_{ 0 };
		int32 end_{ 0 };
	};
}

