#pragma once

#include "RMathInternals.h"
#include "RInt2.h"


namespace At0::Ray
{
	struct Int3
	{
		int32_t x, y, z;

		// --------------------------------------------------------------------
		// Constructors
		constexpr Int3() : x(0), y(0), z(0) {}
		constexpr Int3(int32_t x) : x(x), y(0), z(0) {}
		constexpr Int3(int32_t x, int32_t y) : x(x), y(y), z(0) {}
		constexpr Int3(int32_t x, int32_t y, int32_t z) : x(x), y(y), z(0) {}
		constexpr Int3(const Int2& xy, int32_t z) : x(xy.x), y(xy.y), z(z) {}
		Int3(const Int3&) = default;
		Int3(Int3&&) = default;

		// --------------------------------------------------------------------
		// Operators
		Int3& RAYMATH_CALLCONV operator=(const Int3&) = default;
		Int3& RAYMATH_CALLCONV operator=(Int3&&) = default;

		Int3 RAYMATH_CALLCONV operator+(const Int3& other) const;
		Int3 RAYMATH_CALLCONV operator-(const Int3& other) const;
		Int3 RAYMATH_CALLCONV operator*(const Int3& other) const;
		Int3 RAYMATH_CALLCONV operator/(const Int3& other) const;

		Int3& RAYMATH_CALLCONV operator+=(const Int3& other);
		Int3& RAYMATH_CALLCONV operator-=(const Int3& other);
		Int3& RAYMATH_CALLCONV operator*=(const Int3& other);
		Int3& RAYMATH_CALLCONV operator/=(const Int3& other);

		Int3& RAYMATH_CALLCONV operator++();
		Int3& RAYMATH_CALLCONV operator--();
		Int3 RAYMATH_CALLCONV operator++(int);
		Int3 RAYMATH_CALLCONV operator--(int);

		bool RAYMATH_CALLCONV operator==(const Int3& other) const;
		bool RAYMATH_CALLCONV operator!=(const Int3& other) const;
		bool RAYMATH_CALLCONV operator>(const Int3& other) const;
		bool RAYMATH_CALLCONV operator<(const Int3& other) const;
		bool RAYMATH_CALLCONV operator>=(const Int3& other) const;
		bool RAYMATH_CALLCONV operator<=(const Int3& other) const;
	};

	// --------------------------------------------------------------------
	// Operators
	inline Int3 Int3::operator+(const Int3& other) const
	{
		return { x + other.x, y + other.y, z + other.z };
	}

	inline Int3 Int3::operator-(const Int3& other) const
	{
		return { x - other.x, y - other.y, z - other.z };
	}

	inline Int3 Int3::operator*(const Int3& other) const
	{
		return { x * other.x, y * other.y, z * other.z };
	}

	inline Int3 Int3::operator/(const Int3& other) const
	{
		return { x / other.x, y / other.y, z / other.z };
	}


	inline Int3& Int3::operator+=(const Int3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	inline Int3& Int3::operator-=(const Int3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	inline Int3& Int3::operator*=(const Int3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	inline Int3& Int3::operator/=(const Int3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}


	inline Int3& Int3::operator++()
	{
		*this += 1;
		return *this;
	}

	inline Int3& Int3::operator--()
	{
		*this -= 1;
		return *this;
	}

	inline Int3 Int3::operator++(int)
	{
		Int3 result(*this);
		++* this;
		return result;
	}

	inline Int3 Int3::operator--(int)
	{
		Int3 result(*this);
		--* this;
		return result;
	}

	inline bool Int3::operator==(const Int3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	inline bool Int3::operator!=(const Int3& other) const
	{
		return !(*this == other);
	}

	inline bool Int3::operator>(const Int3& other) const
	{
		return x > other.x && y > other.y && z > other.z;
	}

	inline bool Int3::operator<(const Int3& other) const
	{
		return x < other.x&& y < other.y&& z < other.z;
	}

	inline bool Int3::operator>=(const Int3& other) const
	{
		return !(*this < other);
	}

	inline bool Int3::operator<=(const Int3& other) const
	{
		return !(*this > other);
	}

}