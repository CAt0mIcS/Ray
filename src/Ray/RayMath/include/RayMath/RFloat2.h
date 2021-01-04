#pragma once

#include "RMathInternals.h"


namespace At0::Ray
{
	struct Float2
	{
		float x, y;

		// --------------------------------------------------------------------
		// Constructors
		constexpr Float2() : x(0.0f), y(0.0f) {}
		constexpr Float2(float x, float y) : x(x), y(y) {}
		constexpr Float2(float x) : x(x), y(0.0f) {}
		Float2(const Float2&) = default;
		Float2(Float2&&) = default;

		// --------------------------------------------------------------------
		// Operators
		Float2& RAYMATH_CALLCONV operator=(const Float2&) = default;
		Float2& RAYMATH_CALLCONV operator=(Float2&&) = default;

		Float2 RAYMATH_CALLCONV operator+(const Float2& other) const;
		Float2 RAYMATH_CALLCONV operator-(const Float2& other) const;
		Float2 RAYMATH_CALLCONV operator*(const Float2& other) const;
		Float2 RAYMATH_CALLCONV operator/(const Float2& other) const;

		Float2& RAYMATH_CALLCONV operator+=(const Float2& other);
		Float2& RAYMATH_CALLCONV operator-=(const Float2& other);
		Float2& RAYMATH_CALLCONV operator*=(const Float2& other);
		Float2& RAYMATH_CALLCONV operator/=(const Float2& other);

		Float2& RAYMATH_CALLCONV operator++();
		Float2& RAYMATH_CALLCONV operator--();
		Float2 RAYMATH_CALLCONV operator++(int);
		Float2 RAYMATH_CALLCONV operator--(int);

		bool RAYMATH_CALLCONV operator==(const Float2& other) const;
		bool RAYMATH_CALLCONV operator!=(const Float2& other) const;
		bool RAYMATH_CALLCONV operator>(const Float2& other) const;
		bool RAYMATH_CALLCONV operator<(const Float2& other) const;
		bool RAYMATH_CALLCONV operator>=(const Float2& other) const;
		bool RAYMATH_CALLCONV operator<=(const Float2& other) const;
	};

	// --------------------------------------------------------------------
	// Operators
	inline Float2 Float2::operator+(const Float2& other) const
	{
		return { x + other.x, y + other.y };
	}

	inline Float2 Float2::operator-(const Float2& other) const
	{
		return { x - other.x, y - other.y };
	}

	inline Float2 Float2::operator*(const Float2& other) const
	{
		return { x * other.x, y * other.y };
	}

	inline Float2 Float2::operator/(const Float2& other) const
	{
		return { x / other.x, y / other.y };
	}


	inline Float2& Float2::operator+=(const Float2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	inline Float2& Float2::operator-=(const Float2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	inline Float2& Float2::operator*=(const Float2& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	inline Float2& Float2::operator/=(const Float2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}


	inline Float2& Float2::operator++()
	{
		*this += 1.0f;
		return *this;
	}

	inline Float2& Float2::operator--()
	{
		*this -= 1.0f;
		return *this;
	}

	inline Float2 Float2::operator++(int)
	{
		Float2 result(*this);
		++* this;
		return result;
	}

	inline Float2 Float2::operator--(int)
	{
		Float2 result(*this);
		--* this;
		return result;
	}


	inline bool Float2::operator==(const Float2& other) const
	{
		return x == other.x && y == other.y;
	}

	inline bool Float2::operator!=(const Float2& other) const
	{
		return !(*this == other);
	}

	inline bool Float2::operator>(const Float2& other) const
	{
		return x > other.x && y > other.y;
	}

	inline bool Float2::operator<(const Float2& other) const
	{
		return x < other.x&& y < other.y;
	}

	inline bool Float2::operator>=(const Float2& other) const
	{
		return !(*this < other);
	}

	inline bool Float2::operator<=(const Float2& other) const
	{
		return !(*this > other);
	}
}