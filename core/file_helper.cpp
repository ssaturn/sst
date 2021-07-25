#include "stdafx.h"
#include "file_helper.h"
#include "logger.h"
#include <filesystem>

namespace sst::filesystem
{
	unsigned char* file_helper::read_file(const wchar_t* filename, int64& read_size)
	{
		BYTE* readBuffer = nullptr;
		read_size = 0;

		std::filesystem::path filepath(filename);
		std::fstream file(filepath, std::ios::binary | std::ios::ate);
		if (!file)
		{
			//throw std::runtime_error(filepath.c_str() + L": " + std::strerror(errno));
		}
		if (!file.is_open())
		{
			//throw std::runtime_error(filepath.c_str() + L": " + std::strerror(errno));
		}

		if (const auto size = std::filesystem::file_size(filepath); size == 0)
		{
			return nullptr;
		}

		auto end = file.tellg();
		file.seekg(0, std::ios::beg);
		
		return readBuffer;
	}
}

