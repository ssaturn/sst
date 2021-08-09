#pragma once

#include <sql.h>
#include <sqlext.h>


namespace sst::database::odbc
{
	using string = std::wstring;
	
	class connection
	{
	public:
		connection(const string& dsn, const string& user, const string& pass, long timeout = 0);
		connection(const string& connection_string, long timeout = 0);
		connection();
		connection(const connection& rhs);
		connection(connection&& rhs) noexcept;
		connection& operator=(connection rhs);
		connection& operator=(connection&& rhs) noexcept;
		~connection() noexcept;

		void swap(connection&) noexcept;

		bool connect(SQLHENV henv, const wchar_t* connection_string);
		
		void allocate();
		void deallocate();
		void close() const;

		bool async_connect(const string& dsn, const string& user, const string& pass, void* event_handle, long timeout = 0);
		bool async_connect(const string& connection_string, void* event_handle, long timeout = 0);
		void async_complete();

		bool connected() const;
		void disconnect();
		std::size_t transactions() const;
		void* get_dbc_handle() const;
		void* get_env_handle() const;

		template <class T>
		T get_info(short info_type) const;
		
		string dbms_name() const;
		string dbms_version() const;
		string driver_name() const;
		string database_name() const;
		string catalog_name() const;
	
	private:
		std::size_t ref_transaction();
		std::size_t unref_transaction();
		bool rollback() const;
		void rollback(bool onoff);

		//friend class nanodbc::transaction::transaction_impl;
		class impl;
		std::shared_ptr<impl> impl_;
	};


	
}
