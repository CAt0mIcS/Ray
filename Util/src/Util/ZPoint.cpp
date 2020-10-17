#include "utilpch.h"
#include "ZPoint.h"

#include <iostream>


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

