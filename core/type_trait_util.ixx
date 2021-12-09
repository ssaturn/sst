export module type_trait_util;




export void MyFunc();

#define HAS_MEMBER_FUNC_ARGS(name) \
	template<class T, class Sig, class = void>struct has_member_func_##name :std::false_type {}; \
	template<class T, class R, class...Args> \
	struct has_member_func_##name<T, R(Args...), \
std::enable_if_t<std::is_convertible_v<decltype(T::##name(std::declval<Args>()...)), R> \
		&& !std::is_same_v<R, void>>> : std::true_type {}; \
	template<class T, class...Args> \
	struct has_member_func_##name<T, void(Args...), \
		decltype(void(T::##name(std::declval<Args>()...))) \
	> : std::true_type {};

