#pragma once

#include <concurrent_queue.h>
#include <vector>
#include "buffer.h"
#include "macro.h"


namespace sst::network
{
	class send_buffer
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(send_buffer)
	
	public:
		explicit send_buffer(size_t size);
		~send_buffer();
		
		auto push(const byte* data, size_t length) -> void;
		auto push(std::shared_ptr<memory::buffer::buffer> buffer) -> void;
		bool empty() const { return queue_.empty(); }
		void write_wsabuf();
		
		WSABUF wsabuf{};

	private:
		auto TryPopAll() -> std::vector<std::shared_ptr<memory::buffer::buffer>>;

		size_t buf_size_{ 0 };
		std::atomic<int32> remain_count_{ 0 };
		std::vector<std::shared_ptr<memory::buffer::buffer>> reserved_{};
		concurrency::concurrent_queue<std::shared_ptr<memory::buffer::buffer>> queue_{};
	};
}