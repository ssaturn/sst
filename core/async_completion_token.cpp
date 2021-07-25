#include "stdafx.h"
#include "async_completion_token.h"


namespace sst::network
{
	async_completion_token::async_completion_token(class actor* actor)
		: _OVERLAPPED()
		, actor(actor)
	{
		clear();
	}

	void async_completion_token::clear()
	{
		hEvent = nullptr;
		Internal = 0;
		InternalHigh = 0;
		Offset = 0;
		OffsetHigh = 0;
	}

	void async_completion_token::complete(const DWORD bytes_transferred)
	{
		actor->complete(this, bytes_transferred);
	}

	void async_completion_token::error(const DWORD error)
	{
		actor->error(this, error);
	}
}
