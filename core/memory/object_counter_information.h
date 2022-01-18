#pragma once

#include <array>
#include <atomic>
#include <string_view>
#include <vcruntime_typeinfo.h>


namespace sst::memory
{
	class object_counter_information
	{
	public:
		struct information
		{
			std::string_view name{};
			std::atomic<size_t> count{};
			std::size_t size{};
		};
		static constexpr size_t max_class_count = 1024;
		static std::array<information, max_class_count>	infos;
		static std::atomic<size_t>						indexer;
	};

	template<typename T>
	struct object_indexer
	{
		static size_t index;
	};

	template<typename ClassTy>
	size_t register_object()
	{
		const std::string_view sig = typeid(ClassTy).name();
		const auto index = object_counter_information::indexer.fetch_add(1);

		object_counter_information::infos[index].name = sig;
		object_counter_information::infos[index].size = sizeof(ClassTy);

		return index;
	}

	template<typename T>
	size_t object_indexer<T>::index = register_object<T>();
}
