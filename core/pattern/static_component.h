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
		using TupleTy = std::tuple<TypeOfComponents ...>;
		std::array<std::unique_ptr<static_component<OwnerTy>>, SIZE> components_{};

	public:
		static_component_owner(OwnerTy* owner)
		{
			register_static_component(owner);
		}

		template <size_t I = 0>
		typename std::enable_if_t<I == SIZE, void>
			register_static_component([[maybe_unused]] OwnerTy* owner) {}

		template <size_t I = 0>
		typename std::enable_if_t<(I < SIZE), void>
			register_static_component(OwnerTy* owner)
		{
			components_[I] = std::make_unique<std::tuple_element_t<I, TupleTy>>(owner);
			register_static_component<I + 1>(owner);
		}

		template<typename ComponentTy> requires std::derived_from<ComponentTy, static_component<OwnerTy>>
		ComponentTy* get()
		{
			constexpr size_t index = trait_util::tuple_element_index<ComponentTy, TupleTy>::value;
			return static_cast<ComponentTy*>(components_[index].get());
		}
	};

}