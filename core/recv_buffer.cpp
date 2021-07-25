#include "recv_buffer.h"


namespace sst::network
{
	recv_buffer::recv_buffer(const size_t io_buffer_size)
		: size_(io_buffer_size)
	{
		buf_ = new byte[size_];
	}

	recv_buffer::~recv_buffer()
	{
		delete[] buf_;
		buf_ = nullptr;
	}

	auto recv_buffer::get_process_buffer(const DWORD bytes_of_transferred) -> std::pair<byte*, int32>
	{
		end_ += bytes_of_transferred;
		if (end_ > static_cast<int32>(size_))
		{
			//GConsolePrinter->OutStdErr(LBLUE, L"disconnected: fukc overflow\n");
			return std::make_pair(nullptr, 0);
		}

		return { buf_ + start_, end_ - start_ };
	}

	void recv_buffer::set_processed(const size_t bytes_of_processed)
	{
		start_ += static_cast<int32>(bytes_of_processed);
	}

	auto recv_buffer::get_buffer() -> std::tuple<CHAR*, ULONG>
	{
		const int32 buf_size = end_ - start_;
		assert(buf_size >= 0 && buf_size < size_);

		if (buf_size == 0)
		{
			start_ = end_ = 0;
		}
		else
		{
			::memmove(buf_, buf_ + start_, buf_size);
			start_ = 0;
			end_ = buf_size;
		}

		if (size_ - end_ < 1)
		{
			//GConsolePrinter->OutStdErr(LBLUE, L"disconnected: recv buffer overflow\n");
			return { nullptr, 0 };
		}
		
		return {reinterpret_cast<CHAR*>(buf_ + end_), static_cast<ULONG>(size_ - end_) };
	}
}
