#pragma once

#include "common.h"


namespace sst::memory::buffer
{
	class buffer
	{
	public:
		buffer(byte* data, const ULONG length)
			: buf(data)
			, size(length)
		{
			/*buf = new byte[size + 1];
			memmove_s(buf, size, data, length);*/
		}

		virtual ~buffer()
		{
			delete[] buf;
			buf = nullptr;
		}

		byte* buf{ nullptr };
		ULONG size{ 0 };
	};
}
