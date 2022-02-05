#pragma once

#include "common.h"
#include "pattern/static_component.h"


namespace sst::network
{
	class actor : public pattern::static_component<class session>
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(actor)

	public:
		enum class type : uint8
		{
			sender = 0,
			receiver,
			prereceiver,
			disconnector,
			acceptor,
			connector,
			max
		};
		//static constexpr size_t max_actor_type = static_cast<size_t>(type::max);

	protected:
		explicit actor(session* session_ptr)
			: pattern::static_component<session>(session_ptr)
		{
			
		}
		
	public:
		~actor() override = default;
		
		virtual void complete(struct async_completion_token* act, DWORD bytes_transferred) = 0;
		virtual void error(struct async_completion_token* act, DWORD error) = 0;
	};
}
