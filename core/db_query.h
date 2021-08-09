#pragma once

#include "common.h"
#include "db_param.h"
#include <vector>


namespace sst::database
{
	/*
	 * db_query query(L"usp_select_item");
	 * query.bind_input_parameter(&a);
	 * query.bind_output_parameter(&b);
	 * db_connector->run(db_query);
	 */
	class db_query
	{
	public:
		explicit db_query(const std::wstring& name)
			: name_(name)
		{
			
		}

		template<typename ParamTy>
		void bind_input_parameter(const std::wstring& name, ParamTy* value)
		{
			params_.push_back(std::make_shared<db_param<ParamTy>>(index_, name, value));
			index_++;
		}

		template<typename ParamTy>
		void bind_input_table_parameter(const std::wstring& name, ParamTy* value)
		{
			params_.push_back(std::make_shared<db_param<ParamTy>>(index_, name, value));
			index_++;
		}

		template<typename ParamTy>
		void bind_output_parameter(const std::wstring& name, ParamTy* value)
		{
			params_.push_back(std::make_shared<db_param<ParamTy>>(index_, name, value));
			index_++;
		}

	private:
		int32 index_{ 0 };
		std::wstring name_{};
		std::vector<std::shared_ptr<db_param_base>> params_{};
	};
}
