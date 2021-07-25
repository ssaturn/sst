#pragma once

#include "file_helper.h"

namespace sst
{
	struct memory_header
	{
		size_t total_size{ 0 };

		explicit memory_header(const size_t size)
			: total_size(size)
		{
		}
	};

	template<int32 PageSize>
	class memory_block
	{
		size_t block_size_{ 0 };
		size_t block_per_page_{ 0 };

	public:
		explicit memory_block(const int32 block_size)
			: block_size_(block_size)
			, block_per_page_(PageSize / block_size)
		{
			assert(block_per_page_ > 0);
		}

		void* pop()
		{
			return nullptr;
		}

		void push([[maybe_unused]] void* memory)
		{
		}
	};
}
