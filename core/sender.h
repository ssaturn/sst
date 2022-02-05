#pragma once

#include "actor.h"
#include "send_buffer.h"


namespace sst::network
{
	class sender final : public actor
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(sender)
	
	public:
		//static constexpr type type_value = type::sender;

		explicit sender(session* owner)
			: actor(owner)
		{
			
		}
		~sender() override = default;

		void proc(byte* buffer, size_t size);
		void complete(async_completion_token* token, DWORD bytes_transferred) override;
		void error(async_completion_token* token, DWORD error) override;

	private:
		bool SendData();
		
		std::atomic<bool> is_sending_{ false };
		send_buffer buffer_{ 1024 * 1024 };
	};

	
}
