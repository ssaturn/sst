#pragma once

#include "actor.h"


namespace sst::network
{
	class prereceiver final : public actor
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(prereceiver)
	
	public:
		static constexpr type type_value = type::prereceiver;
		
		explicit prereceiver(actor_owner* owner);
		prereceiver() = delete;
		~prereceiver() override = default;

		void proc();
		void complete(async_completion_token* token, DWORD bytes_transferred) override;
		void error(async_completion_token* token, DWORD error) override;
	};
}
