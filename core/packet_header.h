#pragma once


namespace sst
{
	#pragma pack(push, 1)		
	class packet_header
	{
	public:
		explicit packet_header(const uint16 type = 0, const uint16 size = 0)
			: size(size)
			, type(type)
		{
		}

		uint16 size{ 0 };
		uint16 type{ 0 };
	};
	#pragma pack(pop)	
}

