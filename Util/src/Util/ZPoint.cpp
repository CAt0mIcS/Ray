#include "utilpch.h"
#include "ZPoint.h"

#include <iostream>

#if 0


namespace Zeal::Util
{
	template<typename T>
	Vec2<T>::Vec2(T xy) : x(xy), y(xy) {}

	template<typename T>
	Vec2<T>::Vec2(T ix, T iy) : x(ix), y(iy) {}
	
	template<typename T>
	Vec2<T>::Vec2() : x(0), y(0) {}

	template<typename T>
	Vec2<T> Vec2<T>::operator+(const Vec2<T>& other) const
	{
		Vec2<T> temp(*this);
		temp += other;
		return temp;
	}

	template<typename T>
	Vec2<T> Vec2<T>::operator-(const Vec2<T>& other) const
	{
		Vec2<T> temp(*this);
		temp -= other;
		return temp;
	}

	template<typename T>
	Vec2<T> Vec2<T>::operator*(const Vec2<T>& other) const
	{
		Vec2<T> temp(*this);
		temp *= other;
		return temp;
	}

	template<typename T>
	Vec2<T> Vec2<T>::operator/(const Vec2<T>& other) const
	{
		Vec2<T> temp(*this);
		temp /= other;
		return temp;
	}


	template<typename T>
	Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	template<typename T>
	Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template<typename T>
	Vec2<T>& Vec2<T>::operator*=(const Vec2<T>& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	template<typename T>
	Vec2<T>& Vec2<T>::operator/=(const Vec2<T>& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}


	template<typename T>
	void Vec2<T>::Normalize()
	{
		T length = sqrt(pow(x, 2)) + pow(y, 2);
	}

	template<typename T>
	Vec2<T> Vec2<T>::Normalized() const
	{
		T lenght = sqrt(pow(x, 2) + pow(y, 2));
		return Vec2<T>(x / lenght, y / lenght);
	}

	template<typename T>
	void Vec2<T>::Rotate(T rotation)
	{
		T x1 = x;
		x = (x * cos(rotation)) - (y * sin(rotation));
		y = (x1 * sin(rotation)) - (y * cos(rotation));
	}

	template<typename T>
	Vec2<T> Vec2<T>::Rotated(T rotation) const
	{
		return Vec2<T>(
			(x * cos(rotation)) - (y * sin(rotation)),
			(x * sin(rotation)) + (y * cos(rotation))
			);
	}

	template<typename T>
	T Vec2<T>::Absolute() const
	{
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Vec2<T>& vec)
	{
		os << "{x=" << vec.x << ", y=" << vec.y << "}";
		return os;
	}


	/// <summary>
	/// Compile struct for float and double
	/// </summary>
	template struct Vec2<float>;
	template struct Vec2<double>;

	/// <summary>
	/// Add output operators for float and double
	/// </summary>
	template std::ostream& operator<<(std::ostream& os, const Vec2<float>& vec);
	template std::ostream& operator<<(std::ostream& os, const Vec2<double>& vec);
}

#else

namespace Zeal::Util
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

	std::ostream& operator<<(std::ostream& os, const Vec2& vec)
	{
		os << "{x=" << vec.x << ", y=" << vec.y << "}";
		return os;
	}
}

#endif