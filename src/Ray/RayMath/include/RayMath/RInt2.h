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
		Int2& operator=(const Int2&) = default;
		Int2& operator=(Int2&&) = default;

		Int2 operator+(const Int2& other) const;
		Int2 operator-(const Int2& other) const;
		Int2 operator*(const Int2& other) const;
		Int2 operator/(const Int2& other) const;

		Int2& operator+=(const Int2& other);
		Int2& operator-=(const Int2& other);
		Int2& operator*=(const Int2& other);
		Int2& operator/=(const Int2& other);

		Int2& operator++();
		Int2& operator--();
		Int2 operator++(int);
		Int2 operator--(int);

		bool operator==(const Int2& other) const;
		bool operator!=(const Int2& other) const;
		bool operator>(const Int2& other) const;
		bool operator<(const Int2& other) const;
		bool operator>=(const Int2& other) const;
		bool operator<=(const Int2& other) const;
	};

	// --------------------------------------------------------------------
	// Operators
	Int2 Int2::operator+(const Int2& other) const
	{
		return { x + other.x, y + other.y };
	}

	Int2 Int2::operator-(const Int2& other) const
	{
		return { x - other.x, y - other.y };
	}

	Int2 Int2::operator*(const Int2& other) const
	{
		return { x * other.x, y * other.y };
	}

	Int2 Int2::operator/(const Int2& other) const
	{
		return { x / other.x, y / other.y };
	}


	Int2& Int2::operator+=(const Int2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Int2& Int2::operator-=(const Int2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Int2& Int2::operator*=(const Int2& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Int2& Int2::operator/=(const Int2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}


	Int2& Int2::operator++()
	{
		*this += 1;
		return *this;
	}

	Int2& Int2::operator--()
	{
		*this -= 1;
		return *this;
	}

	Int2 Int2::operator++(int)
	{
		Int2 result(*this);
		++* this;
		return result;
	}

	Int2 Int2::operator--(int)
	{
		Int2 result(*this);
		--* this;
		return result;
	}


	bool Int2::operator==(const Int2& other) const
	{
		return x == other.x && y == other.y;
	}

	bool Int2::operator!=(const Int2& other) const
	{
		return !(*this == other);
	}

	bool Int2::operator>(const Int2& other) const
	{
		return x > other.x && y > other.y;
	}

	bool Int2::operator<(const Int2& other) const
	{
		return x < other.x&& y < other.y;
	}

	bool Int2::operator>=(const Int2& other) const
	{
		return !(*this < other);
	}

	bool Int2::operator<=(const Int2& other) const
	{
		return !(*this > other);
	}
}