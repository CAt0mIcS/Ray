#include "Ray/Rpch.h"
#include "Ray/Core/RRect.h"

#include <iostream>

namespace At0::Ray
{
	RRect::RRect(float ltrb)
		: left(ltrb), top(ltrb), right(ltrb), bottom(ltrb)
	{

	}

	RRect::RRect(float ileft, float itop, float iright, float ibottom)
		: left(ileft), top(itop), right(iright), bottom(ibottom)
	{

	}

	RRect::RRect()
		: left(0), top(0), right(0), bottom(0)
	{

	}

	RRect RRect::operator+(const RRect& other) const
	{
		RRect temp(*this);
		temp += other;
		return temp;
	}

	RRect RRect::operator-(const RRect& other) const
	{
		RRect temp(*this);
		temp -= other;
		return temp;
	}

	RRect RRect::operator*(const RRect& other) const
	{
		RRect temp(*this);
		temp *= other;
		return temp;
	}

	RRect RRect::operator/(const RRect& other) const
	{
		RRect temp(*this);
		temp /= other;
		return temp;
	}


	RRect& RRect::operator+=(const RRect& other)
	{
		left += other.left;
		top += other.top;
		right += other.right;
		bottom += other.bottom;
		return *this;
	}

	RRect& RRect::operator-=(const RRect& other)
	{
		left -= other.left;
		top -= other.top;
		right -= other.right;
		bottom -= other.bottom;
		return *this;
	}

	RRect& RRect::operator*=(const RRect& other)
	{
		left *= other.left;
		top *= other.top;
		right *= other.right;
		bottom *= other.bottom;
		return *this;
	}

	RRect& RRect::operator/=(const RRect& other)
	{
		left /= other.left;
		top /= other.top;
		right /= other.right;
		bottom /= other.bottom;
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const RRect& rect)
	{
		os << "{left=" << rect.left << ", top=" << rect.top << ", right=" << rect.right << ", bottom=" << rect.bottom << "}";
		return os;
	}

}
