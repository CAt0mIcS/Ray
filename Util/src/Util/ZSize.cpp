#include "utilpch.h"
#include "ZSize.h"


namespace Zeal::Util
{
	Size::Size(float wh)
		: width(wh), height(wh)
	{
	}

	Size::Size(float iw, float ih)
		: width(iw), height(ih)
	{
	}

	Size::Size()
		: width(0), height(0)
	{
	}

	Size Size::operator+(const Size& other) const
	{
		Size temp(*this);
		temp += other;
		return temp;
	}

	Size Size::operator-(const Size& other) const
	{
		Size temp(*this);
		temp -= other;
		return temp;
	}

	Size Size::operator*(const Size& other) const
	{
		Size temp(*this);
		temp *= other;
		return temp;
	}

	Size Size::operator/(const Size& other) const
	{
		Size temp(*this);
		temp /= other;
		return temp;
	}


	Size& Size::operator+=(const Size& other)
	{
		width += other.width;
		height += other.height;
		return *this;
	}

	Size& Size::operator-=(const Size& other)
	{
		width -= other.width;
		height -= other.height;
		return *this;
	}

	Size& Size::operator*=(const Size& other)
	{
		width *= other.width;
		height *= other.height;
		return *this;
	}

	Size& Size::operator/=(const Size& other)
	{
		width /= other.width;
		height /= other.height;
		return *this;
	}
	
	Size::operator D2D::PointF() const
	{
		return { width, height };
	}

	Size::operator D2D::SizeF() const
	{
		return { width, height };
	}

	std::ostream& operator<<(std::ostream& os, const Size& vec)
	{
		os << "{width=" << vec.height << ", height=" << vec.height << "}";
		return os;
	}
}


