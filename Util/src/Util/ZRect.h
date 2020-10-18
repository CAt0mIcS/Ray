#pragma once

#if 0



namespace Zeal::Util
{
	template<typename T>
	struct ZRect
	{
		/// <summary>
		/// Untransformed positions of the four sides of a rect
		/// </summary>
		T left, top, right, bottom;

		/// <summary>
		/// ZRect Constructor
		/// </summary>
		/// <param name="ltrb">Is any value which will be asigned to left, top, right and bottom</param>
		ZRect(T ltrb);

		/// <summary>
		/// ZRect Constructor
		/// </summary>
		/// <param name="ileft">Specifies the left side of the rect</param>
		/// <param name="itop">Specifies the top side of the rect</param>
		/// <param name="iright">Specifies the right side of the rect</param>
		/// <param name="ibottom">Specifies the bottom side of the rect</param>
		ZRect(T ileft, T itop, T iright, T ibottom);

		/// <summary>
		/// ZRect Constructor, sets left, top, right and bottom to 0
		/// </summary>
		ZRect();

		ZRect<T> operator+(const ZRect<T>& other) const;
		ZRect<T> operator-(const ZRect<T>& other) const;
		ZRect<T> operator*(const ZRect<T>& other) const;
		ZRect<T> operator/(const ZRect<T>& other) const;

		ZRect<T>& operator+=(const ZRect<T>& other);
		ZRect<T>& operator-=(const ZRect<T>& other);
		ZRect<T>& operator*=(const ZRect<T>& other);
		ZRect<T>& operator/=(const ZRect<T>& other);
	};


	/// <summary>
	/// Output stream operator for the rect
	/// </summary>
	/// <param name="os">Is the output stream</param>
	/// <param name="rect">Is the rect to output</param>
	/// <returns>The modified output stream</returns>
	template<typename T>
	std::ostream& operator<<(std::ostream& os, const ZRect<T>& rect);
}

#else

namespace Zeal::Util
{
	struct ZRect
	{
		/// <summary>
		/// Untransformed positions of the four sides of a rect
		/// </summary>
		float left, top, right, bottom;

		/// <summary>
		/// ZRect Constructor
		/// </summary>
		/// <param name="ltrb">Is any value which will be asigned to left, top, right and bottom</param>
		ZRect(float ltrb);

		/// <summary>
		/// ZRect Constructor
		/// </summary>
		/// <param name="ileft">Specifies the left side of the rect</param>
		/// <param name="itop">Specifies the top side of the rect</param>
		/// <param name="iright">Specifies the right side of the rect</param>
		/// <param name="ibottom">Specifies the bottom side of the rect</param>
		ZRect(float ileft, float itop, float iright, float ibottom);

		/// <summary>
		/// ZRect Constructor, sets left, top, right and bottom to 0
		/// </summary>
		ZRect();

		ZRect operator+(const ZRect& other) const;
		ZRect operator-(const ZRect& other) const;
		ZRect operator*(const ZRect& other) const;
		ZRect operator/(const ZRect& other) const;

		ZRect& operator+=(const ZRect& other);
		ZRect& operator-=(const ZRect& other);
		ZRect& operator*=(const ZRect& other);
		ZRect& operator/=(const ZRect& other);
	};


	/// <summary>
	/// Output stream operator for the rect
	/// </summary>
	/// <param name="os">Is the output stream</param>
	/// <param name="rect">Is the rect to output</param>
	/// <returns>floathe modified output stream</returns>
	std::ostream& operator<<(std::ostream& os, const ZRect& rect);
}

#endif