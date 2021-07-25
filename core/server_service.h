#pragma once

#include "basic_service.h"
#include "acceptor.h"
#include "session_pool.h"


namespace sst::network
{
	class server_service : public basic_service
	{
	public:
		static constexpr size_t session_pool_size = 2000;
		
		//session_pool<session_pool_size> _pool{};
	};
}