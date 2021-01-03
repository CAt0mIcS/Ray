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
		UInt3& operator=(const UInt3&) = default;
		UInt3& operator=(UInt3&&) = default;

		UInt3 operator+(const UInt3& other) const;
		UInt3 operator-(const UInt3& other) const;
		UInt3 operator*(const UInt3& other) const;
		UInt3 operator/(const UInt3& other) const;

		UInt3& operator+=(const UInt3& other);
		UInt3& operator-=(const UInt3& other);
		UInt3& operator*=(const UInt3& other);
		UInt3& operator/=(const UInt3& other);

		UInt3& operator++();
		UInt3& operator--();
		UInt3 operator++(int);
		UInt3 operator--(int);

		bool operator==(const UInt3& other) const;
		bool operator!=(const UInt3& other) const;
		bool operator>(const UInt3& other) const;
		bool operator<(const UInt3& other) const;
		bool operator>=(const UInt3& other) const;
		bool operator<=(const UInt3& other) const;
	};

	// --------------------------------------------------------------------
	// Operators
	UInt3 UInt3::operator+(const UInt3& other) const
	{
		return { x + other.x, y + other.y, z + other.z };
	}

	UInt3 UInt3::operator-(const UInt3& other) const
	{
		return { x - other.x, y - other.y, z - other.z };
	}

	UInt3 UInt3::operator*(const UInt3& other) const
	{
		return { x * other.x, y * other.y, z * other.z };
	}

	UInt3 UInt3::operator/(const UInt3& other) const
	{
		return { x / other.x, y / other.y, z / other.z };
	}


	UInt3& UInt3::operator+=(const UInt3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	UInt3& UInt3::operator-=(const UInt3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	UInt3& UInt3::operator*=(const UInt3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	UInt3& UInt3::operator/=(const UInt3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}


	UInt3& UInt3::operator++()
	{
		*this += 1;
		return *this;
	}

	UInt3& UInt3::operator--()
	{
		*this -= 1;
		return *this;
	}

	UInt3 UInt3::operator++(int)
	{
		UInt3 result(*this);
		++* this;
		return result;
	}

	UInt3 UInt3::operator--(int)
	{
		UInt3 result(*this);
		--* this;
		return result;
	}

	bool UInt3::operator==(const UInt3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool UInt3::operator!=(const UInt3& other) const
	{
		return !(*this == other);
	}

	bool UInt3::operator>(const UInt3& other) const
	{
		return x > other.x && y > other.y && z > other.z;
	}

	bool UInt3::operator<(const UInt3& other) const
	{
		return x < other.x&& y < other.y&& z < other.z;
	}

	bool UInt3::operator>=(const UInt3& other) const
	{
		return !(*this < other);
	}

	bool UInt3::operator<=(const UInt3& other) const
	{
		return !(*this > other);
	}
}