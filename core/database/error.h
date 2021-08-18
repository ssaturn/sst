#pragma once

#include <sql.h>
#include <stdexcept>




namespace sst::database
{
	// MASK : success
	inline bool success(const RETCODE rc)
	{
#ifdef SST_ODBC_API_DEBUG
		std::cerr << "<-- rc: " << return_code(rc) << " | " << std::endl;
#endif
		return rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO;
	}
	
	class database_error : public std::runtime_error
	{
	public:
		/// \brief Creates runtime_error with message about last ODBC error.
		/// \param handle The native ODBC statement or connection handle.
		/// \param handle_type The native ODBC handle type code for the given handle.
		/// \param info Additional info that will be appended to the beginning of the error message.
		database_error(void* handle, short handle_type, const std::string& info = "");
		const char* what() const noexcept;
		long native() const noexcept;
		const std::string state() const noexcept;

	private:
		long native_error_{ 0 };
		std::string sql_state_{};
		/**
		 * \brief 
		 */
		std::string message_{};
	};

	class type_incompatible_error : public std::runtime_error
	{
	public:
		type_incompatible_error();
		const char* what() const noexcept override;
	};

	/// \brief Accessed null data.
	/// \see exceptions
	class null_access_error : public std::runtime_error
	{
	public:
		null_access_error();
		const char* what() const noexcept override;
	};

	/// \brief Index out of range.
	/// \see exceptions
	class index_range_error : public std::runtime_error
	{
	public:
		index_range_error();
		const char* what() const noexcept override;
	};

	/// \brief Programming logic error.
	/// \see exceptions
	class programming_error : public std::runtime_error
	{
	public:
		explicit programming_error(const std::string& info);
		const char* what() const noexcept override;
	};

	
	
}
