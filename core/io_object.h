#pragma once

#include "async_completion_token.h"
#include "memory.h"
#include "common.h"


namespace sst::network
{
	// completion key
	class io_object : public memory::disposable
	{
	public:
		~io_object() override = default;

		virtual auto get_handle()->HANDLE = 0;
		virtual auto handle_completion(async_completion_token* token, DWORD bytes_transferred) -> void = 0;
	};

	
	
	class io_job : public io_object
	{
	public:
		auto get_handle()->HANDLE override;
		auto handle_completion(async_completion_token* token, DWORD bytes_transferred) -> void override;

		static io_job* get_job_object();
	};

	

	inline auto io_job::get_handle() -> HANDLE
	{
		return HANDLE(nullptr);
	}

	inline auto io_job::handle_completion([[maybe_unused]] async_completion_token* token,
	                                      [[maybe_unused]] DWORD bytes_transferred) -> void
	{
	}
}
