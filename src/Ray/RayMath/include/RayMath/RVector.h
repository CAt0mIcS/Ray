#pragma once

#include "RMathInternals.h"
#include "RFloat4.h"
#include <assert.h>
#include <utility>


namespace At0::Ray
{
	struct Vector;

	// Fix-up for (1st-3rd) Vector parameters that are pass-in-register for x86, ARM, ARM64, and vector call; by reference otherwise
#if ( defined(_M_IX86) || defined(_M_ARM) || defined(_M_ARM64) || RAY_VECTORCALL ) && !defined(RAY_NO_INTRINSICS)
	typedef const Vector FVector;
#else
	typedef const Vector& FVector;
#endif

	// Fix-up for (4th) Vector parameter to pass in-register for ARM, ARM64, and x64 vector call; by reference otherwise
#if ( defined(_M_ARM) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || (RAY_VECTORCALL && !defined(_M_IX86) ) ) && !defined(RAY_NO_INTRINSICS)
	typedef const Vector GVector;
#else
	typedef const Vector& GVector;
#endif

	// Fix-up for (5th & 6th) Vector parameter to pass in-register for ARM64 and vector call; by reference otherwise
#if ( defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || RAY_VECTORCALL ) && !defined(RAY_NO_INTRINSICS)
	typedef const Vector HVector;
#else
	typedef const Vector& HVector;
#endif

	// Fix-up for (7th+) Vector parameters to pass by reference
	typedef const Vector& CVector;

	// ----------------------------------------------------------
	// Vector
	struct Vector
	{
#if defined(RAY_SSE_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
		__m128 v;
#elif defined(RAY_ARM_NEON_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
		float32x4_t v;
#else
		union
		{
			float v[4];
			float x, y, z, w;
		};
#endif
		Vector() = default;

		Vector(float x);
		Vector(float x, float y);
		Vector(float x, float y, float z);
		Vector(float x, float y, float z, float w);

		Vector(const Float3& xyz);
		Vector(const Float3& xyz, float w);
		Vector(const Float2& xy, const Float2& zw);

		Vector(const Vector&) = default;

		Vector& operator=(const Vector&) = default;

		Vector(Vector&&) = default;
		Vector& operator=(Vector&&) = default;

		// --------------------------------------------------------------------
		// Operators
		Vector RAYMATH_CALLCONV operator-() const;

		Vector RAYMATH_CALLCONV operator+(FVector other) const;
		Vector RAYMATH_CALLCONV operator-(FVector other) const;
		Vector RAYMATH_CALLCONV operator*(FVector other) const;
		Vector RAYMATH_CALLCONV operator/(FVector other) const;

		Vector& RAYMATH_CALLCONV operator+=(FVector other);
		Vector& RAYMATH_CALLCONV operator-=(FVector other);
		Vector& RAYMATH_CALLCONV operator*=(FVector other);
		Vector& RAYMATH_CALLCONV operator/=(FVector other);

		Vector& RAYMATH_CALLCONV operator++();
		Vector& RAYMATH_CALLCONV operator--();
		//Vector RAYMATH_CALLCONV operator++(int);
		//Vector RAYMATH_CALLCONV operator--(int);

		bool RAYMATH_CALLCONV operator==(FVector other) const;
		bool RAYMATH_CALLCONV operator!=(FVector other) const;
		//bool RAYMATH_CALLCONV operator>(FVector other) const;
		//bool RAYMATH_CALLCONV operator<(FVector other) const;
		//bool RAYMATH_CALLCONV operator>=(FVector other) const;
		//bool RAYMATH_CALLCONV operator<=(FVector other) const;

		const float& operator[](uint32_t idx) const { assert(idx < 4 && "Index Out Of Range"); return v[idx]; }
		float& operator[](uint32_t idx) { return const_cast<float&>(std::as_const(*this)[idx]); }

		// ---------------------------------------------------
		// Helper Functions
		Vector Length() const;
		Vector Sqrt() const;
		static void SinCos(Vector* pSin, Vector* pCos, Vector v);
		static Vector SplatX(FVector v);
		static Vector SplatY(FVector v);
		static Vector SplatZ(FVector v);
		static Vector Normalize(FVector v);
		static Vector DotProduct(FVector v1, FVector v2);
		static Vector CrossProduct(FVector v1, FVector v2);

		static Vector MergeXY(FVector v1, FVector v2);
		static Vector MergeZW(FVector v1, FVector v2);
		static Vector Select(FVector v1, FVector v2, FVector control);
	};

	namespace Constants
	{
		RAY_GLOBALCONST Vector Select1110 = { (float)SELECT_1, (float)SELECT_1, (float)SELECT_1, (float)SELECT_0 };
	}

	inline Vector::Vector(float x)
	{
		v[0] = x;
		v[1] = 0.0f;
		v[2] = 0.0f;
		v[3] = 0.0f;
	}

	inline Vector::Vector(float x, float y)
	{
		v[0] = x;
		v[1] = y;
		v[2] = 0.0f;
		v[3] = 0.0f;
	}

	inline Vector::Vector(float x, float y, float z)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
		v[3] = 0.0f;
	}

	inline Vector::Vector(float x, float y, float z, float w)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
		v[3] = w;
	}

	inline Vector::Vector(const Float3& xyz)
	{
		v[0] = xyz.x;
		v[1] = xyz.y;
		v[2] = xyz.z;
		v[3] = 0.0f;
	}

	inline Vector::Vector(const Float3& xyz, float w)
	{
		v[0] = xyz.x;
		v[1] = xyz.y;
		v[2] = xyz.z;
		v[3] = w;
	}

	inline Vector::Vector(const Float2& xy, const Float2& zw)
	{
		v[0] = xy.x;
		v[1] = xy.y;
		v[2] = zw.x;
		v[3] = zw.y;
	}


	inline Vector Vector::operator-() const
	{
		Vector vec{};
		vec[0] = -v[0];
		vec[1] = -v[1];
		vec[2] = -v[2];
		vec[3] = -v[3];
		return vec;
	}

	inline Vector Vector::operator+(FVector other) const
	{
		Vector v = *this;
		v[0] += other[0];
		v[1] += other[1];
		v[2] += other[2];
		v[3] += other[3];
		return v;
	}

	inline Vector Vector::operator-(FVector other) const
	{
		Vector v = *this;
		v[0] -= other[0];
		v[1] -= other[1];
		v[2] -= other[2];
		v[3] -= other[3];
		return v;
	}

	inline Vector Vector::operator*(FVector other) const
	{
		Vector v = *this;
		v[0] *= other[0];
		v[1] *= other[1];
		v[2] *= other[2];
		v[3] *= other[3];
		return v;
	}

	inline Vector Vector::operator/(FVector other) const
	{
		Vector v = *this;
		v[0] /= other[0];
		v[1] /= other[1];
		v[2] /= other[2];
		v[3] /= other[3];
		return v;
	}


	inline Vector& Vector::operator+=(FVector other)
	{
		v[0] += other[0];
		v[1] += other[1];
		v[2] += other[2];
		v[3] += other[3];
		return *this;
	}

	inline Vector& Vector::operator-=(FVector other)
	{
		v[0] -= other[0];
		v[1] -= other[1];
		v[2] -= other[2];
		v[3] -= other[3];
		return *this;
	}

	inline Vector& Vector::operator*=(FVector other)
	{
		v[0] *= other[0];
		v[1] *= other[1];
		v[2] *= other[2];
		v[3] *= other[3];
		return *this;
	}

	inline Vector& Vector::operator/=(FVector other)
	{
		v[0] /= other[0];
		v[1] /= other[1];
		v[2] /= other[2];
		v[3] /= other[3];
		return *this;
	}


	inline Vector& Vector::operator++()
	{
		++v[0];
		++v[1];
		++v[2];
		++v[3];
		return *this;
	}

	inline Vector& Vector::operator--()
	{
		--v[0];
		--v[1];
		--v[2];
		--v[3];
		return *this;
	}

	//inline Vector Vector::operator++(int)
	//{

	//}

	//inline Vector Vector::operator--(int)
	//{

	//}


	inline bool Vector::operator==(FVector other) const
	{
		return v[0] == other[0] && v[1] == other[1] && v[2] == other[2] && v[3] == other[3];
	}

	inline bool Vector::operator!=(FVector other) const
	{
		return !(*this == other);
	}

	inline void Vector::SinCos(Vector* pSin, Vector* pCos, Vector v)
	{
		*pSin = {
			sinf(v[0]),
			sinf(v[1]),
			sinf(v[2]),
			sinf(v[3])
		};

		*pCos = {
			cosf(v[0]),
			cosf(v[1]),
			cosf(v[2]),
			cosf(v[3])
		};
	}

	inline Vector Vector::SplatX(FVector v)
	{
		Vector result;
		result[0] =
			result[1] =
			result[2] =
			result[3] = v[0];
		return result;
	}

	inline Vector Vector::SplatY(FVector v)
	{
		Vector result;
		result[0] =
			result[1] =
			result[2] =
			result[3] = v[1];
		return result;
	}

	inline Vector Vector::SplatZ(FVector v)
	{
		Vector result;
		result[0] =
			result[1] =
			result[2] =
			result[3] = v[2];
		return result;
	}

	inline Vector Vector::Normalize(FVector v)
	{
		float length;
		Vector result;

		result = v.Length();
		length = result[0];

		if (length > 0)
			length = 1.0f / length;

		result[0] = v[0] * length;
		result[1] = v[1] * length;
		result[2] = v[2] * length;
		result[3] = v[3] * length;
		return result;
	}

	inline Vector Vector::DotProduct(FVector v1, FVector v2)
	{
		float value = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
		Vector result;
		result[0] =
			result[1] =
			result[2] =
			result[3] = value;
		return result;
	}

	inline Vector Vector::CrossProduct(FVector v1, FVector v2)
	{
		return {
			(v1[1] * v2[2]) - (v1[2] * v2[1]),
			(v1[2] * v2[0]) - (v1[0] * v2[2]),
			(v1[0] * v2[1]) - (v1[1] * v2[0]),
			0.0f
		};
	}

	inline Vector Vector::Length() const
	{
		Vector result = DotProduct(*this, *this);
		return result.Sqrt();
	}

	inline Vector Vector::Sqrt() const
	{
		return {
			sqrtf(v[0]),
			sqrtf(v[1]),
			sqrtf(v[2]),
			sqrtf(v[3])
		};
	}

	inline Vector Vector::MergeXY(FVector v1, FVector v2)
	{
		return {
			v1[0],
			v2[0],
			v1[1],
			v2[1],
		};
	}

	inline Vector Vector::MergeZW(FVector v1, FVector v2)
	{
		return {
			v1[2],
			v2[2],
			v1[3],
			v2[3]
		};
	}

	inline Vector Vector::Select(FVector v1, FVector v2, FVector control)
	{
		struct Vec
		{
			union
			{
				uint32_t x, y, z, w;
				uint32_t r[4];
			};

			Vec(uint32_t x, uint32_t y, uint32_t z, uint32_t w) : x(x), y(y), z(z), w(w) {}
			uint32_t operator[](uint32_t idx) { return r[idx]; }
		};

		Vec vec1{ (float)v1.x, (float)v1.y, (float)v1.z, (float)v1.w };
		Vec vec2{ (float)v2.x, (float)v2.y, (float)v2.z, (float)v2.w };
		Vec vecControl{ (float)control.x, (float)control.y, (float)control.z, (float)control.w };

		Vec result{
			(vec1[0] & ~vecControl[0]) | (vec2[0] & vecControl[0]),
			(vec1[1] & ~vecControl[1]) | (vec2[1] & vecControl[1]),
			(vec1[2] & ~vecControl[2]) | (vec2[2] & vecControl[2]),
			(vec1[3] & ~vecControl[3]) | (vec2[3] & vecControl[3]),
		};

		return { (float)result.x, (float)result.y, (float)result.z, (float)result.w };
	}

}
