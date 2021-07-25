#include "stdafx.h"
#include "behavior.h"

namespace sst
{
	behavior::behavior(behavior_owner* owner, behavior_type type)
		: owner_(owner)
		, type_(type)
	{

	}

	behavior::~behavior()
	{
		for (auto& bv : pending_)
		{
			delete bv;
		}
		pending_.clear();
	}

	bool behavior::done() const
	{
		// ���� ���ð� üũ
		if (end_reason_ == behavior_end_reason::BER_NONE)
		{
			return false;
		}

		return std::chrono::system_clock::now() >= end_tick_;
	}

	void behavior::SetEnd(int64 after, behavior_end_reason reason)
	{
		end_reason_ = reason;
		end_tick_ = std::chrono::system_clock::now() + std::chrono::milliseconds(after);
	}




}