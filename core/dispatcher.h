#pragma once

#include "common.h"
#include "packet_header.h"


namespace sst::network
{
	template <class Callback, int MaxSize>
	class dispatcher
	{
	public:
		dispatcher()
			: callbacks_()
		{

		}
		~dispatcher()
		{

		}

		void register_callback(uint16 type, Callback* callback)
		{
			callbacks_[type] = callback;
		}

		Callback* dispatch(uint16 type)
		{
			return callbacks_[type];
		}

		// Packet Dispatch에서 사용
		bool process(class session* session, const char* pTr, const uint16 pTrSz)
		{
			auto header = reinterpret_cast<const packet_header*>(pTr);
			if (header->type >= callbacks_.size() || callbacks_[header->type] == nullptr)
			{
				return false;
			}

			callbacks_[header->type]->Proc(static_cast<void*>(session), pTr, pTrSz);

			return true;
		}

	private:
		std::array<Callback*, MaxSize> callbacks_;
	};
}
