#pragma once

#include "../core/thread.h"


import sst.network;

class io_thread final : public sst::threading::thread
{
public:
	explicit io_thread(class sst::network::proactor* proactor)
		: thread("io_thread", 0)
		, proactor_(proactor)
	{

	}

	bool setup() override { return true; }
	void run() override
	{
		while (true)
		{
			proactor_->proc();
		}
	}
	void cleanup() override {}

private:
	sst::network::proactor* proactor_{ nullptr };
};
