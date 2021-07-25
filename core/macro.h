#pragma once


#define DISALLOW_SPECIAL_MEMBER_FUNCTIONS(__name__) \
	public: \
		__name__(const __name__&) = delete; \
		__name__& operator=(const __name__&) = delete; \
		__name__(__name__&&) = delete; \
		__name__& operator=(__name__&&) = delete; \
	private: 
