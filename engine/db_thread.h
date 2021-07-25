#pragma once

#include <core/thread.h>

namespace sst::engine::database
{
	class db_thread : public threading::thread
	{
	public:

	protected:
		bool setup() override { return true; }
		void run() override {}
		void cleanup() override {}

		// connection을 가지게 만들자 그래서 쓰레드별로 connection으로 처리를 할 수 있도록 하자.

	};
}
