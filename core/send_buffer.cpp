#include "stdafx.h"
#include "send_buffer.h"
#include "buffer.h"


namespace sst::network
{
	send_buffer::send_buffer(const size_t size)
	{
		wsabuf.buf = new CHAR[size + 1];
		buf_size_ = size;
	}

	send_buffer::~send_buffer()
	{
		delete[] wsabuf.buf;
		wsabuf.buf = nullptr;
	}

	void send_buffer::push(const byte* data, const size_t length)
	{
		push(std::make_shared<memory::buffer::buffer>(data, static_cast<ULONG>(length)));
	}

	auto send_buffer::push(const std::shared_ptr<memory::buffer::buffer> buffer) -> void
	{
		queue_.push(buffer);
	}

	void send_buffer::write_wsabuf()
	{
		wsabuf.len = 0;
		
		auto buffers = TryPopAll();
		ULONG offset = 0;
		size_t written_count = 0;
		for(const auto& buffer : buffers)
		{
			if (buffer->size + offset > buf_size_)
			{
				break;
			}
			
			::memcpy_s(wsabuf.buf, buf_size_, buffer->buf, buffer->size);
			offset += buffer->size;
			written_count++;
		}
		
		wsabuf.len = offset;
		buffers.erase(buffers.begin(), buffers.begin() + written_count);
		if (!buffers.empty())
		{
			assert(reserved_.empty());
			reserved_.assign(buffers.begin(), buffers.end());
		}
	}

	auto send_buffer::TryPopAll() -> std::vector<std::shared_ptr<memory::buffer::buffer>>
	{
		std::vector buffers(reserved_.begin(), reserved_.end());
		reserved_.clear();
		
		std::shared_ptr<memory::buffer::buffer> node{ nullptr };
		while (queue_.try_pop(node))
		{
			buffers.push_back(node);
		}

		return buffers;
	}
}
