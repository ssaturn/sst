#pragma once


namespace sst::trait_util
{
	//
	// https://stackoverflow.com/questions/1005476/how-to-detect-whether-there-is-a-specific-member-variable-in-class
	//
#define OBJECT_HAS_MEMBER(member_value) \
	template<typename T, typename = void> \
	struct object_has_member_##member_value : std::false_type {}; \
	\
	template<typename T> \
	struct object_has_member_##member_value<T, decltype(std::declval<T>().member_value, void())> : std::true_type {};

	OBJECT_HAS_MEMBER(result)

	template <typename T, typename = int>
	struct HasX : std::false_type { };

	template <typename T>
	struct HasX <T, decltype((void)T::x, 0)> : std::true_type { };

}