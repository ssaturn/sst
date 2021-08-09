#pragma once

#include <sqlext.h>
#include <sqltypes.h>
#include <type_traits>
#include "common.h"


namespace sst::database
{
	constexpr int32 max_varchar_str_len = 8000;
	constexpr int32 max_nvarchar_str_len = 4000;
	
	template<typename DataTy>
	constexpr SQLSMALLINT to_c_type()
	{
		if constexpr (std::is_same_v<int32, DataTy>)
		{
			return SQL_C_SLONG;
		}
		else if constexpr (std::is_same_v<uint32, DataTy>)
		{
			return SQL_C_ULONG;
		}
		else if constexpr (std::is_same_v<int64, DataTy>)
		{
			return SQL_C_SBIGINT;
		}
		else if constexpr (std::is_same_v<uint64, DataTy>)
		{
			return SQL_C_UBIGINT;
		}
		else if constexpr (std::is_same_v<int16, DataTy>)
		{
			return SQL_C_SSHORT;
		}
		else if constexpr (std::is_same_v<uint16, DataTy>)
		{
			return SQL_C_USHORT;
		}
		else if constexpr (std::is_same_v<int8, DataTy>)
		{
			return SQL_C_STINYINT;
		}
		else if constexpr (std::is_same_v<uint8, DataTy>)
		{
			return SQL_C_UTINYINT;
		}
		else if constexpr (std::is_same_v<float, DataTy>)
		{
			return SQL_C_FLOAT;
		}
		else if constexpr (std::is_same_v<double, DataTy>)
		{
			return SQL_C_DOUBLE;
		}
		else if constexpr (std::is_same_v<char, DataTy>)
		{
			return SQL_C_CHAR;
		}
		
		
		

		return SQL_C_DEFAULT;
	}

	template<typename DataTy>
	constexpr SQLSMALLINT to_sql_type()
	{
		return SQL_UNKNOWN_TYPE;
	}
}
