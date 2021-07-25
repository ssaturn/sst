#pragma once

#include "common.h"


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

		template<typename ActorTy>// requires std::is_base_of_v<class actor, ActorTy>
		ActorTy* get();

	private:
		actor_owner* owner_{ nullptr };
	};

	

	class actor_owner
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(actor_owner)
	
	public:
		actor_owner() = default;
		~actor_owner() = default;

		template<typename ActorTy, typename ... Args> requires std::is_base_of_v<class actor, ActorTy>
		void register_actor(Args ... args)
		{
			const auto index = static_cast<uint8>(ActorTy::type_value);
			actors_[index] = std::make_unique<ActorTy>(std::forward<decltype(args)>(args)...);
		}
		
		template<typename ActorTy> requires std::is_base_of_v<class actor, ActorTy>
		ActorTy* get()
		{
			const auto index = static_cast<uint8>(ActorTy::type_value);
			return static_cast<ActorTy*>(actors_[index].get());
		}

	private:
		std::array<std::unique_ptr<actor>, actor::max_actor_type> actors_{};
	};

	// implement
	template <typename ActorTy>// requires std::is_base_of_v<actor, ActorTy>
		ActorTy* actor::get()
		{
			return owner_->get<ActorTy>();
		}
	
}
