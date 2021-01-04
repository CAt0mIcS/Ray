#pragma once

#include "RMathInternals.h"


namespace At0::Ray
{
	struct Int2
	{
		int32_t x, y;

		// --------------------------------------------------------------------
		// Constructors
		constexpr Int2() : x(0), y(0) {}
		constexpr Int2(int32_t x, int32_t y) : x(x), y(y) {}
		constexpr Int2(int32_t x) : x(x), y(0) {}
		Int2(const Int2&) = default;
		Int2(Int2&&) = default;

		// --------------------------------------------------------------------
		// Operators
		Int2& RAYMATH_CALLCONV operator=(const Int2&) = default;
		Int2& RAYMATH_CALLCONV operator=(Int2&&) = default;

		Int2 RAYMATH_CALLCONV operator+(const Int2& other) const;
		Int2 RAYMATH_CALLCONV operator-(const Int2& other) const;
		Int2 RAYMATH_CALLCONV operator*(const Int2& other) const;
		Int2 RAYMATH_CALLCONV operator/(const Int2& other) const;

		Int2& RAYMATH_CALLCONV operator+=(const Int2& other);
		Int2& RAYMATH_CALLCONV operator-=(const Int2& other);
		Int2& RAYMATH_CALLCONV operator*=(const Int2& other);
		Int2& RAYMATH_CALLCONV operator/=(const Int2& other);

		Int2& RAYMATH_CALLCONV operator++();
		Int2& RAYMATH_CALLCONV operator--();
		Int2 RAYMATH_CALLCONV operator++(int);
		Int2 RAYMATH_CALLCONV operator--(int);

		bool RAYMATH_CALLCONV operator==(const Int2& other) const;
		bool RAYMATH_CALLCONV operator!=(const Int2& other) const;
		bool RAYMATH_CALLCONV operator>(const Int2& other) const;
		bool RAYMATH_CALLCONV operator<(const Int2& other) const;
		bool RAYMATH_CALLCONV operator>=(const Int2& other) const;
		bool RAYMATH_CALLCONV operator<=(const Int2& other) const;
	};

	// --------------------------------------------------------------------
	// Operators
	inline Int2 Int2::operator+(const Int2& other) const
	{
		return { x + other.x, y + other.y };
	}

	inline Int2 Int2::operator-(const Int2& other) const
	{
		return { x - other.x, y - other.y };
	}

	inline Int2 Int2::operator*(const Int2& other) const
	{
		return { x * other.x, y * other.y };
	}

	inline Int2 Int2::operator/(const Int2& other) const
	{
		return { x / other.x, y / other.y };
	}


	inline Int2& Int2::operator+=(const Int2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	inline Int2& Int2::operator-=(const Int2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	inline Int2& Int2::operator*=(const Int2& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	inline Int2& Int2::operator/=(const Int2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}


	inline Int2& Int2::operator++()
	{
		*this += 1;
		return *this;
	}

	inline Int2& Int2::operator--()
	{
		*this -= 1;
		return *this;
	}

	inline Int2 Int2::operator++(int)
	{
		Int2 result(*this);
		++* this;
		return result;
	}

	inline Int2 Int2::operator--(int)
	{
		Int2 result(*this);
		--* this;
		return result;
	}


	inline bool Int2::operator==(const Int2& other) const
	{
		return x == other.x && y == other.y;
	}

	inline bool Int2::operator!=(const Int2& other) const
	{
		return !(*this == other);
	}

	inline bool Int2::operator>(const Int2& other) const
	{
		return x > other.x && y > other.y;
	}

	inline bool Int2::operator<(const Int2& other) const
	{
		return x < other.x&& y < other.y;
	}

	inline bool Int2::operator>=(const Int2& other) const
	{
		return !(*this < other);
	}

	inline bool Int2::operator<=(const Int2& other) const
	{
		return !(*this > other);
	}
}