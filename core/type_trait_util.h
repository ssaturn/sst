#pragma once

#include <concepts>

#pragma warning(disable:5103)

namespace sst
{
	template<typename T>
	concept object_type = std::is_object_v<T>;

#define IS_MEMBER_OBJECT(member_name) \
	template<typename ClassTy> \
	concept is_member_object_##member_name = requires (ClassTy t) { \
		{ t.##member_name } -> std::same_as<int&>; \
	};

	IS_MEMBER_OBJECT(result_value);
}