#pragma once

#include <memory>
#include <sqltypes.h>
#include <string>
#include <vector>


namespace sst::database
{
	class db_param_base
	{
	protected:
		explicit db_param_base(const SQLUSMALLINT index)
			: index_(index)
		{
			
		}

		virtual ~db_param_base() = default;

		virtual SQLLEN* db_value() = 0;
		virtual SQLLEN db_value_max() = 0;
		virtual SQLPOINTER rgb_value() = 0;
		virtual SQLSMALLINT* c_type() = 0;
		virtual SQLSMALLINT* sql_type() = 0;

		SQLUSMALLINT index_{ 0 };
	};

	template<typename DataTy>
	class db_param : public db_param_base
	{
	public:
		explicit db_param(SQLUSMALLINT index, const std::wstring& name, DataTy* data);
		

	private:
		std::wstring name_{};
		DataTy value_;
		SQLLEN cb_value_{ 0 };
	};

	template <typename DataTy>
	db_param<DataTy>::db_param(const SQLUSMALLINT index, const std::wstring& name, DataTy* data)
		: db_param_base(index)
		, name_(name)
	{
		value_ = *data;
	}

	class db_param_table : public db_param_base
	{

		std::vector<std::shared_ptr<db_param_base>> params_;
	};


	template<typename DataTy>
	class db_param_array : public db_param_base
	{
		
	};

	template<typename DataTy>
	class db_param_vector : public db_param_base
	{

	};

	
}
