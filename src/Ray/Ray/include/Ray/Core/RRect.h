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

		/// <summary>
		/// RRect Constructor
		/// </summary>
		/// <param name="ltrb">Is any value which will be asigned to left, top, right and bottom</param>
		RRect(float ltrb);

		/// <summary>
		/// RRect Constructor
		/// </summary>
		/// <param name="ileft">Specifies the left side of the rect</param>
		/// <param name="itop">Specifies the top side of the rect</param>
		/// <param name="iright">Specifies the right side of the rect</param>
		/// <param name="ibottom">Specifies the bottom side of the rect</param>
		RRect(float ileft, float itop, float iright, float ibottom);

		/// <summary>
		/// RRect Constructor, sets left, top, right and bottom to 0
		/// </summary>
		RRect();

		RRect operator+(const RRect& other) const;
		RRect operator-(const RRect& other) const;
		RRect operator*(const RRect& other) const;
		RRect operator/(const RRect& other) const;

		RRect& operator+=(const RRect& other);
		RRect& operator-=(const RRect& other);
		RRect& operator*=(const RRect& other);
		RRect& operator/=(const RRect& other);

		/// <summary>
		/// Output stream operator for the rect
		/// </summary>
		/// <param name="os">Is the output stream</param>
		/// <param name="rect">Is the rect to output</param>
		/// <returns>floathe modified output stream</returns>
		RAY_API friend std::ostream& operator<<(std::ostream& os, const RRect& rect);
	};


	
}

