#include "rlpch.h"
#include "Vec2.h"

#include <iostream>


namespace At0::Reyal
{
	Vec2::Vec2(float xy) : x(xy), y(xy) {}

	Vec2::Vec2(float ix, float iy) : x(ix), y(iy) {}

	Vec2::Vec2() : x(0), y(0) {}

	Vec2 Vec2::operator+(const Vec2& other) const
	{
		Vec2 temp(*this);
		temp += other;
		return temp;
	}

	Vec2 Vec2::operator-(const Vec2& other) const
	{
		Vec2 temp(*this);
		temp -= other;
		return temp;
	}

	Vec2 Vec2::operator*(const Vec2& other) const
	{
		Vec2 temp(*this);
		temp *= other;
		return temp;
	}

	Vec2 Vec2::operator/(const Vec2& other) const
	{
		Vec2 temp(*this);
		temp /= other;
		return temp;
	}


	Vec2& Vec2::operator+=(const Vec2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2& Vec2::operator-=(const Vec2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vec2& Vec2::operator*=(const Vec2& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Vec2& Vec2::operator/=(const Vec2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}


	void Vec2::Normalize()
	{
		float length = sqrt(pow(x, 2)) + pow(y, 2);
	}

	Vec2 Vec2::Normalized() const
	{
		float lenght = sqrt(pow(x, 2) + pow(y, 2));
		return Vec2(x / lenght, y / lenght);
	}

	void Vec2::Rotate(float rotation)
	{
		float x1 = x;
		x = (x * cos(rotation)) - (y * sin(rotation));
		y = (x1 * sin(rotation)) - (y * cos(rotation));
	}

	Vec2 Vec2::Rotated(float rotation) const
	{
		return Vec2(
			(x * cos(rotation)) - (y * sin(rotation)),
			(x * sin(rotation)) + (y * cos(rotation))
			);
	}

	float Vec2::Absolute() const
	{
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	Vec2::operator D2D1_POINT_2F() const
	{
		return { x, y };
	}

	Vec2::operator D2D1_SIZE_F() const
	{
		return { x, y };
	}

	std::ostream& operator<<(std::ostream& os, const Vec2& vec)
	{
		os << "{x=" << vec.x << ", y=" << vec.y << "}";
		return os;
	}
	
	RL_API std::wostream& operator<<(std::wostream& os, const Vec2& vec)
	{
		os << L"{x=" << vec.x << L", y=" << vec.y << L"}";
		return os;
	}
}