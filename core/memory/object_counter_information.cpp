#include "object_counter_information.h"


namespace sst::memory
{
	std::array<object_counter_information::information, object_counter_information::max_class_count>
	object_counter_information::infos{};

	std::atomic<size_t>
	object_counter_information::indexer{};
}
