#pragma once

#include <concepts>
#include <thread>
#include <mutex>
#include <core/logger.h>


namespace sst::utility
{
	template<typename FuncTy> requires std::invocable<FuncTy>
	class defer
	{
	public:
		using func_t = std::conditional_t<std::is_function_v<FuncTy>, std::add_pointer_t<FuncTy>, FuncTy>;

		explicit defer(FuncTy&& func)
			: func_(std::forward(func))
		{
			
		}

		~defer()
		{
			DoOnce();
		}

	private:
		void DoOnce()
		{
			try
			{
				std::call_once(flag_, func_);
			}
			catch(...)
			{
				log_debug() << L"error call once ";
			}
		}

		func_t func_;
		std::once_flag flag_;
	};
}