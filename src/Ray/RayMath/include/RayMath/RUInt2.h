#pragma once

#include "RMathInternals.h"


namespace At0::Ray
{
	struct UInt2
	{
		uint32_t x, y;

		// --------------------------------------------------------------------
		// Constructors
		constexpr UInt2() : x(0u), y(0u) {}
		constexpr UInt2(uint32_t x, uint32_t y) : x(x), y(y) {}
		constexpr UInt2(uint32_t x) : x(x), y(0u) {}
		UInt2(const UInt2&) = default;
		UInt2(UInt2&&) = default;

		// --------------------------------------------------------------------
		// Operators
		UInt2& RAYMATH_CALLCONV operator=(const UInt2&) = default;
		UInt2& RAYMATH_CALLCONV operator=(UInt2&&) = default;

		UInt2 RAYMATH_CALLCONV operator+(const UInt2& other) const;
		UInt2 RAYMATH_CALLCONV operator-(const UInt2& other) const;
		UInt2 RAYMATH_CALLCONV operator*(const UInt2& other) const;
		UInt2 RAYMATH_CALLCONV operator/(const UInt2& other) const;

		UInt2& RAYMATH_CALLCONV operator+=(const UInt2& other);
		UInt2& RAYMATH_CALLCONV operator-=(const UInt2& other);
		UInt2& RAYMATH_CALLCONV operator*=(const UInt2& other);
		UInt2& RAYMATH_CALLCONV operator/=(const UInt2& other);

		UInt2& RAYMATH_CALLCONV operator++();
		UInt2& RAYMATH_CALLCONV operator--();
		UInt2 RAYMATH_CALLCONV operator++(int);
		UInt2 RAYMATH_CALLCONV operator--(int);

		bool RAYMATH_CALLCONV operator==(const UInt2& other) const;
		bool RAYMATH_CALLCONV operator!=(const UInt2& other) const;
		bool RAYMATH_CALLCONV operator>(const UInt2& other) const;
		bool RAYMATH_CALLCONV operator<(const UInt2& other) const;
		bool RAYMATH_CALLCONV operator>=(const UInt2& other) const;
		bool RAYMATH_CALLCONV operator<=(const UInt2& other) const;
	};


	// --------------------------------------------------------------------
	// Operators
	UInt2 UInt2::operator+(const UInt2& other) const
	{
		return { x + other.x, y + other.y };
	}

	UInt2 UInt2::operator-(const UInt2& other) const
	{
		return { x - other.x, y - other.y };
	}

	UInt2 UInt2::operator*(const UInt2& other) const
	{
		return { x * other.x, y * other.y };
	}

	UInt2 UInt2::operator/(const UInt2& other) const
	{
		return { x / other.x, y / other.y };
	}


	UInt2& UInt2::operator+=(const UInt2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	UInt2& UInt2::operator-=(const UInt2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	UInt2& UInt2::operator*=(const UInt2& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	UInt2& UInt2::operator/=(const UInt2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}


	UInt2& UInt2::operator++()
	{
		*this += 1;
		return *this;
	}

	UInt2& UInt2::operator--()
	{
		*this -= 1;
		return *this;
	}

	UInt2 UInt2::operator++(int)
	{
		UInt2 result(*this);
		++* this;
		return result;
	}

	UInt2 UInt2::operator--(int)
	{
		UInt2 result(*this);
		--* this;
		return result;
	}


	bool UInt2::operator==(const UInt2& other) const
	{
		return x == other.x && y == other.y;
	}

	bool UInt2::operator!=(const UInt2& other) const
	{
		return !(*this == other);
	}

	bool UInt2::operator>(const UInt2& other) const
	{
		return x > other.x && y > other.y;
	}

	bool UInt2::operator<(const UInt2& other) const
	{
		return x < other.x&& y < other.y;
	}

	bool UInt2::operator>=(const UInt2& other) const
	{
		return !(*this < other);
	}

	bool UInt2::operator<=(const UInt2& other) const
	{
		return !(*this > other);
	}

}