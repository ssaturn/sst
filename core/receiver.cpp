#include "stdafx.h"
#include "receiver.h"
#include "disconnector.h"
#include "prereceiver.h"
#include "session.h"
#include "sender.h"


namespace sst::network
{
	receiver::receiver(actor_owner* owner)
		: actor(owner)
		, buffer_(1024 * 1024)
	{
	}

	void receiver::receive()
	{
		const auto token = new async_completion_token(this);
		std::tie(token->wsabuf.buf, token->wsabuf.len) = buffer_.get_buffer();
		
		DWORD recv_bytes = 0, flags = 0;
		if (SOCKET_ERROR == WSARecv(get_owner<session>()->get_socket(), &(token->wsabuf), 1, &recv_bytes, &flags, token, nullptr))
		{
			if (const size_t error_code = WSAGetLastError(); error_code != WSA_IO_PENDING)
			{
				//ErrorHandler(errorCode);
				delete token;
			}
		}
		
		std::cout << "pre_receive!!" << std::endl;
	}

	void receiver::complete([[maybe_unused]] async_completion_token* token, const DWORD bytes_transferred)
	{
		if (bytes_transferred == 0)
		{
			//GConsolePrinter->OutStdErr(LBLUE, L"disconnected: recv 0 bytes\n");
			get<disconnector>()->disconnect();
			return;
		}

		auto [buf, buf_length] = buffer_.get_process_buffer(bytes_transferred);
		const auto process_length = get_owner<session>()->on_receive_data(buf, buf_length);
		if (process_length > buf_length)
		{
			// wtf
			get<disconnector>()->disconnect();
			return;
		}

		buffer_.set_processed(process_length);

		get<prereceiver>()->prereceive();
	}

	void receiver::error([[maybe_unused]] async_completion_token* token, [[maybe_unused]] DWORD error)
	{
	}


}