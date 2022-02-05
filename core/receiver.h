#pragma once

#include "actor.h"
#include "recv_buffer.h"


namespace sst::network
{
	class receiver final : public actor
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(receiver)
	
	public:
		static constexpr type type_value = type::receiver;
		
		explicit receiver(session* owner);
		~receiver() override = default;

		void proc();
		void complete(async_completion_token* token, DWORD bytes_transferred) override;
		void error(async_completion_token* token, DWORD error) override;

	private:
		recv_buffer buffer_;
	};
}
