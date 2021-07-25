#pragma once

#include <Windows.h>
#include <vector>
#include <array>
#include <cassert>
#include "file_helper.h"
#include <concurrent_queue.h>

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
		//concurrency::concurrent_queue<>;

	public:
		explicit memory_block(const int32 block_size)
			: block_size_(block_size)
			, block_per_page_(PageSize / block_size)
		{
			assert(block_per_page_ > 0);
		}

		void* pop()
		{
			
			//SLIST_ENTRY* entry = ::InterlockedPopEntrySList(&m_freeList);
			//if (entry != nullptr)
			//{
			//	return entry;
			//}

			//void* newPage = ::malloc(PageSize);
			//assert(newPage != nullptr);

			//auto baseAddr = reinterpret_cast<unsigned char*>(newPage);
			//for (size_t i = 1; i < block_per_page_; ++i)	// 1번째 것은 리턴할 것이기 때문에 1부터
			//{
			//	push(baseAddr + (i * block_size_));
			//}

			//auto header = reinterpret_cast<memory_header*>(newPage);

			//return header;
			return nullptr;
		}

		void push(void* memory)
		{
			if (memory)
			{
				//memory_header* header = reinterpret_cast<memory_header*>(memory);
				//::InterlockedPushEntrySList(&m_freeList, header);
			}
		}
	};

	/*class DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) memory_padding
	{
	public:

		unsigned int padding;

		explicit memory_padding(uint32_t padding_)
			: padding(padding_)
		{
		}
	};*/


	template<int PAGE_SIZE, int BLOCK_COUNT>
	class MemoryManagerT
	{
		enum : unsigned int
		{
			PADDING_HEAD = 0x98723523,
			PADDING_TAIL = 0xABCEDFAB,
		};

		//typedef std::vector<MemoryBlock<PAGE_SIZE>*>	BlockVec;

		std::array<memory_block<PAGE_SIZE>*, BLOCK_COUNT> m_blocks;
		std::vector<memory_block<PAGE_SIZE>*> m_sizeToBlock;
		//BlockVec				m_sizeToBlock;
		size_t					m_maxBlockSize;

	public:

		MemoryManagerT()
			: m_maxBlockSize(0)
			, m_sizeToBlock()
		{
			int currentIndex = 1;
			for (auto i = 0; i < (int)m_blocks.size(); ++i)
			{
				int blockSize = (4 + (i & 3)) << (1 + (i >> 2));
				if ((blockSize % MEMORY_ALLOCATION_ALIGNMENT) == 0 &&
					blockSize >= sizeof(SLIST_ENTRY))
				{
					m_blocks[i] = new memory_block<PAGE_SIZE>(blockSize);
					m_maxBlockSize = blockSize;

					while (currentIndex <= blockSize)
					{
						m_sizeToBlock.push_back(m_blocks[i]);
						++currentIndex;
					}
				}
				else
				{
					m_blocks[i] = nullptr;
				}
			}
		}

		//void* alloc(size_t size)
		//{
		//	void* memory = nullptr;
		//	size_t totalSize = sizeof(memory_header) + size + (2 * sizeof(memory_padding));	// 앞뒤로 두개
		//	if (m_maxBlockSize < totalSize)
		//	{
		//		memory = ::malloc(totalSize);
		//	}
		//	else
		//	{
		//		memory = m_sizeToBlock[totalSize]->pop();
		//	}

		//	void* headerAddr = memory;
		//	memory_header* header = new (headerAddr)memory_header(totalSize);

		//	void* headAddr = header + 1;
		//	memory_padding* head = new (headAddr)memory_padding(PADDING_HEAD);
		//	void* tailAddr = reinterpret_cast<unsigned char*>(head + 1) + size;
		//	memory_padding* tail = new (tailAddr)memory_padding(PADDING_TAIL);
		//	(tail);

		//	return (head + 1);
		//}

		/*void free(void* memory)
		{
			memory_padding* head = reinterpret_cast<memory_padding*>(memory) - 1;
			memory_header* header = reinterpret_cast<memory_header*>(head) - 1;

			size_t totalSize = header->total_size;
			memory_padding* tail = reinterpret_cast<memory_padding*>(reinterpret_cast<unsigned char*>(header) + totalSize - sizeof(memory_padding));

			assert(head->padding == PADDING_HEAD);
			assert(tail->padding == PADDING_TAIL);

			if (totalSize > m_maxBlockSize)
			{
				::free(header);
			}
			else
			{
				m_sizeToBlock[totalSize]->push(header);
			}
		}*/
	};

#define MEMORY_PAGE_SIZE	(256 * 1024)
#define MEMORY_BLOCK_COUNT	(53)			// 64Kb 까지
	typedef MemoryManagerT<MEMORY_PAGE_SIZE, MEMORY_BLOCK_COUNT>	MemoryManager;

}

extern sst::MemoryManager* g_memory_manager;