#pragma once

#include "RMathInternals.h"
#include "RFloat2.h"


namespace At0::Ray
{
	struct Float3
	{
		float x, y, z;

		// --------------------------------------------------------------------
		// Constructors
		constexpr Float3() : x(0.0f), y(0.0f), z(0.0f) {}
		constexpr Float3(float x) : x(x), y(0.0f), z(0.0f) {}
		constexpr Float3(float x, float y) : x(x), y(y), z(0.0f) {}
		constexpr Float3(float x, float y, float z) : x(x), y(y), z(0.0f) {}
		constexpr Float3(const Float2& xy, float z) : x(xy.x), y(xy.y), z(z) {}
		constexpr Float3(const Float2& xy) : x(xy.x), y(xy.y), z(0.0f) {}
		Float3(const Float3&) = default;
		Float3(Float3&&) = default;

		// --------------------------------------------------------------------
		// Operators
		Float3& RAYMATH_CALLCONV operator=(const Float3&) = default;
		Float3& RAYMATH_CALLCONV operator=(Float3&&) = default;

		Float3 RAYMATH_CALLCONV operator+(const Float3& other) const;
		Float3 RAYMATH_CALLCONV operator-(const Float3& other) const;
		Float3 RAYMATH_CALLCONV operator*(const Float3& other) const;
		Float3 RAYMATH_CALLCONV operator/(const Float3& other) const;

		Float3& RAYMATH_CALLCONV operator+=(const Float3& other);
		Float3& RAYMATH_CALLCONV operator-=(const Float3& other);
		Float3& RAYMATH_CALLCONV operator*=(const Float3& other);
		Float3& RAYMATH_CALLCONV operator/=(const Float3& other);

		Float3& RAYMATH_CALLCONV operator++();
		Float3& RAYMATH_CALLCONV operator--();
		Float3 RAYMATH_CALLCONV operator++(int);
		Float3 RAYMATH_CALLCONV operator--(int);

		bool RAYMATH_CALLCONV operator==(const Float3& other) const;
		bool RAYMATH_CALLCONV operator!=(const Float3& other) const;
		bool RAYMATH_CALLCONV operator>(const Float3& other) const;
		bool RAYMATH_CALLCONV operator<(const Float3& other) const;
		bool RAYMATH_CALLCONV operator>=(const Float3& other) const;
		bool RAYMATH_CALLCONV operator<=(const Float3& other) const;
	};

	// --------------------------------------------------------------------
	// Operators
	Float3 Float3::operator+(const Float3& other) const
	{
		return { x + other.x, y + other.y, z + other.z };
	}

	Float3 Float3::operator-(const Float3& other) const
	{
		return { x - other.x, y - other.y, z - other.z };
	}

	Float3 Float3::operator*(const Float3& other) const
	{
		return { x * other.x, y * other.y, z * other.z };
	}

	Float3 Float3::operator/(const Float3& other) const
	{
		return { x / other.x, y / other.y, z / other.z };
	}


	Float3& Float3::operator+=(const Float3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Float3& Float3::operator-=(const Float3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Float3& Float3::operator*=(const Float3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Float3& Float3::operator/=(const Float3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}


	Float3& Float3::operator++()
	{
		*this += 1.0f;
		return *this;
	}

	Float3& Float3::operator--()
	{
		*this -= 1.0f;
		return *this;
	}

	Float3 Float3::operator++(int)
	{
		Float3 result(*this);
		++* this;
		return result;
	}

	Float3 Float3::operator--(int)
	{
		Float3 result(*this);
		--* this;
		return result;
	}

	bool Float3::operator==(const Float3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool Float3::operator!=(const Float3& other) const
	{
		return !(*this == other);
	}

	bool Float3::operator>(const Float3& other) const
	{
		return x > other.x && y > other.y && z > other.z;
	}

	bool Float3::operator<(const Float3& other) const
	{
		return x < other.x&& y < other.y&& z < other.z;
	}

	bool Float3::operator>=(const Float3& other) const
	{
		return !(*this < other);
	}

	bool Float3::operator<=(const Float3& other) const
	{
		return !(*this > other);
	}

}