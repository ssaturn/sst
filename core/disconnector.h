#pragma once

#include "actor.h"


namespace sst::network
{
	class disconnector final : public actor
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(disconnector)
	
	public:
		static constexpr type type_value = type::disconnector;
		
		explicit disconnector(session* owner);
		~disconnector() override = default;

		void proc();
		void complete(async_completion_token* act, DWORD bytes_transferred) override;
		void error(async_completion_token* act, DWORD error) override;
	};
}
