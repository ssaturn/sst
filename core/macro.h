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
#define SST_VSTR(text) (#text) << L":" << (text)

#define SST_CODECVT_TYPE std::codecvt_utf8_utf16
