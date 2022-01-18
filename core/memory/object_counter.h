#pragma once

#include "object_counter_information.h"


namespace sst::memory
{
	template<typename T>
	class object_counter
	{
	protected:
		object_counter()
		{
			object_counter_information::infos[object_indexer<T>::index].count.fetch_add(1);
		}

		virtual ~object_counter()
		{
			object_counter_information::infos[object_indexer<T>::index].count.fetch_sub(1);
		}
	};
}