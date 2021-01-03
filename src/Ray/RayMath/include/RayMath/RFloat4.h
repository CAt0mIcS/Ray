#pragma once

#include "RMathInternals.h"
#include "RFloat2.h"
#include "RFloat3.h"


namespace At0::Ray
{
	struct Float4
	{
		float x, y, z, w;

		// --------------------------------------------------------------------
		// Constructors
		constexpr Float4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		constexpr Float4(float x) : x(x), y(0.0f), z(0.0f), w(1.0f) {}
		constexpr Float4(float x, float y) : x(x), y(y), z(0.0f), w(1.0f) {}
		constexpr Float4(float x, float y, float z) : x(x), y(y), z(z), w(1.0f) {}
		constexpr Float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		constexpr Float4(const Float3& xyz) : x(xyz.x), y(xyz.y), z(xyz.z), w(1.0f) {}
		constexpr Float4(const Float3& xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
		constexpr Float4(const Float2& xy, const Float2& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
		Float4(const Float4&) = default;
		Float4(Float4&&) = default;

		// --------------------------------------------------------------------
		// Operators
		Float4& RAYMATH_CALLCONV operator=(const Float4&) = default;
		Float4& RAYMATH_CALLCONV operator=(Float4&&) = default;

		Float4 RAYMATH_CALLCONV operator+(const Float4& other) const;
		Float4 RAYMATH_CALLCONV operator-(const Float4& other) const;
		Float4 RAYMATH_CALLCONV operator*(const Float4& other) const;
		Float4 RAYMATH_CALLCONV operator/(const Float4& other) const;

		Float4& RAYMATH_CALLCONV operator+=(const Float4& other);
		Float4& RAYMATH_CALLCONV operator-=(const Float4& other);
		Float4& RAYMATH_CALLCONV operator*=(const Float4& other);
		Float4& RAYMATH_CALLCONV operator/=(const Float4& other);

		Float4& RAYMATH_CALLCONV operator++();
		Float4& RAYMATH_CALLCONV operator--();
		Float4 RAYMATH_CALLCONV operator++(int);
		Float4 RAYMATH_CALLCONV operator--(int);

		bool RAYMATH_CALLCONV operator==(const Float4& other) const;
		bool RAYMATH_CALLCONV operator!=(const Float4& other) const;
		bool RAYMATH_CALLCONV operator>(const Float4& other) const;
		bool RAYMATH_CALLCONV operator<(const Float4& other) const;
		bool RAYMATH_CALLCONV operator>=(const Float4& other) const;
		bool RAYMATH_CALLCONV operator<=(const Float4& other) const;
	};


	// RAY_TODO: Float4::w component should maybe be modified when adding/... Float4s
	// --------------------------------------------------------------------
	// Operators
	Float4 Float4::operator+(const Float4& other) const
	{
		return { x + other.x, y + other.y, z + other.z };
	}

	Float4 Float4::operator-(const Float4& other) const
	{
		return { x - other.x, y - other.y, z - other.z };
	}

	Float4 Float4::operator*(const Float4& other) const
	{
		return { x * other.x, y * other.y, z * other.z };
	}

	Float4 Float4::operator/(const Float4& other) const
	{
		return { x / other.x, y / other.y, z / other.z };
	}


	Float4& Float4::operator+=(const Float4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Float4& Float4::operator-=(const Float4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Float4& Float4::operator*=(const Float4& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Float4& Float4::operator/=(const Float4& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}


	Float4& Float4::operator++()
	{
		*this += 1.0f;
		return *this;
	}

	Float4& Float4::operator--()
	{
		*this -= 1.0f;
		return *this;
	}

	Float4 Float4::operator++(int)
	{
		Float4 result(*this);
		++* this;
		return result;
	}

	Float4 Float4::operator--(int)
	{
		Float4 result(*this);
		--* this;
		return result;
	}

	bool Float4::operator==(const Float4& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool Float4::operator!=(const Float4& other) const
	{
		return !(*this == other);
	}

	bool Float4::operator>(const Float4& other) const
	{
		return x > other.x && y > other.y && z > other.z;
	}

	bool Float4::operator<(const Float4& other) const
	{
		return x < other.x&& y < other.y&& z < other.z;
	}

	bool Float4::operator>=(const Float4& other) const
	{
		return !(*this < other);
	}

	bool Float4::operator<=(const Float4& other) const
	{
		return !(*this > other);
	}
}