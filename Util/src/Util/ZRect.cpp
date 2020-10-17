#include "utilpch.h"
#include "ZRect.h"

#include <iostream>


namespace Zeal::Util
{
	template<typename T>
	ZRect<T>::ZRect(T ltrb)
		: left(ltrb), top(ltrb), right(ltrb), bottom(ltrb)
	{

	}

	template<typename T>
	ZRect<T>::ZRect(T ileft, T itop, T iright, T ibottom)
		: left(ileft), top(itop), right(iright), bottom(ibottom)
	{

	}

	template<typename T>
	ZRect<T>::ZRect()
		: left(0), top(0), right(0), bottom(0)
	{

	}

	template<typename T>
	ZRect<T> ZRect<T>::operator+(const ZRect<T>& other) const
	{
		ZRect<T> temp(*this);
		temp += other;
		return temp;
	}

	template<typename T>
	ZRect<T> ZRect<T>::operator-(const ZRect<T>& other) const
	{
		ZRect<T> temp(*this);
		temp -= other;
		return temp;
	}

	template<typename T>
	ZRect<T> ZRect<T>::operator*(const ZRect<T>& other) const
	{
		ZRect<T> temp(*this);
		temp *= other;
		return temp;
	}

	template<typename T>
	ZRect<T> ZRect<T>::operator/(const ZRect<T>& other) const
	{
		ZRect<T> temp(*this);
		temp /= other;
		return temp;
	}


	template<typename T>
	ZRect<T>& ZRect<T>::operator+=(const ZRect<T>& other)
	{
		left += other.left;
		top += other.top;
		right += other.right;
		bottom += other.bottom;
		return *this;
	}

	template<typename T>
	ZRect<T>& ZRect<T>::operator-=(const ZRect<T>& other)
	{
		left -= other.left;
		top -= other.top;
		right -= other.right;
		bottom -= other.bottom;
		return *this;
	}

	template<typename T>
	ZRect<T>& ZRect<T>::operator*=(const ZRect<T>& other)
	{
		left *= other.left;
		top *= other.top;
		right *= other.right;
		bottom *= other.bottom;
		return *this;
	}

	template<typename T>
	ZRect<T>& ZRect<T>::operator/=(const ZRect<T>& other)
	{
		left /= other.left;
		top /= other.top;
		right /= other.right;
		bottom /= other.bottom;
		return *this;
	}


	template<typename T>
	std::ostream& operator<<(std::ostream& os, const ZRect<T>& rect)
	{
		os << "{left=" << rect.left << ", top=" << rect.top << ", right=" << rect.right << ", bottom=" << rect.bottom << "}";
		return os;
	}

	/// <summary>
	/// Compile struct for float and double
	/// </summary>
	template struct ZRect<float>;
	template struct ZRect<double>;

	/// <summary>
	/// Add output operators for float and double
	/// </summary>
	template std::ostream& operator<<(std::ostream& os, const ZRect<float>& rect);
	template std::ostream& operator<<(std::ostream& os, const ZRect<double>& rect);

}