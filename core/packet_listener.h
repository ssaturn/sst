#pragma once

namespace sst
{
	class packet_listener
	{
	public:
		virtual void notify(const void* pTr, const unsigned pTrSz) = 0;
	};
}
