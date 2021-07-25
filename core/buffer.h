#pragma once

#include "common.h"


namespace sst::network
{
	class buffer
	{
	public:
		buffer(const byte* data, const ULONG length)
			: size(length)
		{
			buf = new byte[size + 1];
			memmove_s(buf, size, data, length);
		}
	
		byte* buf{ nullptr };
		ULONG size{ 0 };
	};
}
