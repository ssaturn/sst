#pragma once

#include <corecrt_math.h>


namespace sst::math
{
	constexpr float k_epsilon = 1.0e-6f;
	constexpr float k_pi = 3.1415926535897932384626433832795f;
	constexpr float k_half_pi = 1.5707963267948966192313216916398f;
	constexpr float k_two_pi = 2.0f * k_pi;
	
	inline float sqrt(const float val) { return sqrtf(val); }
	inline float inv_sqrt(const float val) { return 1.0f / sqrtf(val); }
	inline float abs(const float f) { return fabsf(f); }
	
	inline bool is_zero(const float a)
	{
		return (fabsf(a) < k_epsilon);
	}
	
	inline bool are_equal(const float a, const float b)
	{
		return (is_zero(a - b));
	}

	inline float sin(const float a)
	{
		return sinf(a);
	}
	
	inline float cos(const float a)
	{
		return cosf(a);
	}

	inline float tan(const float a)
	{
		return tanf(a);
	}
	
	inline void sin_cos(const float a, float& sina, float& cosa)
	{
		sina = sinf(a);
		cosa = cosf(a);
	}
	//extern void fast_sin_cos(float a, float& sina, float& cosa);

	inline float degree_to_radian(const float a)
	{
		return (a / 180.0f) * k_pi;
	}

	inline float radian_to_degree(const float r)
	{
		return (r / k_pi) * 180;
	}
	

}
