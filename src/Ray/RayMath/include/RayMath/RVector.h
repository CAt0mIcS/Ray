#pragma once

#include "RMathInternals.h"
#include "RFloat4.h"

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
		// --------------------------------------------------------------------
		// Operators
		Vector& RAYMATH_CALLCONV operator=(const Vector&) = default;
		Vector& RAYMATH_CALLCONV operator=(Vector&&) = default;

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
		Vector RAYMATH_CALLCONV operator++(int);
		Vector RAYMATH_CALLCONV operator--(int);

		bool RAYMATH_CALLCONV operator==(FVector other) const;
		bool RAYMATH_CALLCONV operator!=(FVector other) const;
		bool RAYMATH_CALLCONV operator>(FVector other) const;
		bool RAYMATH_CALLCONV operator<(FVector other) const;
		bool RAYMATH_CALLCONV operator>=(FVector other) const;
		bool RAYMATH_CALLCONV operator<=(FVector other) const;

	};
}
