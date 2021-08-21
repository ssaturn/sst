#include "../stdafx.h"
#include "error.h"

#include <sql.h>
#include <sqltypes.h>


namespace 
{
    std::string recent_error(SQLHANDLE handle, SQLSMALLINT handle_type, long& native, std::string& state)
    {
        std::wstring result{};
        std::string rvalue{};
        std::vector<SQLCHAR> sql_message(SQL_MAX_MESSAGE_LENGTH);
        sql_message[0] = '\0';

        SQLSMALLINT i = 1;
        SQLINTEGER native_error = 0;
        SQLSMALLINT total_bytes = 0;
        SQLCHAR sql_state[6] = { 0 };
        RETCODE rc;

        do
        {
            rc = SQLGetDiagRec(handle_type, handle, i, sql_state, &native_error, 0, 0, &total_bytes);
        	if (sst::database::success(rc) && total_bytes > 0)
                sql_message.resize(static_cast<std::size_t>(total_bytes) + 1);

            if (rc == SQL_NO_DATA)
                break;

			rc = SQLGetDiagRec(handle_type, handle, i, sql_state, &native_error, sql_message.data(), (SQLSMALLINT)sql_message.size(), &total_bytes);
            if (!sst::database::success(rc))
            {
                //convert(std::move(result), rvalue);
                return rvalue;
            }

            if (!result.empty())
                result += ' ';

            result += std::wstring(sql_message.begin(), sql_message.end());
            i++;

            // NOTE: unixODBC using PostgreSQL and SQLite drivers crash if you call SQLGetDiagRec()
            // more than once. So as a (terrible but the best possible) workaround just exit
            // this loop early on non-Windows systems.
#ifndef _MSC_VER
            break;
#endif
        } while (rc != SQL_NO_DATA);

        /*convert(std::move(result), rvalue);
        if (size(sql_state) > 0)
        {
            state.clear();
            state.reserve(size(sql_state) - 1);
            for (std::size_t idx = 0; idx != size(sql_state) - 1; ++idx)
            {
                state.push_back(static_cast<char>(sql_state[idx]));
            }
        }*/

        native = native_error;
        std::string status = state;
        status += ": ";
        status += rvalue;

        // some drivers insert \0 into error messages for unknown reasons
        using std::replace;
        replace(status.begin(), status.end(), '\0', ' ');

        return status;
    }
}

namespace sst::database
{
    /*error::error(SQLHANDLE handle, short handle_type, const std::string& info)
        : std::runtime_error(info)
        , sql_state_("00000")
    {
        message_ = std::string(std::runtime_error::what()) + recent_error(handle, handle_type, native_error_, sql_state_);
    }

    const char* error::what() const noexcept
    {
        return message_.c_str();
    }

    long error::native() const noexcept
    {
        return native_error_;
    }

    const std::string error::state() const noexcept
    {
        return sql_state_;
    }*/

}
