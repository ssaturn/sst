#pragma once

#include "common.h"

namespace sst::threading
{
	using thread_id_t = int32;
	
	class [[nodiscard]] thread
	{
	public:
		explicit thread(std::string_view name, thread_id_t thread_id);
		virtual ~thread();

		[[nodiscard]] HANDLE get_handle() const { return handle_; }
		uint32 get_os_id() const { return os_id_; }
		thread_id_t get_id() const { return id_; }
		void start() const;

	protected:
		virtual bool setup() = 0;
		virtual void run() = 0;
		virtual void cleanup() = 0;

	private:
		static unsigned int __stdcall thread_proc(void* arg);

		HANDLE handle_{ nullptr };
		uint32 os_id_{ 0 };
		thread_id_t id_{ 0 };
		char name_[128]{};
	};
}
