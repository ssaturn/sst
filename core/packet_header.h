#pragma once


namespace sst
{
	#pragma pack(push, 1)		
	class packet_header
	{
	public:
		explicit packet_header(uint16 type = 0, uint16 size = 0)
			: size(size)
			, type(type)
		{
		}

		uint16 size{ 0 };
		uint16 type{ 0 };
	};
	#pragma pack(pop)

	constexpr size_t packet_header_size = sizeof(packet_header);
}

