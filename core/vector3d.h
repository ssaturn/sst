#pragma once

#include "common.h"


namespace sst::math
{
	class vector3d
	{
	public:
		vector3d() = default;
		explicit vector3d(float x, float y, float z);
		vector3d(const vector3d& other);
		vector3d& operator=(const vector3d& other);
		vector3d(vector3d&& other) noexcept;
		vector3d& operator=(vector3d&& other) noexcept;
		~vector3d() = default;

		friend std::ostream& operator<<(std::ostream& out, const vector3d& source);

		// accessors
		float& operator[](const uint32 i) { return (&x)[i]; }
		float operator[](const uint32 i) const { return (&x)[i]; }

		[[nodiscard]] float length() const;
		[[nodiscard]] float length_seq() const;
		friend float distance(const vector3d& p0, const vector3d& p1);
		friend float distance_seq(const vector3d& p0, const vector3d& p1);

		// comparison
		bool operator==(const vector3d& other) const;
		bool operator!=(const vector3d& other) const;
		[[nodiscard]] bool is_zero() const;
		[[nodiscard]] bool is_unit() const;

		// manipulators
		void set(float t_x, float t_y, float t_z);
		void clean();       // sets near-zero elements to 0
		void zero();		// sets all elements to 0
		void normalize();   // sets to unit vector

		 // addition/subtraction
		vector3d operator+(const vector3d& other) const;
		vector3d operator-(const vector3d& other) const;
		vector3d operator-() const;
		friend vector3d& operator+=(vector3d& vector, const vector3d& other);
		friend vector3d& operator-=(vector3d& vector, const vector3d& other);

		// scalar multiplication
		vector3d  operator*(float scalar) const;
		vector3d& operator*=(float scalar);
		vector3d  operator/(float scalar) const;
		vector3d& operator/=(float scalar);
		friend vector3d operator*(float scalar, const vector3d& vector);

		// dot product/cross product
		[[nodiscard]] float dot(const vector3d& vector) const;
		[[nodiscard]] vector3d cross(const vector3d& vector) const;
		friend float dot(const vector3d& vector1, const vector3d& vector2);
		friend vector3d cross(const vector3d& vector1, const vector3d& vector2);

		// useful defaults
		static vector3d x_axis;
		static vector3d y_axis;
		static vector3d z_axis;
		static vector3d origin;
		
		float x{ 0 };
		float y{ 0 };
		float z{ 0 };
	};

}
