#pragma once

#include <Windows.h>
#include <assert.h>


namespace sst
{
	#pragma pack(push, 1)		
	class packet_header
	{
	public:
		explicit packet_header(const unsigned short type = 0, const unsigned short size = 0)
			: size(size)
			, type(type)
		{
		}

		unsigned short	size{ 0 };
		unsigned short	type{ 0 };
	};
	#pragma pack(pop)	
}

