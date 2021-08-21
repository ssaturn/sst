#include "../../stdafx.h"
#include "connection.h"


#define SST_THROW_DATABASE_ERROR(handle, handle_type)                                          \
    throw sst::database_error(handle, handle_type, __FILE__ ":" SST_STRINGIZE(__LINE__) ": ")

namespace
{
	bool success(const RETCODE rc)
    {
#ifdef SST_ODBC_API_DEBUG
        std::cerr << "<-- rc: " << return_code(rc) << " | " << std::endl;
#endif
        return rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO;
    }

	
	void deallocate_handle(SQLHANDLE& handle, short handle_type)
    {
        if (!handle)
        {
            return;
        }
            

        RETCODE rc = SQLFreeHandle(handle_type, handle);
        if (!success(rc))
        {
            //SST_THROW_DATABASE_ERROR(handle, handle_type);
        	
        }
            
        handle = nullptr;
    }

	void allocate_env_handle(SQLHENV& env)
    {
        if (env)
        {
            return;
        }
            

       /* RETCODE rc;
        NANODBC_CALL_RC(SQLAllocHandle, rc, SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
        if (!success(rc))
            NANODBC_THROW_DATABASE_ERROR(env, SQL_HANDLE_ENV);

        try
        {
            NANODBC_CALL_RC(
                SQLSetEnvAttr,
                rc,
                env,
                SQL_ATTR_ODBC_VERSION,
                (SQLPOINTER)NANODBC_ODBC_VERSION,
                SQL_IS_UINTEGER);
            if (!success(rc))
                NANODBC_THROW_DATABASE_ERROR(env, SQL_HANDLE_ENV);
        }
        catch (...)
        {
            deallocate_handle(env, SQL_HANDLE_ENV);
            throw;
        }*/
    }

    void allocate_dbc_handle([[maybe_unused]] SQLHDBC& conn, [[maybe_unused]] SQLHENV env)
    {
        /*NANODBC_ASSERT(env);
        if (conn)
            return;

        try
        {
            RETCODE rc;
            NANODBC_CALL_RC(SQLAllocHandle, rc, SQL_HANDLE_DBC, env, &conn);
            if (!success(rc))
                NANODBC_THROW_DATABASE_ERROR(env, SQL_HANDLE_ENV);
        }
        catch (...)
        {
            deallocate_handle(conn, SQL_HANDLE_DBC);
            throw;
        }*/
    }
	
}

namespace sst::database::odbc
{
    class connection::impl
    {
    public:
        impl(const impl&) = delete;
        impl& operator=(const impl&) = delete;

        impl()
            : env_(nullptr)
            , dbc_(nullptr)
            , connected_(false)
            , transactions_(0)
            , rollback_(false)
        {
        }

        impl(const string& dsn, const string& user, const string& pass, long timeout)
            : env_(nullptr)
            , dbc_(nullptr)
            , connected_(false)
            , transactions_(0)
            , rollback_(false)
        {
            allocate();

            try
            {
                connect(dsn, user, pass, timeout);
            }
            catch (...)
            {
                deallocate();
                throw;
            }
        }

        impl(const string& connection_string, const long timeout)
            : env_(nullptr)
            , dbc_(nullptr)
            , connected_(false)
            , transactions_(0)
            , rollback_(false)
        {
            allocate();

            try
            {
                connect(connection_string, timeout);
            }
            catch (...)
            {
                deallocate();
                throw;
            }
        }

        ~impl() noexcept
        {
            try
            {
                disconnect();
            }
            catch (...)
            {
                // ignore exceptions thrown during disconnect
            }
            deallocate();
        }

        void allocate()
        {
            allocate_env_handle(env_);
            allocate_dbc_handle(dbc_, env_);
        }

        void deallocate()
        {
            deallocate_handle(dbc_, SQL_HANDLE_DBC);
            deallocate_handle(env_, SQL_HANDLE_ENV);
        }

        void enable_async([[maybe_unused]] void* event_handle)
        {
            assert(dbc_);

           /* RETCODE rc;
            NANODBC_CALL_RC(
                SQLSetConnectAttr,
                rc,
                dbc_,
                SQL_ATTR_ASYNC_DBC_FUNCTIONS_ENABLE,
                (SQLPOINTER)SQL_ASYNC_DBC_ENABLE_ON,
                SQL_IS_INTEGER);
            if (!success(rc))
                NANODBC_THROW_DATABASE_ERROR(dbc_, SQL_HANDLE_DBC);

            NANODBC_CALL_RC(
                SQLSetConnectAttr, rc, dbc_, SQL_ATTR_ASYNC_DBC_EVENT, event_handle, SQL_IS_POINTER);
            if (!success(rc))
                NANODBC_THROW_DATABASE_ERROR(dbc_, SQL_HANDLE_DBC);*/
        }

        void async_complete()
        {
            /*NANODBC_ASSERT(dbc_);

            RETCODE rc, arc;
            NANODBC_CALL_RC(SQLCompleteAsync, rc, SQL_HANDLE_DBC, dbc_, &arc);
            if (!success(rc) || !success(arc))
                NANODBC_THROW_DATABASE_ERROR(dbc_, SQL_HANDLE_DBC);

            connected_ = true;

            NANODBC_CALL_RC(
                SQLSetConnectAttr,
                rc,
                dbc_,
                SQL_ATTR_ASYNC_DBC_FUNCTIONS_ENABLE,
                (SQLPOINTER)SQL_ASYNC_DBC_ENABLE_OFF,
                SQL_IS_INTEGER);
            if (!success(rc))
                NANODBC_THROW_DATABASE_ERROR(dbc_, SQL_HANDLE_DBC);*/
        }

        RETCODE connect(
            [[maybe_unused]] const string& dsn,
            [[maybe_unused]] const string& user,
            [[maybe_unused]] const string& pass,
            [[maybe_unused]] long timeout,
            void* event_handle = nullptr)
        {
            allocate_env_handle(env_);
            disconnect();

            deallocate_handle(dbc_, SQL_HANDLE_DBC);
            allocate_dbc_handle(dbc_, env_);

            //RETCODE rc;
            //if (timeout != 0)
            //{
            //    // Avoid to set the timeout to 0 (no timeout).
            //    // This is a workaround for the Oracle ODBC Driver (11.1), as this
            //    // operation is not supported by the Driver.
            //    NANODBC_CALL_RC(
            //        SQLSetConnectAttr,
            //        rc,
            //        dbc_,
            //        SQL_LOGIN_TIMEOUT,
            //        (SQLPOINTER)(std::intptr_t)timeout,
            //        0);
            //    if (!success(rc))
            //        NANODBC_THROW_DATABASE_ERROR(dbc_, SQL_HANDLE_DBC);
            //}

#if !defined(NANODBC_DISABLE_ASYNC) && defined(SQL_ATTR_ASYNC_DBC_EVENT)
            if (event_handle != nullptr)
                enable_async(event_handle);
#endif

           /* NANODBC_CALL_RC(
                NANODBC_FUNC(SQLConnect),
                rc,
                dbc_,
                (NANODBC_SQLCHAR*)dsn.c_str(),
                SQL_NTS,
                !user.empty() ? (NANODBC_SQLCHAR*)user.c_str() : 0,
                SQL_NTS,
                !pass.empty() ? (NANODBC_SQLCHAR*)pass.c_str() : 0,
                SQL_NTS);
            if (!success(rc) && (event_handle == nullptr || rc != SQL_STILL_EXECUTING))
                NANODBC_THROW_DATABASE_ERROR(dbc_, SQL_HANDLE_DBC);

            connected_ = success(rc);*/
            RETCODE rc;
            return rc;
        }

        RETCODE
            connect([[maybe_unused]] const string& connection_string, long timeout, void* event_handle = nullptr)
        {
            allocate_env_handle(env_);
            disconnect();

            deallocate_handle(dbc_, SQL_HANDLE_DBC);
            allocate_dbc_handle(dbc_, env_);

            RETCODE rc;
            if (timeout != 0)
            {
                // Avoid to set the timeout to 0 (no timeout).
                // This is a workaround for the Oracle ODBC Driver (11.1), as this
                // operation is not supported by the Driver.
                /*NANODBC_CALL_RC(
                    SQLSetConnectAttr,
                    rc,
                    dbc_,
                    SQL_LOGIN_TIMEOUT,
                    (SQLPOINTER)(std::intptr_t)timeout,
                    0);
                if (!success(rc))
                    NANODBC_THROW_DATABASE_ERROR(dbc_, SQL_HANDLE_DBC);*/
            }

#if !defined(NANODBC_DISABLE_ASYNC) && defined(SQL_ATTR_ASYNC_DBC_EVENT)
            if (event_handle != nullptr)
                enable_async(event_handle);
#endif

            /*NANODBC_CALL_RC(
                NANODBC_FUNC(SQLDriverConnect),
                rc,
                dbc_,
                0,
                (NANODBC_SQLCHAR*)connection_string.c_str(),
                SQL_NTS,
                nullptr,
                0,
                nullptr,
                SQL_DRIVER_NOPROMPT);
            if (!success(rc) && (event_handle == nullptr || rc != SQL_STILL_EXECUTING))
                NANODBC_THROW_DATABASE_ERROR(dbc_, SQL_HANDLE_DBC);*/

            connected_ = success(rc);

            return rc;
        }

        bool connected() const { return connected_; }

        void disconnect()
        {
            if (connected())
            {
	            RETCODE rc = SQLDisconnect(dbc_);
                if (!success(rc))
                {
                    //NANODBC_THROW_DATABASE_ERROR(dbc_, SQL_HANDLE_DBC);
                }
                    
            }
            connected_ = false;
        }

        std::size_t transactions() const { return transactions_; }

        HDBC native_dbc_handle() const { return dbc_; }
        HENV native_env_handle() const { return env_; }

        template <class T>
        T get_info(short info_type) const
        {
            return get_info_impl<T>(info_type);
        }
        string dbms_name() const;
        string dbms_version() const;
        string driver_name() const;
        string database_name() const;
        string catalog_name() const
        {
            /*NANODBC_SQLCHAR name[SQL_MAX_OPTION_STRING_LENGTH] = { 0 };
            SQLINTEGER length(0);
            RETCODE rc;
            NANODBC_CALL_RC(
                NANODBC_FUNC(SQLGetConnectAttr),
                rc,
                dbc_,
                SQL_ATTR_CURRENT_CATALOG,
                name,
                sizeof(name) / sizeof(NANODBC_SQLCHAR),
                &length);
            if (!success(rc))
                NANODBC_THROW_DATABASE_ERROR(dbc_, SQL_HANDLE_DBC);
            return string(&name[0], &name[size(name)]);*/
        }

        std::size_t ref_transaction() { return ++transactions_; }

        std::size_t unref_transaction()
        {
            if (transactions_ > 0)
                --transactions_;
            return transactions_;
        }

        bool rollback() const { return rollback_; }

        void rollback(bool onoff) { rollback_ = onoff; }

    public:
        HSTMT stmt{ nullptr };

    private:
        template <class T>
        T get_info_impl(short info_type) const;

        HENV env_;
        HDBC dbc_;
        
        bool connected_;
        std::size_t transactions_;
        bool rollback_; // if true, this connection is marked for eventual transaction rollback
    };

	
	connection::connection()
		: impl_(new impl())
	{
	}

	connection::connection(const connection& rhs)
		: impl_(rhs.impl_)
	{
	}

	connection::connection(connection&& rhs) noexcept
		: impl_(std::move(rhs.impl_))
	{
	}

	connection& connection::operator=(connection rhs)
	{
		swap(rhs);
		return *this;
	}

	odbc::connection::~connection()
	{
		close();
	}

	bool connection::connect(const SQLHENV henv, const wchar_t* connection_string)
	{
		if (::SQLAllocHandle(SQL_HANDLE_DBC, henv, &impl_->stmt) != SQL_SUCCESS)
		{
			return false;
		}

		SQLWCHAR in_connect_str[1024] = { 0, };
		::wcscpy_s(in_connect_str, connection_string);

		SQLWCHAR out_connect_str[1024] = { 0, };
		SQLSMALLINT out_connect_str_len = 0;

		const SQLRETURN result = ::SQLDriverConnectW(impl_->native_dbc_handle(), nullptr,
		                                             in_connect_str, _countof(in_connect_str),
		                                             out_connect_str, _countof(out_connect_str), &out_connect_str_len,
		                                             SQL_DRIVER_NOPROMPT);

		if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO)
		{
			return false;
		}

		if (::SQLAllocHandle(SQL_HANDLE_STMT, impl_->native_dbc_handle(), &impl_->stmt) != SQL_SUCCESS)
		{
			return false;
		}

		return true;
	}

	void connection::close() const
	{
		if (impl_->native_env_handle() != nullptr)
		{
			::SQLFreeHandle(SQL_HANDLE_STMT, impl_->native_env_handle());
		}

		if (impl_->native_dbc_handle() != nullptr)
		{
			::SQLFreeHandle(SQL_HANDLE_DBC, impl_->native_dbc_handle());
		}
	}

    string connection::dbms_name() const
	{
		return L"";
	}

	template <class T>
	T connection::get_info(short info_type) const
	{
		return get_info_impl<T>(info_type);
	}
}

