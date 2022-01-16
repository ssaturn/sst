#pragma once

#include <array>
#include <atomic>
#include <string_view>
#include <vcruntime_typeinfo.h>


namespace sst
{
	class class_instance_counter
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
	struct class_indexer
	{
		static size_t index;
	};

	template<typename ClassTy>
	size_t register_class()
	{
		const std::string_view sig = typeid(ClassTy).name();
		const auto index = class_instance_counter::indexer.fetch_add(1);

		class_instance_counter::infos[index].name = sig;
		class_instance_counter::infos[index].size = sizeof(ClassTy);

		return index;
	}

	template<typename T>
	size_t class_indexer<T>::index = register_class<T>();
}
