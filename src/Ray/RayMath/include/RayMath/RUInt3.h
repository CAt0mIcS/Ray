#pragma once

#include "RMathInternals.h"
#include "RUInt2.h"


namespace At0::Ray
{
	struct UInt3
	{
		uint32_t x, y, z;

		// --------------------------------------------------------------------
		// Constructors
		constexpr UInt3() : x(0), y(0), z(0) {}
		constexpr UInt3(uint32_t x) : x(x), y(0), z(0) {}
		constexpr UInt3(uint32_t x, uint32_t y) : x(x), y(y), z(0) {}
		constexpr UInt3(uint32_t x, uint32_t y, uint32_t z) : x(x), y(y), z(0) {}
		constexpr UInt3(const UInt2& xy, uint32_t z) : x(xy.x), y(xy.y), z(z) {}
		UInt3(const UInt3&) = default;
		UInt3(UInt3&&) = default;

		// --------------------------------------------------------------------
		// Operators
		UInt3& RAYMATH_CALLCONV operator=(const UInt3&) = default;
		UInt3& RAYMATH_CALLCONV operator=(UInt3&&) = default;

		UInt3 RAYMATH_CALLCONV operator+(const UInt3& other) const;
		UInt3 RAYMATH_CALLCONV operator-(const UInt3& other) const;
		UInt3 RAYMATH_CALLCONV operator*(const UInt3& other) const;
		UInt3 RAYMATH_CALLCONV operator/(const UInt3& other) const;

		UInt3& RAYMATH_CALLCONV operator+=(const UInt3& other);
		UInt3& RAYMATH_CALLCONV operator-=(const UInt3& other);
		UInt3& RAYMATH_CALLCONV operator*=(const UInt3& other);
		UInt3& RAYMATH_CALLCONV operator/=(const UInt3& other);

		UInt3& RAYMATH_CALLCONV operator++();
		UInt3& RAYMATH_CALLCONV operator--();
		UInt3 RAYMATH_CALLCONV operator++(int);
		UInt3 RAYMATH_CALLCONV operator--(int);

		bool RAYMATH_CALLCONV operator==(const UInt3& other) const;
		bool RAYMATH_CALLCONV operator!=(const UInt3& other) const;
		bool RAYMATH_CALLCONV operator>(const UInt3& other) const;
		bool RAYMATH_CALLCONV operator<(const UInt3& other) const;
		bool RAYMATH_CALLCONV operator>=(const UInt3& other) const;
		bool RAYMATH_CALLCONV operator<=(const UInt3& other) const;
	};

	// --------------------------------------------------------------------
	// Operators
	inline UInt3 UInt3::operator+(const UInt3& other) const
	{
		return { x + other.x, y + other.y, z + other.z };
	}

	inline UInt3 UInt3::operator-(const UInt3& other) const
	{
		return { x - other.x, y - other.y, z - other.z };
	}

	inline UInt3 UInt3::operator*(const UInt3& other) const
	{
		return { x * other.x, y * other.y, z * other.z };
	}

	inline UInt3 UInt3::operator/(const UInt3& other) const
	{
		return { x / other.x, y / other.y, z / other.z };
	}


	inline UInt3& UInt3::operator+=(const UInt3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	inline UInt3& UInt3::operator-=(const UInt3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	inline UInt3& UInt3::operator*=(const UInt3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	inline UInt3& UInt3::operator/=(const UInt3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}


	inline UInt3& UInt3::operator++()
	{
		*this += 1;
		return *this;
	}

	inline UInt3& UInt3::operator--()
	{
		*this -= 1;
		return *this;
	}

	inline UInt3 UInt3::operator++(int)
	{
		UInt3 result(*this);
		++* this;
		return result;
	}

	inline UInt3 UInt3::operator--(int)
	{
		UInt3 result(*this);
		--* this;
		return result;
	}

	inline bool UInt3::operator==(const UInt3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	inline bool UInt3::operator!=(const UInt3& other) const
	{
		return !(*this == other);
	}

	inline bool UInt3::operator>(const UInt3& other) const
	{
		return x > other.x && y > other.y && z > other.z;
	}

	inline bool UInt3::operator<(const UInt3& other) const
	{
		return x < other.x&& y < other.y&& z < other.z;
	}

	inline bool UInt3::operator>=(const UInt3& other) const
	{
		return !(*this < other);
	}

	inline bool UInt3::operator<=(const UInt3& other) const
	{
		return !(*this > other);
	}
}