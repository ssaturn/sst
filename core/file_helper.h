#pragma once

#include "common.h"


namespace sst::filesystem
{
	class file_helper
	{
	public:
		static byte* read_file(const wchar_t* filename, int64& read_size);
	};

	class file_closer
	{
	public:
		explicit file_closer(FILE* file)
			: file_(file)
		{
		}
		~file_closer()
		{
			if (file_ != nullptr)
			{
				::fclose(file_);
			}
		}
	private:
		FILE* file_;
	};
}
