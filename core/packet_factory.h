#pragma once

#include <unordered_map>
#include "singleton.h"


namespace sst::network
{
	using packet_id_t = uint16_t;
	
	class packet_factory : public serialization::singleton<packet_factory>
	{
	public:
		struct packet_creator_base
		{
			virtual ~packet_creator_base() = default;
			
		};
		
		template<typename PacketTy>
		struct packet_creator final : packet_creator_base
		{
			packet_creator() = default;
			~packet_creator() override = default;
			
			using packet_type_value = PacketTy;
			static PacketTy* make_packet()
			{
				return new PacketTy();
			}
		};
		
		template<typename PacketTy>
		bool register_packet(const packet_id_t id)
		{
			if (auto result = packet_trunk_.emplace(id, packet_creator<PacketTy>()); !result.second)
			{
				// error log : duplicated register
				return false;
			}
			
			return true;
		}

		template<typename PacketTy>
		PacketTy* get_packet(const packet_id_t id)
		{
			if (auto iter = packet_trunk_.find(id); iter != packet_trunk_.end())
			{
				if (auto creator = static_cast<PacketTy*>(iter->second); creator != nullptr)
				{
					return creator->make_packet();
				}
			}
			return nullptr;
		}

	private:
		std::unordered_map<packet_id_t, packet_creator_base*> packet_trunk_;
	};
}
