#pragma once

#include <cmath>


namespace Zeal::Util
{
	template<typename T>
	struct Vec2
	{
		/// <summary>
		/// X and Y screen position
		/// </summary>
		T x, y;

		/// <summary>
		/// Vec2 Constructor
		/// </summary>
		/// <param name="xy">Is any value which will be asigned to x and y</param>
		Vec2(T xy);

		/// <summary>
		/// Vec2 Constructor
		/// </summary>
		/// <param name="ix">Is the value asigned to x</param>
		/// <param name="iy">Is the value asigned to y</param>
		Vec2(T ix, T iy);

		/// <summary>
		/// Vec2 Constructor, sets x and y to 0
		/// </summary>
		Vec2();

		Vec2<T> operator+(const Vec2<T>& other) const;
		Vec2<T> operator-(const Vec2<T>& other) const;
		Vec2<T> operator*(const Vec2<T>& other) const;
		Vec2<T> operator/(const Vec2<T>& other) const;

		Vec2<T>& operator+=(const Vec2<T>& other);
		Vec2<T>& operator-=(const Vec2<T>& other);
		Vec2<T>& operator*=(const Vec2<T>& other);
		Vec2<T>& operator/=(const Vec2<T>& other);

		/// <summary>
		/// Change the vec length to exactly one
		/// </summary>
		void Normalize();

		/// <summary>
		/// Calculates a normalized vector from this and returns it
		/// </summary>
		/// <returns>The normalized vector</returns>
		Vec2<T> Normalized() const;

		/// <summary>
		/// Rotate the vector clockwize in the z direction
		/// </summary>
		/// <param name="rotation">Specifies the rotation angle in radians</param>
		void Rotate(T rotation);

		/// <summary>
		/// Rotate this vector clockwize in the z direction
		/// </summary>
		/// <param name="rotation">Specifies the rotation angle in radians</param>
		/// <returns>The rotated vector</returns>
		Vec2<T> Rotated(T rotation) const;

		/// <summary>
		/// Calculates the length of the vector and returns it
		/// </summary>
		/// <returns>The lenght of this vector</returns>
		T Absolute() const;

		
	};
	
	/// <summary>
	/// Output stream operator for the vector
	/// </summary>
	/// <param name="os">Is the output stream</param>
	/// <param name="vec">Is the vector to output</param>
	/// <returns>The modified output stream</returns>
	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Vec2<T>& vec);


	template<typename T>
	using ZPoint = Vec2<T>;
}


