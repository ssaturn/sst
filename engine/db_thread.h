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

		// connection�� ������ ������ �׷��� �����庰�� connection���� ó���� �� �� �ֵ��� ����.

	};
}
