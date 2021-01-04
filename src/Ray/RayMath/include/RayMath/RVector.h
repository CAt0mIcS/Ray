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
		float v[4];
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
	};

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
}
