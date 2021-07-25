#include "stdafx.h"
#include "file_helper.h"
#include "Logger.h"

namespace sst::filesystem
{
	unsigned char* file_helper::read_file(const wchar_t* filename, int64& read_size)
	{
		BYTE* readBuffer = nullptr;
		read_size = 0;

		HANDLE handle = CreateFile(filename,
								   GENERIC_READ,
								   FILE_SHARE_READ,
								   NULL,
								   OPEN_EXISTING,
								   FILE_ATTRIBUTE_NORMAL,
								   NULL);

		if (handle == INVALID_HANDLE_VALUE)
		{
			LOG().error(L"Could not open file %s (error %d)\n", filename, GetLastError());
			return nullptr;
		}
		else
		{
			DWORD bytesRead = 0;
			DWORD bytesToRead = 0;
			bytesToRead = ::GetFileSize(handle, NULL);
			if (bytesToRead > 0)
			{
				readBuffer = new BYTE[bytesToRead + 1];
				BOOL result = ::ReadFile(handle, readBuffer, bytesToRead, &bytesRead, NULL);
				if (result == FALSE)
				{
					delete[] readBuffer;
					LOG().error(L"\"%s\" read error!! %d\n", filename, GetLastError());
					return nullptr;
				}

				read_size = bytesToRead;

				const BYTE UTF8_BOM[3] = { 0xEF, 0xBB, 0xBF };
				if (read_size >= sizeof(UTF8_BOM))
				{
					if (::memcmp(UTF8_BOM, readBuffer, sizeof(UTF8_BOM)) == 0)
					{
						BYTE* temp = new BYTE[read_size];
						::memcpy(temp, &readBuffer[sizeof(UTF8_BOM)], read_size - sizeof(UTF8_BOM));
						delete[] readBuffer;

						readBuffer = temp;
						read_size -= sizeof(UTF8_BOM);
					}
				}
			}
		}

		::CloseHandle(handle);

		return readBuffer;
	}

	HANDLE file_helper::open_utf16_file(const wchar_t* filename)
	{
		if (filename == NULL)
			return INVALID_HANDLE_VALUE;

		HANDLE handle = ::CreateFile(filename,
									 GENERIC_WRITE,
									 FILE_SHARE_READ,
									 NULL,
									 OPEN_ALWAYS,
									 FILE_ATTRIBUTE_NORMAL,
									 NULL);

		if (handle == INVALID_HANDLE_VALUE)
		{
			return INVALID_HANDLE_VALUE;
		}

		DWORD fileSize = 0;
		fileSize = ::GetFileSize(handle, NULL);
		if (fileSize == 0)
		{
			// 새로 생성된 파일이라면 BOM 설정을 한다.
			BYTE utf16Bom[2];
			utf16Bom[0] = 0xFF;
			utf16Bom[1] = 0xFE;

			DWORD written = 0;
			::WriteFile(handle, utf16Bom, sizeof(utf16Bom), &written, NULL);
			::FlushFileBuffers(handle);
		}

		::SetFilePointer(handle, 0, NULL, FILE_END);
		return handle;
	}
}

