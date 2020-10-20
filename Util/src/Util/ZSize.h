#pragma once

#include <iostream>
#include "Direct2D.h"


namespace Zeal::Util
{
	struct Size
	{
		/// <summary>
		/// Width and Height screen position
		/// </summary>
		float width, height;

		/// <summary>
		/// Size Constructor
		/// </summary>
		/// <param name="xy">Is any value which will be asigned to width and height</param>
		Size(float wh);

		/// <summary>
		/// Size Constructor
		/// </summary>
		/// <param name="ix">Is the value asigned to width</param>
		/// <param name="iy">Is the value asigned to height</param>
		Size(float iw, float ih);

		/// <summary>
		/// Size Constructor, sets width and height to 0
		/// </summary>
		Size();

		Size operator+(const Size& other) const;
		Size operator-(const Size& other) const;
		Size operator*(const Size& other) const;
		Size operator/(const Size& other) const;

		Size& operator+=(const Size& other);
		Size& operator-=(const Size& other);
		Size& operator*=(const Size& other);
		Size& operator/=(const Size& other);

		/// <summary>
		/// Conversion operator to Direct2D Point
		/// </summary>
		operator D2D::PointF() const;

		/// <summary>
		/// Conversion operator to Direct2D Size
		/// </summary>
		operator D2D::SizeF() const;

	};

	/// <summary>
	/// Output stream operator for the vector
	/// </summary>
	/// <param name="os">Is the output stream</param>
	/// <param name="vec">Is the size to output</param>
	/// <returns>floathe modified output stream</returns>
	std::ostream& operator<<(std::ostream& os, const Size& vec);
}