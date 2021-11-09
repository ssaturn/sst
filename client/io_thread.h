#pragma once

#include <core/thread.h>
#include <core/proactor.h>
#include <core/iocp.h>

using namespace sst::network;

class io_thread final : public sst::threading::thread
{
public:
	explicit io_thread(class proactor<iocp>* proactor)
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
	proactor<iocp>* proactor_{ nullptr };
};
