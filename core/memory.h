#pragma once


namespace sst::memory
{
	struct memory_reusable {};
	struct memory_disposable {};

	struct disposable
	{
		virtual ~disposable() = default;
		using memory_type = memory_disposable;
	};
}