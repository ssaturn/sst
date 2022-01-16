#pragma once


#include <core/windows.h>

namespace client
{
	class windows final : public sst::gui::windows
	{
	public:
		using sst::gui::windows::windows;

		void run_custom() override;
	};
}
