#include "stdafx.h"
#include "sender.h"
#include "disconnector.h"
#include "logger.h"
#include "session.h"


namespace sst::network
{
	session* sender::get_owner()
	{
		return actor::get_owner<sst::network::session>();
	}

	void sender::proc(byte* buffer, const size_t size)
	{
		if (buffer == nullptr)
		{
			return;
		}
		
		buffer_.push(buffer, size);
		if (!is_sending_.load())
		{
			SendData();
		}
	}

	void sender::complete([[maybe_unused]] async_completion_token* token, [[maybe_unused]] DWORD bytes_transferred)
	{
		if(!buffer_.empty())
		{
			SendData();
			return;
		}

		is_sending_ = true;
		log_debug << "sender::complete";

	}

	void sender::error([[maybe_unused]] async_completion_token* token, [[maybe_unused]] DWORD error)
	{
	}

	bool sender::SendData()
	{
		const auto token = new async_completion_token(this);
		
		DWORD send_bytes = 0;
		buffer_.write_wsabuf();
		if (SOCKET_ERROR == WSASend(get_owner()->get_socket(), &(buffer_.wsabuf), 1, &send_bytes, 0, token, nullptr))
		{
			if (const auto error_code = ::WSAGetLastError(); error_code != WSA_IO_PENDING)
			{
				get_owner()->get<disconnector>()->proc();
				delete token;
				return false;
			}
		}

		return true;
	}


	
}