#include "../stdafx.h"
#include "connection.h"


namespace sst::database
{
	connection::~connection()
	{
		close();
	}

	bool connection::connect(const SQLHENV henv, const wchar_t* connection_string)
	{
		if (::SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc_) != SQL_SUCCESS)
		{
			return false;
		}

		SQLWCHAR in_connect_str[1024] = { 0, };
		::wcscpy_s(in_connect_str, connection_string);

		SQLWCHAR out_connect_str[1024] = { 0, };
		SQLSMALLINT out_connect_str_len = 0;

		const SQLRETURN result = ::SQLDriverConnectW(hdbc_, nullptr,
		                                             in_connect_str, _countof(in_connect_str),
		                                             out_connect_str, _countof(out_connect_str), &out_connect_str_len,
		                                             SQL_DRIVER_NOPROMPT);

		if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO)
		{
			return false;
		}

		if (::SQLAllocHandle(SQL_HANDLE_STMT, hdbc_, &hstmt_) != SQL_SUCCESS)
		{
			return false;
		}

		return true;
	}

	void connection::close() const
	{
		if (hstmt_ != nullptr)
		{
			::SQLFreeHandle(SQL_HANDLE_STMT, hstmt_);
		}

		if (hdbc_ != nullptr)
		{
			::SQLFreeHandle(SQL_HANDLE_DBC, hdbc_);
		}
	}
}

