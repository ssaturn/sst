#include "stdafx.h"
#include "disconnector.h"
#include "async_completion_token.h"
#include "session.h"


namespace sst::network
{
	disconnector::disconnector(actor_owner* owner)
		: actor(owner)
	{
		
	}

	void disconnector::disconnect()
	{
	}

	void disconnector::complete([[maybe_unused]] async_completion_token* act, [[maybe_unused]] DWORD bytes_transferred)
	{
	}

	void disconnector::error([[maybe_unused]] async_completion_token* act, [[maybe_unused]] DWORD error)
	{
	}
}