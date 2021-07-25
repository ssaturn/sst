#pragma once

#include "actor.h"


import sst;

namespace sst::network
{
	class connector final : public actor
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(connector)
	
	public:
		static constexpr type type_value = type::connector;

		explicit connector(actor_owner* owner);
		~connector() override = default;

		void connect(const ipv4_address& addr);
		void complete(async_completion_token* token, DWORD bytes_transferred) override;
		void error(async_completion_token* token, DWORD error) override;
	};

}