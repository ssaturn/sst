#pragma once


#define DISALLOW_SPECIAL_MEMBER_FUNCTIONS(__name__) \
	public: \
		__name__(const __name__&) = delete; \
		__name__& operator=(const __name__&) = delete; \
		__name__(__name__&&) = delete; \
		__name__& operator=(__name__&&) = delete; \
	private: 


#define SST_STRINGIZE_I(text) #text
#define SST_STRINGIZE(text) SST_STRINGIZE_I(text)

#define SST_CODECVT_TYPE std::codecvt_utf8_utf16
//#define NANODBC_CALL_RC(FUNC, RC, ...)                                                             \
//    do                                                                                             \
//    {                                                                                              \
//        std::cerr << __FILE__                                                                      \
//            ":" SST_STRINGIZE(__LINE__) " " SST_STRINGIZE(FUNC) "(" #__VA_ARGS__ ")"       \
//                  << std::endl;                                                                    \
//        RC = FUNC(__VA_ARGS__);                                                                    \
//    } while (false) /**/
//#define NANODBC_CALL(FUNC, ...)                                                                    \
//    do                                                                                             \
//    {                                                                                              \
//        std::cerr << __FILE__                                                                      \
//            ":" SST_STRINGIZE(__LINE__) " " SST_STRINGIZE(FUNC) "(" #__VA_ARGS__ ")"       \
//                  << std::endl;                                                                    \
//        FUNC(__VA_ARGS__);                                                                         \
//    } while (false) /**/
//#else
//#define NANODBC_CALL_RC(FUNC, RC, ...) RC = FUNC(__VA_ARGS__)
//#define NANODBC_CALL(FUNC, ...) FUNC(__VA_ARGS__)
//#endif