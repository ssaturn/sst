#include "thread.h"
#include <memory>
#include <type_traits>
#include <vector>

export module sst.threading.thread_pool;

namespace sst::threading
{
	export class thread_pool
	{
	public:
		explicit thread_pool(const size_t size)
		{
			threads_.resize(size);
		}
		
		template<typename ThreadTy, typename ... Args> requires std::is_base_of_v<thread, ThreadTy>
		void make_thread(Args&& ... args)
		{
			auto thread = std::make_shared<ThreadTy>(std::forward<decltype(args)>(args)...);
			threads_.push_back(thread);
		}

		void run();

	private:
		std::vector<std::shared_ptr<thread>> threads_{};
	};

	void thread_pool::run()
	{
		for(auto& thread : threads_)
		{
			if (thread)
			{
				thread->start();
			}
		}
	}
}
