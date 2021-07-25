#pragma once

#include "basic_service.h"
#include "connector.h"


namespace sst::network
{
	class client_service : public basic_service
	{
	private:
		connector* _connector{ nullptr };
	};
}
