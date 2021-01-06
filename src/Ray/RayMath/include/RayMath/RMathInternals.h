#pragma once

// RAY_TODO: Implement intrinsics (remove this to use them)
#define RAY_NO_INTRINSICS

// Disable intrinsics for linux (temporary) (RAY_TODO)
#if defined(__linux__) && !defined(RAY_NO_INTRINSICS)
#define RAY_NO_INTRINSICS
#endif


#if defined(_MSC_VER) && !defined(_M_ARM) && !defined(_M_ARM64) && !defined(_M_HYBRID_X86_ARM64) && (!_MANAGED) && (!_M_CEE) && (!defined(_M_IX86_FP) || (_M_IX86_FP > 1))
#define RAYMATH_CALLCONV __vectorcall
#else
#define RAYMATH_CALLCONV
#endif


#if defined(_MSC_VER)
#define RAY_ALIGNED(x) __declspec(align(x))
#elif defined(__GNUC__)
#define RAY_ALIGNED(x) __attribute__ ((aligned(x)))
#endif

#if !defined(RAY_AVX2_INTRINSICS) && defined(__AVX2__) && !defined(RAY_NO_INTRINSICS)
#define RAY_AVX2_INTRINSICS
#endif

#if !defined(RAY_FMA3_INTRINSICS) && defined(RAY_AVX2_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
#define RAY_FMA3_INTRINSICS
#endif

#if !defined(RAY_F16C_INTRINSICS) && defined(RAY_AVX2_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
#define RAY_F16C_INTRINSICS
#endif

#if defined(RAY_FMA3_INTRINSICS) && !defined(RAY_AVX_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
#define RAY_AVX_INTRINSICS
#endif

#if defined(RAY_F16C_INTRINSICS) && !defined(RAY_AVX_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
#define RAY_AVX_INTRINSICS
#endif

#if !defined(RAY_AVX_INTRINSICS) && defined(__AVX__) && !defined(RAY_NO_INTRINSICS)
#define RAY_AVX_INTRINSICS
#endif

#if defined(RAY_AVX_INTRINSICS) && !defined(RAY_SSE4_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
#define RAY_SSE4_INTRINSICS
#endif

#if defined(RAY_SSE4_INTRINSICS) && !defined(RAY_SSE3_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
#define RAY_SSE3_INTRINSICS
#endif

#if defined(RAY_SSE3_INTRINSICS) && !defined(RAY_SSE_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
#define RAY_SSE_INTRINSICS
#endif

#if !defined(RAY_ARM_NEON_INTRINSICS) && !defined(RAY_SSE_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
#if (defined(_M_IX86) || defined(_M_X64)) && !defined(_M_HYBRID_X86_ARM64)
#define RAY_SSE_INTRINSICS
#elif defined(_M_ARM) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
#define RAY_ARM_NEON_INTRINSICS
#endif
#endif // !RAY_ARM_NEON_INTRINSICS && !RAY_SSE_INTRINSICS && !RAY_NO_INTRINSICS

#include <stdint.h>
#include <math.h>
#include <float.h>
#include <malloc.h>

// __cpuid on windows
#ifdef _WIN32
#include <intrin.h>
#endif

#ifndef RAY_NO_INTRINSICS
#ifdef RAY_SSE_INTRINSICS
#include <xmmintrin.h>
#include <emmintrin.h>

#ifdef RAY_SSE3_INTRINSICS
#include <pmmintrin.h>
#endif

#ifdef RAY_SSE4_INTRINSICS
#include <smmintrin.h>
#endif

#ifdef RAY_AVX_INTRINSICS
#include <immintrin.h>
#endif

#elif defined(_XM_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
#include <arm64_neon.h>
#else
#include <arm_neon.h>
#endif
#endif
#endif // !_XM_NO_INTRINSICS

#ifndef RAY_GLOBALCONST
#ifdef _MSC_VER
#define RAY_GLOBALCONST extern const __declspec(selectany)
#elif defined(__GNUC__)
#define RAY_GLOBALCONST extern const __attribute__((selectany))
#endif
#endif // ! RAY_GLOBALCONST



namespace At0::Ray
{
	namespace Constants
	{
		constexpr float PI = 3.141592654f;
		constexpr float M2PI = 6.283185307f;
		constexpr float M1DIVPI = 0.318309886f;
		constexpr float M1DIV2PI = 0.159154943f;
		constexpr float PIDIV2 = 1.570796327f;
		constexpr float PIDIV4 = 0.785398163f;

		constexpr uint32_t SELECT_0 = 0x00000000;
		constexpr uint32_t SELECT_1 = 0xFFFFFFFF;
	}

	template<typename T = float>
	constexpr T ConvertToRadians(T degrees) { return degrees * (PI / static_cast<T>(180)) }

	template<typename T = float>
	constexpr T ConvertToDegrees(T radians) { return radians * (static_cast<T>(180) / PI) }

}