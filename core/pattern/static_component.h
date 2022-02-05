#pragma once

#include <array>
#include <memory>
#include <concepts>
#include "../trait_util/tuple_element_index.h"

namespace sst::pattern
{
	template<typename OwnerTy>
	class static_component
	{
	public:
		explicit static_component(OwnerTy* owner)
			: owner_(owner)
		{
			
		}

		virtual ~static_component()
		{
			
		}

		[[nodiscard]] OwnerTy* get_owner()
		{
			return owner_;
		}

		[[nodiscard]] const OwnerTy* get_owner() const
		{
			return owner_;
		}

	private:
		OwnerTy* owner_{ nullptr };
	};

	template<typename OwnerTy, typename ... TypeOfComponents>
	class static_component_owner
	{
		static constexpr size_t SIZE = sizeof...(TypeOfComponents);

		std::array<std::unique_ptr<static_component<OwnerTy>>, SIZE> actors_{};

	public:
		

		template<typename ComponentTy> requires std::derived_from<ComponentTy, static_component<OwnerTy>>
		ComponentTy* get()
		{
			constexpr size_t index = trait_util::tuple_element_index<ComponentTy, std::tuple<TypeOfComponents ...>>::value;
			return static_cast<ComponentTy*>(actors_[index].get());
		}
	};

}