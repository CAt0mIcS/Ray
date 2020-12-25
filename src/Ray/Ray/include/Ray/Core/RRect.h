#pragma once

#include "../RBase.h"

namespace std
{
	template <class Elem, class Traits>
	class basic_ostream;
}

namespace At0::Ray
{
	struct RAY_API RRect
	{
		/// <summary>
		/// Untransformed positions of the four sides of a rect
		/// </summary>
		float left, top, right, bottom;

		RRect(float ltrb);
		RRect(float ileft, float itop, float iright, float ibottom);
		RRect();

		RRect operator+(const RRect& other) const;
		RRect operator-(const RRect& other) const;
		RRect operator*(const RRect& other) const;
		RRect operator/(const RRect& other) const;

		RRect& operator+=(const RRect& other);
		RRect& operator-=(const RRect& other);
		RRect& operator*=(const RRect& other);
		RRect& operator/=(const RRect& other);

		RAY_API friend std::ostream& operator<<(std::ostream& os, const RRect& rect);
	};


	
}

