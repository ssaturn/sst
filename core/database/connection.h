#pragma once

#include <sql.h>
#include <sqlext.h>


namespace sst::database
{
	class connection
	{
		DISALLOW_SPECIAL_MEMBER_FUNCTIONS(connection)
		
		SQLHDBC hdbc_{};
		SQLHSTMT hstmt_{};

	public:
		connection() = default;
		~connection();

		bool connect(SQLHENV henv, const wchar_t* connection_string);
		void close() const;

		[[nodiscard]] auto get_handle() const -> SQLHSTMT { return hstmt_; }
	};
}
