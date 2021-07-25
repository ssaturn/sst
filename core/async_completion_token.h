#pragma once

#include "actor.h"

namespace sst::network
{
	struct async_completion_token : OVERLAPPED
	{
		explicit async_completion_token(actor* actor);

		void clear();
		void complete(DWORD bytes_transferred);
		void error(DWORD error);

		WSABUF wsabuf{};
		actor* actor{ nullptr };
	};
}


