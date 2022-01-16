#include "class_instance_counter.h"


namespace sst
{
	std::array<class_instance_counter::information, class_instance_counter::max_class_count>	class_instance_counter::infos{};
	std::atomic<size_t>																			class_instance_counter::indexer{};
}
