#pragma once

#include "common.h"
#include "trait_util/tuple_element_index.h"

namespace sst::network
{
	class actor
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(actor)

	public:
		enum class type : uint8
		{
			sender = 0,
			receiver,
			prereceiver,
			disconnector,
			acceptor,
			connector,
			max
		};
		static constexpr size_t max_actor_type = static_cast<size_t>(type::max);
		static constexpr type type_value = type::sender;

	protected:
		explicit actor(class actor_owner* owner)
			: owner_(owner)
		{
			
		}
		
	public:
		virtual ~actor() = default;
		
		virtual void complete(struct async_completion_token* act, DWORD bytes_transferred) = 0;
		virtual void error(struct async_completion_token* act, DWORD error) = 0;

		template<typename ActorOwner> requires std::is_base_of_v<actor_owner, ActorOwner>
		ActorOwner* get_owner()
		{
			return static_cast<ActorOwner*>(owner_);
		}

		[[nodiscard]] actor_owner* get_owner() const
		{
			return owner_;
		}

		template<typename ActorTy> requires std::derived_from<ActorTy, actor>
		ActorTy* get();

	private:
		actor_owner* owner_{ nullptr };
	};

	
	template<typename T, typename ... Args>
	class actor_owner
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(actor_owner)
	
	public:
		static constexpr std::size_t size = sizeof...(Args);

		actor_owner()
		{
			//std::make_unique<>()
		}
		~actor_owner() = default;

		template<typename ActorTy, typename ... ActorArgs> requires std::is_base_of_v<class actor, ActorTy>
		void register_actor(ActorArgs&& ... args)
		{
			//const auto index = static_cast<uint8>(ActorTy::type_value);

			constexpr auto index = trait_util::tuple_element_index<ActorTy, Args ...>::value;

			actors_[index] = std::make_unique<ActorTy>(std::forward<decltype(args)>(args)...);
		}

		
		template<typename ActorTy> requires std::is_base_of_v<class actor, ActorTy>
		ActorTy* get()
		{
			const auto index = static_cast<uint8>(ActorTy::type_value);
			return static_cast<ActorTy*>(actors_[index].get());
		}

	private:
		std::array<std::unique_ptr<actor>, size> actors_{};
		
	};

	// implement
	template <typename ActorTy> requires std::derived_from<ActorTy, actor>
	ActorTy* actor::get()
	{
		return owner_->get<ActorTy>();
	}
	
}
