#include "vector3d.h"
#include "math.h"


namespace sst::math
{
	vector3d vector3d::x_axis(1.0f, 0.0f, 0.0f);
	vector3d vector3d::y_axis(0.0f, 1.0f, 0.0f);
	vector3d vector3d::z_axis(0.0f, 0.0f, 1.0f);
	vector3d vector3d::origin(0.0f, 0.0f, 0.0f);
	
	vector3d::vector3d(const float x, const float y, const float z): x(x), y(y), z(z)
	{
	}

	vector3d::vector3d(const vector3d& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	vector3d& vector3d::operator=(const vector3d& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;

		return *this;
	}

	vector3d::vector3d(vector3d&& other) noexcept
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	vector3d& vector3d::operator=(vector3d&& other) noexcept
	{
		x = other.x;
		y = other.y;
		z = other.z;
		
		return *this;
	}

	float vector3d::length() const
	{
		return sqrt(length_seq());
	}

	float vector3d::length_seq() const
	{
		return x * x + y * y + z * z;
	}

	bool vector3d::operator==(const vector3d& other) const
	{
		return (are_equal(other.x, x) && are_equal(other.y, y) && are_equal(other.z, z));
	}

	bool vector3d::operator!=(const vector3d& other) const
	{
		return !(*this == other);
	}

	bool vector3d::is_zero() const
	{
		return math::is_zero(x * x + y * y + z * z);
	}

	bool vector3d::is_unit() const
	{
		return math::is_zero(1.0f - x * x - y * y - z * z);
	}

	void vector3d::set(const float t_x, const float t_y, const float t_z)
	{
		x = t_x;
		y = t_y;
		z = t_z;
	}

	void vector3d::clean()
	{
		if (math::is_zero(x))
			x = 0.0f;
			
		if (math::is_zero(y))
			y = 0.0f;
			
		if (math::is_zero(z))
			z = 0.0f;
	}

	void vector3d::zero()
	{
		x = y = z = 0.f;
	}

	void vector3d::normalize()
	{
		if (const float len_sq = length_seq(); is_zero(len_sq))
		{
			zero();
		}
		else
		{
			const float factor = inv_sqrt(len_sq);
			x *= factor;
			y *= factor;
			z *= factor;
		}
	}

	vector3d vector3d::operator+(const vector3d& other) const
	{
		return vector3d(x + other.x, y + other.y, z + other.z);
	}

	vector3d vector3d::operator-(const vector3d& other) const
	{
		return vector3d(x - other.x, y - other.y, z - other.z);
	}

	vector3d vector3d::operator-() const
	{
		return vector3d(-x, -y, -z);
	}

	vector3d vector3d::operator*(const float scalar) const
	{
		return vector3d(scalar * x, scalar * y, scalar * z);
	}

	vector3d& vector3d::operator*=(const float scalar)
	{
		*this = this->operator*(scalar);
		
		return *this;
	}

	vector3d vector3d::operator/(const float scalar) const
	{
		return vector3d(x / scalar, y / scalar, z / scalar);
	}

	vector3d& vector3d::operator/=(const float scalar)
	{
		*this = this->operator/(scalar);

		return *this;
	}

	float vector3d::dot(const vector3d& vector) const
	{
		return (x * vector.x + y * vector.y + z * vector.z);
	}

	vector3d vector3d::cross(const vector3d& vector) const
	{
		return vector3d(y * vector.z - z * vector.y,
					   z * vector.x - x * vector.z,
					   x * vector.y - y * vector.x);
	}

	std::ostream& operator<<(std::ostream& out, const vector3d& source)
	{
		out << '<' << source.x << ',' << source.y << ',' << source.z << '>';

		return out;
	}

	float distance(const vector3d& p0, const vector3d& p1)
	{
		return sqrt(distance_seq(p0, p1));
	}

	float distance_seq(const vector3d& p0, const vector3d& p1)
	{
		const float x = p0.x - p1.x;
		const float y = p0.y - p1.y;
		const float z = p0.z - p1.z;

		return (x * x + y * y + z * z);
	}

	vector3d& operator+=(vector3d& vector, const vector3d& other)
	{
		vector = vector + other;
		return vector;
	}

	vector3d& operator-=(vector3d& vector, const vector3d& other)
	{
		vector = vector - other;
		return vector;
	}

	vector3d operator*(const float scalar, const vector3d& vector)
	{
		return vector * scalar;
	}

	float dot(const vector3d& vector1, const vector3d& vector2)
	{
		return vector1.dot(vector2);
	}

	vector3d cross(const vector3d& vector1, const vector3d& vector2)
	{
		return vector1.cross(vector2);
	}
}

