//-------------------------------------------------------------------------------------
// DirectXMath.h -- SIMD C++ Math library
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// 							   The MIT License (MIT)
//
// Copyright (c) 2011-2020 Microsoft Corp
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// NOTICE
// This file has been modified to assure platform compatibility and correct namespacing.
//-------------------------------------------------------------------------------------

#pragma once

#ifndef __cplusplus
#error DirectX Math requires C++
#endif

#define DIRECTX_MATH_VERSION 313


#if defined(_MSC_VER) && !defined(_M_ARM) && !defined(_M_ARM64) && !defined(_M_HYBRID_X86_ARM64) && (!_MANAGED) && (!_M_CEE) && (!defined(_M_IX86_FP) || (_M_IX86_FP > 1)) && !defined(_XM_NO_INTRINSICS_) && !defined(_XM_VECTORCALL_)
#define _XM_VECTORCALL_ 1
#endif

#ifdef __linux__
#define _XM_NO_INTRINSICS_
#endif

#if _XM_VECTORCALL_
#define XM_CALLCONV __vectorcall
#else
#define XM_CALLCONV
#endif

#if defined(_MSC_VER) && (_MSC_FULL_VER < 190023506)
#define XM_CONST const
#define XM_CONSTEXPR
#else
#define XM_CONST constexpr
#define XM_CONSTEXPR constexpr
#endif

#if defined(_MSC_VER)
#define XM_ALIGNED(x) __declspec(align(x))
#else
#if defined(__GNUC__)
#define XM_ALIGNED(x) __attribute__ ((aligned(x)))
#endif
#endif

#ifndef XM_DEPRECATED
#define XM_DEPRECATED [[deprecated("This is deprecated and will be removed in a future version.")]]
#endif

#if !defined(_XM_AVX2_INTRINSICS_) && defined(__AVX2__) && !defined(_XM_NO_INTRINSICS_)
#define _XM_AVX2_INTRINSICS_
#endif

#if !defined(_XM_FMA3_INTRINSICS_) && defined(_XM_AVX2_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
#define _XM_FMA3_INTRINSICS_
#endif

#if !defined(_XM_F16C_INTRINSICS_) && defined(_XM_AVX2_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
#define _XM_F16C_INTRINSICS_
#endif

#if defined(_XM_FMA3_INTRINSICS_) && !defined(_XM_AVX_INTRINSICS_)
#define _XM_AVX_INTRINSICS_
#endif

#if defined(_XM_F16C_INTRINSICS_) && !defined(_XM_AVX_INTRINSICS_)
#define _XM_AVX_INTRINSICS_
#endif

#if !defined(_XM_AVX_INTRINSICS_) && defined(__AVX__) && !defined(_XM_NO_INTRINSICS_)
#define _XM_AVX_INTRINSICS_
#endif

#if defined(_XM_AVX_INTRINSICS_) && !defined(_XM_SSE4_INTRINSICS_)
#define _XM_SSE4_INTRINSICS_
#endif

#if defined(_XM_SSE4_INTRINSICS_) && !defined(_XM_SSE3_INTRINSICS_)
#define _XM_SSE3_INTRINSICS_
#endif

#if defined(_XM_SSE3_INTRINSICS_) && !defined(_XM_SSE_INTRINSICS_)
#define _XM_SSE_INTRINSICS_
#endif

#if !defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
#if (defined(_M_IX86) || defined(_M_X64)) && !defined(_M_HYBRID_X86_ARM64)
#define _XM_SSE_INTRINSICS_
#elif defined(_M_ARM) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
#define _XM_ARM_NEON_INTRINSICS_
#endif
#endif // !_XM_ARM_NEON_INTRINSICS_ && !_XM_SSE_INTRINSICS_ && !_XM_NO_INTRINSICS_

#if !defined(_XM_NO_Vector_OVERLOADS_) && defined(__clang__)
#define _XM_NO_Vector_OVERLOADS_
#endif

#pragma warning(push)
#pragma warning(disable:4514 4820)
// C4514/4820: Off by default noise
#include <math.h>
#include <float.h>
#include <malloc.h>
#pragma warning(pop)

#ifndef _XM_NO_INTRINSICS_
#pragma warning(push)
#pragma warning(disable : 4987)
// C4987: Off by default noise
#pragma warning(pop)

#ifdef _WIN32
#include <intrin.h>
#endif

#ifdef _XM_SSE_INTRINSICS_
#include <xmmintrin.h>
#include <emmintrin.h>

#ifdef _XM_SSE3_INTRINSICS_
#include <pmmintrin.h>
#endif

#ifdef _XM_SSE4_INTRINSICS_
#include <smmintrin.h>
#endif

#ifdef _XM_AVX_INTRINSICS_
#include <immintrin.h>
#endif

#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
#include <arm64_neon.h>
#else
#include <arm_neon.h>
#endif
#endif
#endif // !_XM_NO_INTRINSICS_

#include <assert.h>

#pragma warning(push)
#pragma warning(disable : 4005 4668)
// C4005/4668: Old header issue
#include <stdint.h>
#pragma warning(pop)

/****************************************************************************
 *
 * Conditional intrinsics
 *
 ****************************************************************************/

#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)

#if defined(_XM_NO_MOVNT_)
#define XM_STREAM_PS( p, a ) _mm_store_ps( p, a )
#define XM_SFENCE()
#else
#define XM_STREAM_PS( p, a ) _mm_stream_ps( p, a )
#define XM_SFENCE() _mm_sfence()
#endif

#if defined(_XM_AVX_INTRINSICS_)
#define XM_PERMUTE_PS( v, c ) _mm_permute_ps( v, c )
#else
#define XM_PERMUTE_PS( v, c ) _mm_shuffle_ps( v, v, c )
#endif

#endif // _XM_SSE_INTRINSICS_ && !_XM_NO_INTRINSICS_

namespace At0
{
	namespace Ray
	{


		/****************************************************************************
		 *
		 * Constant definitions
		 *
		 ****************************************************************************/

#if defined(__XNAMATH_H__) && defined(XM_PI)
#undef XM_PI
#undef XM_2PI
#undef XM_1DIVPI
#undef XM_1DIV2PI
#undef XM_PIDIV2
#undef XM_PIDIV4
#undef XM_SELECT_0
#undef XM_SELECT_1
#undef XM_PERMUTE_0X
#undef XM_PERMUTE_0Y
#undef XM_PERMUTE_0Z
#undef XM_PERMUTE_0W
#undef XM_PERMUTE_1X
#undef XM_PERMUTE_1Y
#undef XM_PERMUTE_1Z
#undef XM_PERMUTE_1W
#undef XM_CRMASK_CR6
#undef XM_CRMASK_CR6TRUE
#undef XM_CRMASK_CR6FALSE
#undef XM_CRMASK_CR6BOUNDS
#undef XM_CACHE_LINE_SIZE
#endif

		XM_CONST float XM_PI = 3.141592654f;
		XM_CONST float XM_2PI = 6.283185307f;
		XM_CONST float XM_1DIVPI = 0.318309886f;
		XM_CONST float XM_1DIV2PI = 0.159154943f;
		XM_CONST float XM_PIDIV2 = 1.570796327f;
		XM_CONST float XM_PIDIV4 = 0.785398163f;

		XM_CONST uint32_t XM_SELECT_0 = 0x00000000;
		XM_CONST uint32_t XM_SELECT_1 = 0xFFFFFFFF;

		XM_CONST uint32_t XM_PERMUTE_0X = 0;
		XM_CONST uint32_t XM_PERMUTE_0Y = 1;
		XM_CONST uint32_t XM_PERMUTE_0Z = 2;
		XM_CONST uint32_t XM_PERMUTE_0W = 3;
		XM_CONST uint32_t XM_PERMUTE_1X = 4;
		XM_CONST uint32_t XM_PERMUTE_1Y = 5;
		XM_CONST uint32_t XM_PERMUTE_1Z = 6;
		XM_CONST uint32_t XM_PERMUTE_1W = 7;

		XM_CONST uint32_t XM_SWIZZLE_X = 0;
		XM_CONST uint32_t XM_SWIZZLE_Y = 1;
		XM_CONST uint32_t XM_SWIZZLE_Z = 2;
		XM_CONST uint32_t XM_SWIZZLE_W = 3;

		XM_CONST uint32_t XM_CRMASK_CR6 = 0x000000F0;
		XM_CONST uint32_t XM_CRMASK_CR6TRUE = 0x00000080;
		XM_CONST uint32_t XM_CRMASK_CR6FALSE = 0x00000020;
		XM_CONST uint32_t XM_CRMASK_CR6BOUNDS = XM_CRMASK_CR6FALSE;

		XM_CONST size_t XM_CACHE_LINE_SIZE = 64;


		/****************************************************************************
		 *
		 * Macros
		 *
		 ****************************************************************************/

#if defined(__XNAMATH_H__) && defined(ComparisonAllTrue)
#undef ComparisonAllTrue
#undef ComparisonAnyTrue
#undef ComparisonAllFalse
#undef ComparisonAnyFalse
#undef ComparisonMixed
#undef ComparisonAllInBounds
#undef ComparisonAnyOutOfBounds
#endif

		 // Unit conversion

		inline XM_CONSTEXPR float ConvertToRadians(float fDegrees) { return fDegrees * (XM_PI / 180.0f); }
		inline XM_CONSTEXPR float ConvertToDegrees(float fRadians) { return fRadians * (180.0f / XM_PI); }

		// Condition register evaluation proceeding a recording (R) comparison

		inline bool ComparisonAllTrue(uint32_t CR) { return (((CR)&XM_CRMASK_CR6TRUE) == XM_CRMASK_CR6TRUE); }
		inline bool ComparisonAnyTrue(uint32_t CR) { return (((CR)&XM_CRMASK_CR6FALSE) != XM_CRMASK_CR6FALSE); }
		inline bool ComparisonAllFalse(uint32_t CR) { return (((CR)&XM_CRMASK_CR6FALSE) == XM_CRMASK_CR6FALSE); }
		inline bool ComparisonAnyFalse(uint32_t CR) { return (((CR)&XM_CRMASK_CR6TRUE) != XM_CRMASK_CR6TRUE); }
		inline bool ComparisonMixed(uint32_t CR) { return (((CR)&XM_CRMASK_CR6) == 0); }
		inline bool ComparisonAllInBounds(uint32_t CR) { return (((CR)&XM_CRMASK_CR6BOUNDS) == XM_CRMASK_CR6BOUNDS); }
		inline bool ComparisonAnyOutOfBounds(uint32_t CR) { return (((CR)&XM_CRMASK_CR6BOUNDS) != XM_CRMASK_CR6BOUNDS); }


		/****************************************************************************
		 *
		 * Data types
		 *
		 ****************************************************************************/

#pragma warning(push)
#pragma warning(disable:4068 4201 4365 4324 4820)
		 // C4068: ignore unknown pragmas
		 // C4201: nonstandard extension used : nameless struct/union
		 // C4365: Off by default noise
		 // C4324/4820: padding warnings

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 25000, "FVector is 16 bytes")
#endif

//------------------------------------------------------------------------------
#if defined(_XM_NO_INTRINSICS_)
		struct __vector4
		{
			union
			{
				float       vector4_f32[4];
				uint32_t    vector4_u32[4];
			};
		};
#endif // _XM_NO_INTRINSICS_

		//------------------------------------------------------------------------------
		// Vector intrinsic: Four 32 bit floating point components aligned on a 16 byte
		// boundary and mapped to hardware vector registers
#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
		typedef __m128 Vector;
#elif defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
		typedef float32x4_t Vector;
#else
		typedef __vector4 Vector;
#endif

		// Fix-up for (1st-3rd) Vector parameters that are pass-in-register for x86, ARM, ARM64, and vector call; by reference otherwise
#if ( defined(_M_IX86) || defined(_M_ARM) || defined(_M_ARM64) || _XM_VECTORCALL_ ) && !defined(_XM_NO_INTRINSICS_)
		typedef const Vector FVector;
#else
		typedef const Vector& FVector;
#endif

		// Fix-up for (4th) Vector parameter to pass in-register for ARM, ARM64, and x64 vector call; by reference otherwise
#if ( defined(_M_ARM) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || (_XM_VECTORCALL_ && !defined(_M_IX86) ) ) && !defined(_XM_NO_INTRINSICS_)
		typedef const Vector GVector;
#else
		typedef const Vector& GVector;
#endif

		// Fix-up for (5th & 6th) Vector parameter to pass in-register for ARM64 and vector call; by reference otherwise
#if ( defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || _XM_VECTORCALL_ ) && !defined(_XM_NO_INTRINSICS_)
		typedef const Vector HVector;
#else
		typedef const Vector& HVector;
#endif

		// Fix-up for (7th+) Vector parameters to pass by reference
		typedef const Vector& CVector;

		//------------------------------------------------------------------------------
		// Conversion types for constants
		XM_ALIGNED(16) struct VectorF32
		{
			union
			{
				float f[4];
				Vector v;
			};

			inline operator Vector() const { return v; }
			inline operator const float* () const { return f; }
#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)
			inline operator __m128i() const { return _mm_castps_si128(v); }
			inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
		};

		XM_ALIGNED(16) struct VectorI32
		{
			union
			{
				int32_t i[4];
				Vector v;
			};

			inline operator Vector() const { return v; }
#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)
			inline operator __m128i() const { return _mm_castps_si128(v); }
			inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
		};

		XM_ALIGNED(16) struct VectorU8
		{
			union
			{
				uint8_t u[16];
				Vector v;
			};

			inline operator Vector() const { return v; }
#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)
			inline operator __m128i() const { return _mm_castps_si128(v); }
			inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
		};

		XM_ALIGNED(16) struct VectorU32
		{
			union
			{
				uint32_t u[4];
				Vector v;
			};

			inline operator Vector() const { return v; }
#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)
			inline operator __m128i() const { return _mm_castps_si128(v); }
			inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
		};

		//------------------------------------------------------------------------------
		// Vector operators

#ifndef _XM_NO_Vector_OVERLOADS_
		Vector    XM_CALLCONV     operator+ (FVector V);
		Vector    XM_CALLCONV     operator- (FVector V);

		Vector& XM_CALLCONV     operator+= (Vector& V1, FVector V2);
		Vector& XM_CALLCONV     operator-= (Vector& V1, FVector V2);
		Vector& XM_CALLCONV     operator*= (Vector& V1, FVector V2);
		Vector& XM_CALLCONV     operator/= (Vector& V1, FVector V2);

		Vector& operator*= (Vector& V, float S);
		Vector& operator/= (Vector& V, float S);

		Vector    XM_CALLCONV     operator+ (FVector V1, FVector V2);
		Vector    XM_CALLCONV     operator- (FVector V1, FVector V2);
		Vector    XM_CALLCONV     operator* (FVector V1, FVector V2);
		Vector    XM_CALLCONV     operator/ (FVector V1, FVector V2);
		Vector    XM_CALLCONV     operator* (FVector V, float S);
		Vector    XM_CALLCONV     operator* (float S, FVector V);
		Vector    XM_CALLCONV     operator/ (FVector V, float S);
#endif /* !_XM_NO_Vector_OVERLOADS_ */

		//------------------------------------------------------------------------------
		// Matrix type: Sixteen 32 bit floating point components aligned on a
		// 16 byte boundary and mapped to four hardware vector registers

		struct Matrix;

		// Fix-up for (1st) Matrix parameter to pass in-register for ARM64 and vector call; by reference otherwise
#if ( defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || _XM_VECTORCALL_ ) && !defined(_XM_NO_INTRINSICS_)
		typedef const Matrix FMatrix;
#else
		typedef const Matrix& FMatrix;
#endif

		// Fix-up for (2nd+) Matrix parameters to pass by reference
		typedef const Matrix& CMatrix;

#ifdef _XM_NO_INTRINSICS_
		struct Matrix
#else
		XM_ALIGNED(16) struct Matrix
#endif
		{
#ifdef _XM_NO_INTRINSICS_
			union
			{
				Vector r[4];
				struct
				{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
					float _41, _42, _43, _44;
				};
				float m[4][4];
			};
#else
			Vector r[4];
#endif

			Matrix() = default;

			Matrix(const Matrix&) = default;

#if defined(_MSC_VER) && (_MSC_FULL_VER < 191426431)
			Matrix& operator= (const Matrix& M) noexcept { r[0] = M.r[0]; r[1] = M.r[1]; r[2] = M.r[2]; r[3] = M.r[3]; return *this; }
#else
			Matrix& operator=(const Matrix&) = default;

			Matrix(Matrix&&) = default;
			Matrix& operator=(Matrix&&) = default;
#endif

			constexpr Matrix(FVector R0, FVector R1, FVector R2, CVector R3) : r{ R0,R1,R2,R3 } {}
			Matrix(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33);
			explicit Matrix(/*_In_reads_(16) */const float* pArray);

#ifdef _XM_NO_INTRINSICS_
			float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
			float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }
#endif

			Matrix    operator+ () const { return *this; }
			Matrix    operator- () const;

			Matrix& XM_CALLCONV     operator+= (FMatrix M);
			Matrix& XM_CALLCONV     operator-= (FMatrix M);
			Matrix& XM_CALLCONV     operator*= (FMatrix M);
			Matrix& operator*= (float S);
			Matrix& operator/= (float S);

			Matrix    XM_CALLCONV     operator+ (FMatrix M) const;
			Matrix    XM_CALLCONV     operator- (FMatrix M) const;
			Matrix    XM_CALLCONV     operator* (FMatrix M) const;
			Matrix    operator* (float S) const;
			Matrix    operator/ (float S) const;

			friend Matrix     XM_CALLCONV     operator* (float S, FMatrix M);
		};

		//------------------------------------------------------------------------------
		// 2D Vector; 32 bit floating point components
		struct Float2
		{
			float x;
			float y;

			Float2() = default;

			Float2(const Float2&) = default;
			Float2& operator=(const Float2&) = default;

			Float2(Float2&&) = default;
			Float2& operator=(Float2&&) = default;

			XM_CONSTEXPR Float2(float _x, float _y) : x(_x), y(_y) {}
			explicit Float2(/*_In_reads_(2) */const float* pArray) : x(pArray[0]), y(pArray[1]) {}
		};

		// 2D Vector; 32 bit floating point components aligned on a 16 byte boundary
		XM_ALIGNED(16) struct Float2A : public Float2
		{
			Float2A() = default;

			Float2A(const Float2A&) = default;
			Float2A& operator=(const Float2A&) = default;

			Float2A(Float2A&&) = default;
			Float2A& operator=(Float2A&&) = default;

			XM_CONSTEXPR Float2A(float _x, float _y) : Float2(_x, _y) {}
			explicit Float2A(/*_In_reads_(2) */const float* pArray) : Float2(pArray) {}
		};

		//------------------------------------------------------------------------------
		// 2D Vector; 32 bit signed integer components
		struct Int2
		{
			int32_t x;
			int32_t y;

			Int2() = default;

			Int2(const Int2&) = default;
			Int2& operator=(const Int2&) = default;

			Int2(Int2&&) = default;
			Int2& operator=(Int2&&) = default;

			XM_CONSTEXPR Int2(int32_t _x, int32_t _y) : x(_x), y(_y) {}
			explicit Int2(/*_In_reads_(2) */const int32_t* pArray) : x(pArray[0]), y(pArray[1]) {}
		};

		// 2D Vector; 32 bit unsigned integer components
		struct UInt2
		{
			uint32_t x;
			uint32_t y;

			UInt2() = default;

			UInt2(const UInt2&) = default;
			UInt2& operator=(const UInt2&) = default;

			UInt2(UInt2&&) = default;
			UInt2& operator=(UInt2&&) = default;

			XM_CONSTEXPR UInt2(uint32_t _x, uint32_t _y) : x(_x), y(_y) {}
			explicit UInt2(/*_In_reads_(2) */const uint32_t* pArray) : x(pArray[0]), y(pArray[1]) {}
		};

		//------------------------------------------------------------------------------
		// 3D Vector; 32 bit floating point components
		struct Float3
		{
			float x;
			float y;
			float z;

			Float3() = default;

			Float3(const Float3&) = default;
			Float3& operator=(const Float3&) = default;

			Float3(Float3&&) = default;
			Float3& operator=(Float3&&) = default;

			XM_CONSTEXPR Float3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
			explicit Float3(/*_In_reads_(3) */const float* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
		};

		// 3D Vector; 32 bit floating point components aligned on a 16 byte boundary
		XM_ALIGNED(16) struct Float3A : public Float3
		{
			Float3A() = default;

			Float3A(const Float3A&) = default;
			Float3A& operator=(const Float3A&) = default;

			Float3A(Float3A&&) = default;
			Float3A& operator=(Float3A&&) = default;

			XM_CONSTEXPR Float3A(float _x, float _y, float _z) : Float3(_x, _y, _z) {}
			explicit Float3A(/*_In_reads_(3) */const float* pArray) : Float3(pArray) {}
		};

		//------------------------------------------------------------------------------
		// 3D Vector; 32 bit signed integer components
		struct Int3
		{
			int32_t x;
			int32_t y;
			int32_t z;

			Int3() = default;

			Int3(const Int3&) = default;
			Int3& operator=(const Int3&) = default;

			Int3(Int3&&) = default;
			Int3& operator=(Int3&&) = default;

			XM_CONSTEXPR Int3(int32_t _x, int32_t _y, int32_t _z) : x(_x), y(_y), z(_z) {}
			explicit Int3(/*_In_reads_(3) */const int32_t* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
		};

		// 3D Vector; 32 bit unsigned integer components
		struct UInt3
		{
			uint32_t x;
			uint32_t y;
			uint32_t z;

			UInt3() = default;

			UInt3(const UInt3&) = default;
			UInt3& operator=(const UInt3&) = default;

			UInt3(UInt3&&) = default;
			UInt3& operator=(UInt3&&) = default;

			XM_CONSTEXPR UInt3(uint32_t _x, uint32_t _y, uint32_t _z) : x(_x), y(_y), z(_z) {}
			explicit UInt3(/*_In_reads_(3) */const uint32_t* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
		};

		//------------------------------------------------------------------------------
		// 4D Vector; 32 bit floating point components
		struct Float4
		{
			float x;
			float y;
			float z;
			float w;

			Float4() = default;

			Float4(const Float4&) = default;
			Float4& operator=(const Float4&) = default;

			Float4(Float4&&) = default;
			Float4& operator=(Float4&&) = default;

			XM_CONSTEXPR Float4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
			explicit Float4(/*_In_reads_(4) */const float* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}
		};

		// 4D Vector; 32 bit floating point components aligned on a 16 byte boundary
		XM_ALIGNED(16) struct Float4A : public Float4
		{
			Float4A() = default;

			Float4A(const Float4A&) = default;
			Float4A& operator=(const Float4A&) = default;

			Float4A(Float4A&&) = default;
			Float4A& operator=(Float4A&&) = default;

			XM_CONSTEXPR Float4A(float _x, float _y, float _z, float _w) : Float4(_x, _y, _z, _w) {}
			explicit Float4A(/*_In_reads_(4) */const float* pArray) : Float4(pArray) {}
		};

		//------------------------------------------------------------------------------
		// 4D Vector; 32 bit signed integer components
		struct Int4
		{
			int32_t x;
			int32_t y;
			int32_t z;
			int32_t w;

			Int4() = default;

			Int4(const Int4&) = default;
			Int4& operator=(const Int4&) = default;

			Int4(Int4&&) = default;
			Int4& operator=(Int4&&) = default;

			XM_CONSTEXPR Int4(int32_t _x, int32_t _y, int32_t _z, int32_t _w) : x(_x), y(_y), z(_z), w(_w) {}
			explicit Int4(/*_In_reads_(4) */const int32_t* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}
		};

		// 4D Vector; 32 bit unsigned integer components
		struct UInt4
		{
			uint32_t x;
			uint32_t y;
			uint32_t z;
			uint32_t w;

			UInt4() = default;

			UInt4(const UInt4&) = default;
			UInt4& operator=(const UInt4&) = default;

			UInt4(UInt4&&) = default;
			UInt4& operator=(UInt4&&) = default;

			XM_CONSTEXPR UInt4(uint32_t _x, uint32_t _y, uint32_t _z, uint32_t _w) : x(_x), y(_y), z(_z), w(_w) {}
			explicit UInt4(/*_In_reads_(4) */const uint32_t* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}
		};

		//------------------------------------------------------------------------------
		// 3x3 Matrix: 32 bit floating point components
		struct Float3X3
		{
			union
			{
				struct
				{
					float _11, _12, _13;
					float _21, _22, _23;
					float _31, _32, _33;
				};
				float m[3][3];
			};

			Float3X3() = default;

			Float3X3(const Float3X3&) = default;
			Float3X3& operator=(const Float3X3&) = default;

			Float3X3(Float3X3&&) = default;
			Float3X3& operator=(Float3X3&&) = default;

			XM_CONSTEXPR Float3X3(float m00, float m01, float m02,
				float m10, float m11, float m12,
				float m20, float m21, float m22)
				: _11(m00), _12(m01), _13(m02),
				_21(m10), _22(m11), _23(m12),
				_31(m20), _32(m21), _33(m22) {}
			explicit Float3X3(/*_In_reads_(9) */const float* pArray);

			float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
			float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }
		};

		//------------------------------------------------------------------------------
		// 4x3 Row-major Matrix: 32 bit floating point components
		struct Float4X3
		{
			union
			{
				struct
				{
					float _11, _12, _13;
					float _21, _22, _23;
					float _31, _32, _33;
					float _41, _42, _43;
				};
				float m[4][3];
				float f[12];
			};

			Float4X3() = default;

			Float4X3(const Float4X3&) = default;
			Float4X3& operator=(const Float4X3&) = default;

			Float4X3(Float4X3&&) = default;
			Float4X3& operator=(Float4X3&&) = default;

			XM_CONSTEXPR Float4X3(float m00, float m01, float m02,
				float m10, float m11, float m12,
				float m20, float m21, float m22,
				float m30, float m31, float m32)
				: _11(m00), _12(m01), _13(m02),
				_21(m10), _22(m11), _23(m12),
				_31(m20), _32(m21), _33(m22),
				_41(m30), _42(m31), _43(m32) {}
			explicit Float4X3(/*_In_reads_(12) */const float* pArray);

			float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
			float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }
		};

		// 4x3 Row-major Matrix: 32 bit floating point components aligned on a 16 byte boundary
		XM_ALIGNED(16) struct Float4X3A : public Float4X3
		{
			Float4X3A() = default;

			Float4X3A(const Float4X3A&) = default;
			Float4X3A& operator=(const Float4X3A&) = default;

			Float4X3A(Float4X3A&&) = default;
			Float4X3A& operator=(Float4X3A&&) = default;

			XM_CONSTEXPR Float4X3A(float m00, float m01, float m02,
				float m10, float m11, float m12,
				float m20, float m21, float m22,
				float m30, float m31, float m32) :
				Float4X3(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32) {}
			explicit Float4X3A(/*_In_reads_(12) */const float* pArray) : Float4X3(pArray) {}
		};

		//------------------------------------------------------------------------------
		// 3x4 Column-major Matrix: 32 bit floating point components
		struct Float3X4
		{
			union
			{
				struct
				{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
				};
				float m[3][4];
				float f[12];
			};

			Float3X4() = default;

			Float3X4(const Float3X4&) = default;
			Float3X4& operator=(const Float3X4&) = default;

			Float3X4(Float3X4&&) = default;
			Float3X4& operator=(Float3X4&&) = default;

			XM_CONSTEXPR Float3X4(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23)
				: _11(m00), _12(m01), _13(m02), _14(m03),
				_21(m10), _22(m11), _23(m12), _24(m13),
				_31(m20), _32(m21), _33(m22), _34(m23) {}
			explicit Float3X4(/*_In_reads_(12) */const float* pArray);

			float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
			float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }
		};

		// 3x4 Column-major Matrix: 32 bit floating point components aligned on a 16 byte boundary
		XM_ALIGNED(16) struct Float3X4A : public Float3X4
		{
			Float3X4A() = default;

			Float3X4A(const Float3X4A&) = default;
			Float3X4A& operator=(const Float3X4A&) = default;

			Float3X4A(Float3X4A&&) = default;
			Float3X4A& operator=(Float3X4A&&) = default;

			XM_CONSTEXPR Float3X4A(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23) :
				Float3X4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23) {}
			explicit Float3X4A(/*_In_reads_(12) */const float* pArray) : Float3X4(pArray) {}
		};

		//------------------------------------------------------------------------------
		// 4x4 Matrix: 32 bit floating point components
		struct Float4X4
		{
			union
			{
				struct
				{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
					float _41, _42, _43, _44;
				};
				float m[4][4];
			};

			Float4X4() = default;

			Float4X4(const Float4X4&) = default;
			Float4X4& operator=(const Float4X4&) = default;

			Float4X4(Float4X4&&) = default;
			Float4X4& operator=(Float4X4&&) = default;

			XM_CONSTEXPR Float4X4(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33)
				: _11(m00), _12(m01), _13(m02), _14(m03),
				_21(m10), _22(m11), _23(m12), _24(m13),
				_31(m20), _32(m21), _33(m22), _34(m23),
				_41(m30), _42(m31), _43(m32), _44(m33) {}
			explicit Float4X4(/*_In_reads_(16) */const float* pArray);

			float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
			float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }
		};

		// 4x4 Matrix: 32 bit floating point components aligned on a 16 byte boundary
		XM_ALIGNED(16) struct Float4X4A : public Float4X4
		{
			Float4X4A() = default;

			Float4X4A(const Float4X4A&) = default;
			Float4X4A& operator=(const Float4X4A&) = default;

			Float4X4A(Float4X4A&&) = default;
			Float4X4A& operator=(Float4X4A&&) = default;

			XM_CONSTEXPR Float4X4A(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33)
				: Float4X4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) {}
			explicit Float4X4A(/*_In_reads_(16) */const float* pArray) : Float4X4(pArray) {}
		};

		////////////////////////////////////////////////////////////////////////////////

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

#pragma warning(pop)

/****************************************************************************
 *
 * Data conversion operations
 *
 ****************************************************************************/

		Vector    XM_CALLCONV     ConvertVectorIntToFloat(FVector VInt, uint32_t DivExponent);
		Vector    XM_CALLCONV     ConvertVectorFloatToInt(FVector VFloat, uint32_t MulExponent);
		Vector    XM_CALLCONV     ConvertVectorUIntToFloat(FVector VUInt, uint32_t DivExponent);
		Vector    XM_CALLCONV     ConvertVectorFloatToUInt(FVector VFloat, uint32_t MulExponent);

#if defined(__XNAMATH_H__) && defined(VectorSetBinaryConstant)
#undef VectorSetBinaryConstant
#undef VectorSplatConstant
#undef VectorSplatConstantInt
#endif

		Vector    XM_CALLCONV     VectorSetBinaryConstant(uint32_t C0, uint32_t C1, uint32_t C2, uint32_t C3);
		Vector    XM_CALLCONV     VectorSplatConstant(int32_t IntConstant, uint32_t DivExponent);
		Vector    XM_CALLCONV     VectorSplatConstantInt(int32_t IntConstant);

		/****************************************************************************
		 *
		 * Load operations
		 *
		 ****************************************************************************/

		Vector    XM_CALLCONV     LoadInt(/*_In_ */const uint32_t* pSource);
		Vector    XM_CALLCONV     LoadFloat(/*_In_ */const float* pSource);

		Vector    XM_CALLCONV     LoadInt2(/*_In_reads_(2) */const uint32_t* pSource);
		Vector    XM_CALLCONV     LoadInt2A(/*_In_reads_(2) */const uint32_t* PSource);
		Vector    XM_CALLCONV     LoadFloat2(/*_In_ */const Float2* pSource);
		Vector    XM_CALLCONV     LoadFloat2A(/*_In_ */const Float2A* pSource);
		Vector    XM_CALLCONV     LoadSInt2(/*_In_ */const Int2* pSource);
		Vector    XM_CALLCONV     LoadUInt2(/*_In_ */const UInt2* pSource);

		Vector    XM_CALLCONV     LoadInt3(/*_In_reads_(3) */const uint32_t* pSource);
		Vector    XM_CALLCONV     LoadInt3A(/*_In_reads_(3) */const uint32_t* pSource);
		Vector    XM_CALLCONV     LoadFloat3(/*_In_ */const Float3* pSource);
		Vector    XM_CALLCONV     LoadFloat3A(/*_In_ */const Float3A* pSource);
		Vector    XM_CALLCONV     LoadSInt3(/*_In_ */const Int3* pSource);
		Vector    XM_CALLCONV     LoadUInt3(/*_In_ */const UInt3* pSource);

		Vector    XM_CALLCONV     LoadInt4(/*_In_reads_(4) */const uint32_t* pSource);
		Vector    XM_CALLCONV     LoadInt4A(/*_In_reads_(4) */const uint32_t* pSource);
		Vector    XM_CALLCONV     LoadFloat4(/*_In_ */const Float4* pSource);
		Vector    XM_CALLCONV     LoadFloat4A(/*_In_ */const Float4A* pSource);
		Vector    XM_CALLCONV     LoadSInt4(/*_In_ */const Int4* pSource);
		Vector    XM_CALLCONV     LoadUInt4(/*_In_ */const UInt4* pSource);

		Matrix    XM_CALLCONV     LoadFloat3x3(/*_In_ */const Float3X3* pSource);
		Matrix    XM_CALLCONV     LoadFloat4x3(/*_In_ */const Float4X3* pSource);
		Matrix    XM_CALLCONV     LoadFloat4x3A(/*_In_ */const Float4X3A* pSource);
		Matrix    XM_CALLCONV     LoadFloat3x4(/*_In_ */const Float3X4* pSource);
		Matrix    XM_CALLCONV     LoadFloat3x4A(/*_In_ */const Float3X4A* pSource);
		Matrix    XM_CALLCONV     LoadFloat4x4(/*_In_ */const Float4X4* pSource);
		Matrix    XM_CALLCONV     LoadFloat4x4A(/*_In_ */const Float4X4A* pSource);

		/****************************************************************************
		 *
		 * Store operations
		 *
		 ****************************************************************************/

		void        XM_CALLCONV     StoreInt(/*_Out_ */ uint32_t* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreFloat(/*_Out_ */ float* pDestination, /*_In_ */FVector V);

		void        XM_CALLCONV     StoreInt2(/*_Out_writes_(2)*/ uint32_t* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreInt2A(/*_Out_writes_(2)*/ uint32_t* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreFloat2(/*_Out_ */ Float2* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreFloat2A(/*_Out_ */ Float2A* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreSInt2(/*_Out_ */ Int2* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreUInt2(/*_Out_ */ UInt2* pDestination, /*_In_ */FVector V);

		void        XM_CALLCONV     StoreInt3(/*_Out_writes_(3) */uint32_t* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreInt3A(/*_Out_writes_(3)*/ uint32_t* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreFloat3(/*_Out_ */ Float3* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreFloat3A(/*_Out_ */ Float3A* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreSInt3(/*_Out_ */ Int3* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreUInt3(/*_Out_ */ UInt3* pDestination, /*_In_ */FVector V);

		void        XM_CALLCONV     StoreInt4(/*_Out_writes_(4) */uint32_t* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreInt4A(/*_Out_writes_(4) */uint32_t* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreFloat4(/*_Out_ */ Float4* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreFloat4A(/*_Out_ */ Float4A* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreSInt4(/*_Out_ */ Int4* pDestination, /*_In_ */FVector V);
		void        XM_CALLCONV     StoreUInt4(/*_Out_ */ UInt4* pDestination, /*_In_ */FVector V);

		void        XM_CALLCONV     StoreFloat3x3(/*_Out_ */ Float3X3* pDestination, /*_In_ */FMatrix M);
		void        XM_CALLCONV     StoreFloat4x3(/*_Out_ */ Float4X3* pDestination, /*_In_ */FMatrix M);
		void        XM_CALLCONV     StoreFloat4x3A(/*_Out_ */ Float4X3A* pDestination, /*_In_ */FMatrix M);
		void        XM_CALLCONV     StoreFloat3x4(/*_Out_ */ Float3X4* pDestination, /*_In_ */FMatrix M);
		void        XM_CALLCONV     StoreFloat3x4A(/*_Out_ */ Float3X4A* pDestination, /*_In_ */FMatrix M);
		void        XM_CALLCONV     StoreFloat4x4(/*_Out_ */ Float4X4* pDestination, /*_In_ */FMatrix M);
		void        XM_CALLCONV     StoreFloat4x4A(/*_Out_ */ Float4X4A* pDestination, /*_In_ */FMatrix M);

		/****************************************************************************
		 *
		 * General vector operations
		 *
		 ****************************************************************************/

		Vector    XM_CALLCONV     VectorZero();
		Vector    XM_CALLCONV     VectorSet(float x, float y, float z, float w);
		Vector    XM_CALLCONV     VectorSetInt(uint32_t x, uint32_t y, uint32_t z, uint32_t w);
		Vector    XM_CALLCONV     VectorReplicate(float Value);
		Vector    XM_CALLCONV     VectorReplicatePtr(/*_In_ */const float* pValue);
		Vector    XM_CALLCONV     VectorReplicateInt(uint32_t Value);
		Vector    XM_CALLCONV     VectorReplicateIntPtr(/*_In_ */const uint32_t* pValue);
		Vector    XM_CALLCONV     VectorTrueInt();
		Vector    XM_CALLCONV     VectorFalseInt();
		Vector    XM_CALLCONV     VectorSplatX(FVector V);
		Vector    XM_CALLCONV     VectorSplatY(FVector V);
		Vector    XM_CALLCONV     VectorSplatZ(FVector V);
		Vector    XM_CALLCONV     VectorSplatW(FVector V);
		Vector    XM_CALLCONV     VectorSplatOne();
		Vector    XM_CALLCONV     VectorSplatInfinity();
		Vector    XM_CALLCONV     VectorSplatQNaN();
		Vector    XM_CALLCONV     VectorSplatEpsilon();
		Vector    XM_CALLCONV     VectorSplatSignMask();

		float       XM_CALLCONV     VectorGetByIndex(FVector V, size_t i);
		float       XM_CALLCONV     VectorGetX(FVector V);
		float       XM_CALLCONV     VectorGetY(FVector V);
		float       XM_CALLCONV     VectorGetZ(FVector V);
		float       XM_CALLCONV     VectorGetW(FVector V);

		void        XM_CALLCONV     VectorGetByIndexPtr(/*_Out_ */ float* f, /*_In_ */FVector V, /*_In_ */size_t i);
		void        XM_CALLCONV     VectorGetXPtr(/*_Out_ */ float* x, /*_In_ */FVector V);
		void        XM_CALLCONV     VectorGetYPtr(/*_Out_ */ float* y, /*_In_ */FVector V);
		void        XM_CALLCONV     VectorGetZPtr(/*_Out_ */ float* z, /*_In_ */FVector V);
		void        XM_CALLCONV     VectorGetWPtr(/*_Out_ */ float* w, /*_In_ */FVector V);

		uint32_t    XM_CALLCONV     VectorGetIntByIndex(FVector V, size_t i);
		uint32_t    XM_CALLCONV     VectorGetIntX(FVector V);
		uint32_t    XM_CALLCONV     VectorGetIntY(FVector V);
		uint32_t    XM_CALLCONV     VectorGetIntZ(FVector V);
		uint32_t    XM_CALLCONV     VectorGetIntW(FVector V);

		void        XM_CALLCONV     VectorGetIntByIndexPtr(/*_Out_ */ uint32_t* x, /*_In_ */FVector V, /*_In_ */size_t i);
		void        XM_CALLCONV     VectorGetIntXPtr(/*_Out_ */ uint32_t* x, /*_In_ */FVector V);
		void        XM_CALLCONV     VectorGetIntYPtr(/*_Out_ */ uint32_t* y, /*_In_ */FVector V);
		void        XM_CALLCONV     VectorGetIntZPtr(/*_Out_ */ uint32_t* z, /*_In_ */FVector V);
		void        XM_CALLCONV     VectorGetIntWPtr(/*_Out_ */ uint32_t* w, /*_In_ */FVector V);

		Vector    XM_CALLCONV     VectorSetByIndex(FVector V, float f, size_t i);
		Vector    XM_CALLCONV     VectorSetX(FVector V, float x);
		Vector    XM_CALLCONV     VectorSetY(FVector V, float y);
		Vector    XM_CALLCONV     VectorSetZ(FVector V, float z);
		Vector    XM_CALLCONV     VectorSetW(FVector V, float w);

		Vector    XM_CALLCONV     VectorSetByIndexPtr(/*_In_ */FVector V, /*_In_ */const float* f, /*_In_ */size_t i);
		Vector    XM_CALLCONV     VectorSetXPtr(/*_In_ */FVector V, /*_In_ */const float* x);
		Vector    XM_CALLCONV     VectorSetYPtr(/*_In_ */FVector V, /*_In_ */const float* y);
		Vector    XM_CALLCONV     VectorSetZPtr(/*_In_ */FVector V, /*_In_ */const float* z);
		Vector    XM_CALLCONV     VectorSetWPtr(/*_In_ */FVector V, /*_In_ */const float* w);

		Vector    XM_CALLCONV     VectorSetIntByIndex(FVector V, uint32_t x, size_t i);
		Vector    XM_CALLCONV     VectorSetIntX(FVector V, uint32_t x);
		Vector    XM_CALLCONV     VectorSetIntY(FVector V, uint32_t y);
		Vector    XM_CALLCONV     VectorSetIntZ(FVector V, uint32_t z);
		Vector    XM_CALLCONV     VectorSetIntW(FVector V, uint32_t w);

		Vector    XM_CALLCONV     VectorSetIntByIndexPtr(/*_In_ */FVector V, /*_In_ */const uint32_t* x, /*_In_ */size_t i);
		Vector    XM_CALLCONV     VectorSetIntXPtr(/*_In_ */FVector V, /*_In_ */const uint32_t* x);
		Vector    XM_CALLCONV     VectorSetIntYPtr(/*_In_ */FVector V, /*_In_ */const uint32_t* y);
		Vector    XM_CALLCONV     VectorSetIntZPtr(/*_In_ */FVector V, /*_In_ */const uint32_t* z);
		Vector    XM_CALLCONV     VectorSetIntWPtr(/*_In_ */FVector V, /*_In_ */const uint32_t* w);

#if defined(__XNAMATH_H__) && defined(VectorSwizzle)
#undef VectorSwizzle
#endif

		Vector    XM_CALLCONV     VectorSwizzle(FVector V, uint32_t E0, uint32_t E1, uint32_t E2, uint32_t E3);
		Vector    XM_CALLCONV     VectorPermute(FVector V1, FVector V2, uint32_t PermuteX, uint32_t PermuteY, uint32_t PermuteZ, uint32_t PermuteW);
		Vector    XM_CALLCONV     VectorSelectControl(uint32_t VectorIndex0, uint32_t VectorIndex1, uint32_t VectorIndex2, uint32_t VectorIndex3);
		Vector    XM_CALLCONV     VectorSelect(FVector V1, FVector V2, FVector Control);
		Vector    XM_CALLCONV     VectorMergeXY(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorMergeZW(FVector V1, FVector V2);

#if defined(__XNAMATH_H__) && defined(VectorShiftLeft)
#undef VectorShiftLeft
#undef VectorRotateLeft
#undef VectorRotateRight
#undef VectorInsert
#endif

		Vector    XM_CALLCONV     VectorShiftLeft(FVector V1, FVector V2, uint32_t Elements);
		Vector    XM_CALLCONV     VectorRotateLeft(FVector V, uint32_t Elements);
		Vector    XM_CALLCONV     VectorRotateRight(FVector V, uint32_t Elements);
		Vector    XM_CALLCONV     VectorInsert(FVector VD, FVector VS, uint32_t VSLeftRotateElements,
			uint32_t Select0, uint32_t Select1, uint32_t Select2, uint32_t Select3);

		Vector    XM_CALLCONV     VectorEqual(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorEqualR(/*_Out_ */ uint32_t* pCR, /*_In_ */FVector V1, /*_In_ */FVector V2);
		Vector    XM_CALLCONV     VectorEqualInt(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorEqualIntR(/*_Out_ */ uint32_t* pCR, /*_In_ */FVector V, /*_In_ */FVector V2);
		Vector    XM_CALLCONV     VectorNearEqual(FVector V1, FVector V2, FVector Epsilon);
		Vector    XM_CALLCONV     VectorNotEqual(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorNotEqualInt(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorGreater(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorGreaterR(/*_Out_ */ uint32_t* pCR, /*_In_ */FVector V1, /*_In_ */FVector V2);
		Vector    XM_CALLCONV     VectorGreaterOrEqual(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorGreaterOrEqualR(/*_Out_ */ uint32_t* pCR, /*_In_ */FVector V1, /*_In_ */FVector V2);
		Vector    XM_CALLCONV     VectorLess(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorLessOrEqual(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorInBounds(FVector V, FVector Bounds);
		Vector    XM_CALLCONV     VectorInBoundsR(/*_Out_ */ uint32_t* pCR, /*_In_ */FVector V, /*_In_ */FVector Bounds);

		Vector    XM_CALLCONV     VectorIsNaN(FVector V);
		Vector    XM_CALLCONV     VectorIsInfinite(FVector V);

		Vector    XM_CALLCONV     VectorMin(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorMax(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorRound(FVector V);
		Vector    XM_CALLCONV     VectorTruncate(FVector V);
		Vector    XM_CALLCONV     VectorFloor(FVector V);
		Vector    XM_CALLCONV     VectorCeiling(FVector V);
		Vector    XM_CALLCONV     VectorClamp(FVector V, FVector Min, FVector Max);
		Vector    XM_CALLCONV     VectorSaturate(FVector V);

		Vector    XM_CALLCONV     VectorAndInt(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorAndCInt(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorOrInt(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorNorInt(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorXorInt(FVector V1, FVector V2);

		Vector    XM_CALLCONV     VectorNegate(FVector V);
		Vector    XM_CALLCONV     VectorAdd(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorSum(FVector V);
		Vector    XM_CALLCONV     VectorAddAngles(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorSubtract(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorSubtractAngles(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorMultiply(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorMultiplyAdd(FVector V1, FVector V2, FVector V3);
		Vector    XM_CALLCONV     VectorDivide(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorNegativeMultiplySubtract(FVector V1, FVector V2, FVector V3);
		Vector    XM_CALLCONV     VectorScale(FVector V, float ScaleFactor);
		Vector    XM_CALLCONV     VectorReciprocalEst(FVector V);
		Vector    XM_CALLCONV     VectorReciprocal(FVector V);
		Vector    XM_CALLCONV     VectorSqrtEst(FVector V);
		Vector    XM_CALLCONV     VectorSqrt(FVector V);
		Vector    XM_CALLCONV     VectorReciprocalSqrtEst(FVector V);
		Vector    XM_CALLCONV     VectorReciprocalSqrt(FVector V);
		Vector    XM_CALLCONV     VectorExp2(FVector V);
		Vector    XM_CALLCONV     VectorExpE(FVector V);
		Vector    XM_CALLCONV     VectorExp(FVector V);
		Vector    XM_CALLCONV     VectorLog2(FVector V);
		Vector    XM_CALLCONV     VectorLogE(FVector V);
		Vector    XM_CALLCONV     VectorLog(FVector V);
		Vector    XM_CALLCONV     VectorPow(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorAbs(FVector V);
		Vector    XM_CALLCONV     VectorMod(FVector V1, FVector V2);
		Vector    XM_CALLCONV     VectorModAngles(FVector Angles);
		Vector    XM_CALLCONV     VectorSin(FVector V);
		Vector    XM_CALLCONV     VectorSinEst(FVector V);
		Vector    XM_CALLCONV     VectorCos(FVector V);
		Vector    XM_CALLCONV     VectorCosEst(FVector V);
		void        XM_CALLCONV     VectorSinCos(/*_Out_ */ Vector* pSin, /*_Out_ */ Vector* pCos, /*_In_ */FVector V);
		void        XM_CALLCONV     VectorSinCosEst(/*_Out_ */ Vector* pSin, /*_Out_ */ Vector* pCos, /*_In_ */FVector V);
		Vector    XM_CALLCONV     VectorTan(FVector V);
		Vector    XM_CALLCONV     VectorTanEst(FVector V);
		Vector    XM_CALLCONV     VectorSinH(FVector V);
		Vector    XM_CALLCONV     VectorCosH(FVector V);
		Vector    XM_CALLCONV     VectorTanH(FVector V);
		Vector    XM_CALLCONV     VectorASin(FVector V);
		Vector    XM_CALLCONV     VectorASinEst(FVector V);
		Vector    XM_CALLCONV     VectorACos(FVector V);
		Vector    XM_CALLCONV     VectorACosEst(FVector V);
		Vector    XM_CALLCONV     VectorATan(FVector V);
		Vector    XM_CALLCONV     VectorATanEst(FVector V);
		Vector    XM_CALLCONV     VectorATan2(FVector Y, FVector X);
		Vector    XM_CALLCONV     VectorATan2Est(FVector Y, FVector X);
		Vector    XM_CALLCONV     VectorLerp(FVector V0, FVector V1, float t);
		Vector    XM_CALLCONV     VectorLerpV(FVector V0, FVector V1, FVector T);
		Vector    XM_CALLCONV     VectorHermite(FVector Position0, FVector Tangent0, FVector Position1, GVector Tangent1, float t);
		Vector    XM_CALLCONV     VectorHermiteV(FVector Position0, FVector Tangent0, FVector Position1, GVector Tangent1, HVector T);
		Vector    XM_CALLCONV     VectorCatmullRom(FVector Position0, FVector Position1, FVector Position2, GVector Position3, float t);
		Vector    XM_CALLCONV     VectorCatmullRomV(FVector Position0, FVector Position1, FVector Position2, GVector Position3, HVector T);
		Vector    XM_CALLCONV     VectorBaryCentric(FVector Position0, FVector Position1, FVector Position2, float f, float g);
		Vector    XM_CALLCONV     VectorBaryCentricV(FVector Position0, FVector Position1, FVector Position2, GVector F, HVector G);

		/****************************************************************************
		 *
		 * 2D vector operations
		 *
		 ****************************************************************************/

		bool        XM_CALLCONV     Vector2Equal(FVector V1, FVector V2);
		uint32_t    XM_CALLCONV     Vector2EqualR(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector2EqualInt(FVector V1, FVector V2);
		uint32_t    XM_CALLCONV     Vector2EqualIntR(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector2NearEqual(FVector V1, FVector V2, FVector Epsilon);
		bool        XM_CALLCONV     Vector2NotEqual(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector2NotEqualInt(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector2Greater(FVector V1, FVector V2);
		uint32_t    XM_CALLCONV     Vector2GreaterR(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector2GreaterOrEqual(FVector V1, FVector V2);
		uint32_t    XM_CALLCONV     Vector2GreaterOrEqualR(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector2Less(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector2LessOrEqual(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector2InBounds(FVector V, FVector Bounds);

		bool        XM_CALLCONV     Vector2IsNaN(FVector V);
		bool        XM_CALLCONV     Vector2IsInfinite(FVector V);

		Vector    XM_CALLCONV     Vector2Dot(FVector V1, FVector V2);
		Vector    XM_CALLCONV     Vector2Cross(FVector V1, FVector V2);
		Vector    XM_CALLCONV     Vector2LengthSq(FVector V);
		Vector    XM_CALLCONV     Vector2ReciprocalLengthEst(FVector V);
		Vector    XM_CALLCONV     Vector2ReciprocalLength(FVector V);
		Vector    XM_CALLCONV     Vector2LengthEst(FVector V);
		Vector    XM_CALLCONV     Vector2Length(FVector V);
		Vector    XM_CALLCONV     Vector2NormalizeEst(FVector V);
		Vector    XM_CALLCONV     Vector2Normalize(FVector V);
		Vector    XM_CALLCONV     Vector2ClampLength(FVector V, float LengthMin, float LengthMax);
		Vector    XM_CALLCONV     Vector2ClampLengthV(FVector V, FVector LengthMin, FVector LengthMax);
		Vector    XM_CALLCONV     Vector2Reflect(FVector Incident, FVector Normal);
		Vector    XM_CALLCONV     Vector2Refract(FVector Incident, FVector Normal, float RefractionIndex);
		Vector    XM_CALLCONV     Vector2RefractV(FVector Incident, FVector Normal, FVector RefractionIndex);
		Vector    XM_CALLCONV     Vector2Orthogonal(FVector V);
		Vector    XM_CALLCONV     Vector2AngleBetweenNormalsEst(FVector N1, FVector N2);
		Vector    XM_CALLCONV     Vector2AngleBetweenNormals(FVector N1, FVector N2);
		Vector    XM_CALLCONV     Vector2AngleBetweenVectors(FVector V1, FVector V2);
		Vector    XM_CALLCONV     Vector2LinePointDistance(FVector LinePoint1, FVector LinePoint2, FVector Point);
		Vector    XM_CALLCONV     Vector2IntersectLine(FVector Line1Point1, FVector Line1Point2, FVector Line2Point1, GVector Line2Point2);
		Vector    XM_CALLCONV     Vector2Transform(FVector V, FMatrix M);
		Float4* XM_CALLCONV     Vector2TransformStream(/*_Out_writes_bytes_(sizeof(Float4) + OutputStride * (VectorCount - 1)) */Float4* pOutputStream,
			/*_In_ */size_t OutputStride,
			/*_In_reads_bytes_(sizeof(Float2) + InputStride * (VectorCount - 1)) */const Float2* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount, /*_In_ */FMatrix M);
		Vector    XM_CALLCONV     Vector2TransformCoord(FVector V, FMatrix M);
		Float2* XM_CALLCONV     Vector2TransformCoordStream(/*_Out_writes_bytes_(sizeof(Float2) + OutputStride * (VectorCount - 1)) */Float2* pOutputStream,
			/*_In_ */size_t OutputStride,
			/*_In_reads_bytes_(sizeof(Float2) + InputStride * (VectorCount - 1)) */const Float2* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount, /*_In_ */FMatrix M);
		Vector    XM_CALLCONV     Vector2TransformNormal(FVector V, FMatrix M);
		Float2* XM_CALLCONV     Vector2TransformNormalStream(/*_Out_writes_bytes_(sizeof(Float2) + OutputStride * (VectorCount - 1)) */Float2* pOutputStream,
			/*_In_ */size_t OutputStride,
			/*_In_reads_bytes_(sizeof(Float2) + InputStride * (VectorCount - 1)) */const Float2* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount, /*_In_ */FMatrix M);

		/****************************************************************************
		 *
		 * 3D vector operations
		 *
		 ****************************************************************************/

		bool        XM_CALLCONV     Vector3Equal(FVector V1, FVector V2);
		uint32_t    XM_CALLCONV     Vector3EqualR(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector3EqualInt(FVector V1, FVector V2);
		uint32_t    XM_CALLCONV     Vector3EqualIntR(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector3NearEqual(FVector V1, FVector V2, FVector Epsilon);
		bool        XM_CALLCONV     Vector3NotEqual(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector3NotEqualInt(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector3Greater(FVector V1, FVector V2);
		uint32_t    XM_CALLCONV     Vector3GreaterR(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector3GreaterOrEqual(FVector V1, FVector V2);
		uint32_t    XM_CALLCONV     Vector3GreaterOrEqualR(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector3Less(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector3LessOrEqual(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector3InBounds(FVector V, FVector Bounds);

		bool        XM_CALLCONV     Vector3IsNaN(FVector V);
		bool        XM_CALLCONV     Vector3IsInfinite(FVector V);

		Vector    XM_CALLCONV     Vector3Dot(FVector V1, FVector V2);
		Vector    XM_CALLCONV     Vector3Cross(FVector V1, FVector V2);
		Vector    XM_CALLCONV     Vector3LengthSq(FVector V);
		Vector    XM_CALLCONV     Vector3ReciprocalLengthEst(FVector V);
		Vector    XM_CALLCONV     Vector3ReciprocalLength(FVector V);
		Vector    XM_CALLCONV     Vector3LengthEst(FVector V);
		Vector    XM_CALLCONV     Vector3Length(FVector V);
		Vector    XM_CALLCONV     Vector3NormalizeEst(FVector V);
		Vector    XM_CALLCONV     Vector3Normalize(FVector V);
		Vector    XM_CALLCONV     Vector3ClampLength(FVector V, float LengthMin, float LengthMax);
		Vector    XM_CALLCONV     Vector3ClampLengthV(FVector V, FVector LengthMin, FVector LengthMax);
		Vector    XM_CALLCONV     Vector3Reflect(FVector Incident, FVector Normal);
		Vector    XM_CALLCONV     Vector3Refract(FVector Incident, FVector Normal, float RefractionIndex);
		Vector    XM_CALLCONV     Vector3RefractV(FVector Incident, FVector Normal, FVector RefractionIndex);
		Vector    XM_CALLCONV     Vector3Orthogonal(FVector V);
		Vector    XM_CALLCONV     Vector3AngleBetweenNormalsEst(FVector N1, FVector N2);
		Vector    XM_CALLCONV     Vector3AngleBetweenNormals(FVector N1, FVector N2);
		Vector    XM_CALLCONV     Vector3AngleBetweenVectors(FVector V1, FVector V2);
		Vector    XM_CALLCONV     Vector3LinePointDistance(FVector LinePoint1, FVector LinePoint2, FVector Point);
		void        XM_CALLCONV     Vector3ComponentsFromNormal(/*_Out_ */ Vector* pParallel, /*_Out_ */ Vector* pPerpendicular, /*_In_ */FVector V, /*_In_ */FVector Normal);
		Vector    XM_CALLCONV     Vector3Rotate(FVector V, FVector RotationQuaternion);
		Vector    XM_CALLCONV     Vector3InverseRotate(FVector V, FVector RotationQuaternion);
		Vector    XM_CALLCONV     Vector3Transform(FVector V, FMatrix M);
		Float4* XM_CALLCONV     Vector3TransformStream(/*_Out_writes_bytes_(sizeof(Float4) + OutputStride * (VectorCount - 1)) */Float4* pOutputStream,
			/*_In_ */size_t OutputStride,
			/* _In_reads_bytes_(sizeof(Float3) + InputStride * (VectorCount - 1)) */const Float3* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount, /*_In_ */FMatrix M);
		Vector    XM_CALLCONV     Vector3TransformCoord(FVector V, FMatrix M);
		Float3* XM_CALLCONV     Vector3TransformCoordStream(/*_Out_writes_bytes_(sizeof(Float3) + OutputStride * (VectorCount - 1)) */Float3* pOutputStream,
			/*_In_ */size_t OutputStride,
			/* _In_reads_bytes_(sizeof(Float3) + InputStride * (VectorCount - 1)) */const Float3* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount, /*_In_ */FMatrix M);
		Vector    XM_CALLCONV     Vector3TransformNormal(FVector V, FMatrix M);
		Float3* XM_CALLCONV     Vector3TransformNormalStream(/*_Out_writes_bytes_(sizeof(Float3) + OutputStride * (VectorCount - 1)) */Float3* pOutputStream,
			/*_In_ */size_t OutputStride,
			/* _In_reads_bytes_(sizeof(Float3) + InputStride * (VectorCount - 1)) */const Float3* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount, /*_In_ */FMatrix M);
		Vector    XM_CALLCONV     Vector3Project(FVector V, float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ,
			FMatrix Projection, CMatrix View, CMatrix World);
		Float3* XM_CALLCONV     Vector3ProjectStream(/*_Out_writes_bytes_(sizeof(Float3) + OutputStride * (VectorCount - 1)) */Float3* pOutputStream,
			/*_In_ */size_t OutputStride,
			/* _In_reads_bytes_(sizeof(Float3) + InputStride * (VectorCount - 1)) */const Float3* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount,
			/*_In_ */float ViewportX, /*_In_ */float ViewportY, /*_In_ */float ViewportWidth, /*_In_ */float ViewportHeight, /*_In_ */float ViewportMinZ, /*_In_ */float ViewportMaxZ,
			/*_In_ */FMatrix Projection, /*_In_ */CMatrix View, /*_In_ */CMatrix World);
		Vector    XM_CALLCONV     Vector3Unproject(FVector V, float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ,
			FMatrix Projection, CMatrix View, CMatrix World);
		Float3* XM_CALLCONV     Vector3UnprojectStream(/*_Out_writes_bytes_(sizeof(Float3) + OutputStride * (VectorCount - 1)) */Float3* pOutputStream,
			/*_In_ */size_t OutputStride,
			/* _In_reads_bytes_(sizeof(Float3) + InputStride * (VectorCount - 1)) */const Float3* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount,
			/*_In_ */float ViewportX, /*_In_ */float ViewportY, /*_In_ */float ViewportWidth, /*_In_ */float ViewportHeight, /*_In_ */float ViewportMinZ, /*_In_ */float ViewportMaxZ,
			/*_In_ */FMatrix Projection, /*_In_ */CMatrix View, /*_In_ */CMatrix World);

		/****************************************************************************
		 *
		 * 4D vector operations
		 *
		 ****************************************************************************/

		bool        XM_CALLCONV     Vector4Equal(FVector V1, FVector V2);
		uint32_t    XM_CALLCONV     Vector4EqualR(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector4EqualInt(FVector V1, FVector V2);
		uint32_t    XM_CALLCONV     Vector4EqualIntR(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector4NearEqual(FVector V1, FVector V2, FVector Epsilon);
		bool        XM_CALLCONV     Vector4NotEqual(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector4NotEqualInt(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector4Greater(FVector V1, FVector V2);
		uint32_t    XM_CALLCONV     Vector4GreaterR(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector4GreaterOrEqual(FVector V1, FVector V2);
		uint32_t    XM_CALLCONV     Vector4GreaterOrEqualR(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector4Less(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector4LessOrEqual(FVector V1, FVector V2);
		bool        XM_CALLCONV     Vector4InBounds(FVector V, FVector Bounds);

		bool        XM_CALLCONV     Vector4IsNaN(FVector V);
		bool        XM_CALLCONV     Vector4IsInfinite(FVector V);

		Vector    XM_CALLCONV     Vector4Dot(FVector V1, FVector V2);
		Vector    XM_CALLCONV     Vector4Cross(FVector V1, FVector V2, FVector V3);
		Vector    XM_CALLCONV     Vector4LengthSq(FVector V);
		Vector    XM_CALLCONV     Vector4ReciprocalLengthEst(FVector V);
		Vector    XM_CALLCONV     Vector4ReciprocalLength(FVector V);
		Vector    XM_CALLCONV     Vector4LengthEst(FVector V);
		Vector    XM_CALLCONV     Vector4Length(FVector V);
		Vector    XM_CALLCONV     Vector4NormalizeEst(FVector V);
		Vector    XM_CALLCONV     Vector4Normalize(FVector V);
		Vector    XM_CALLCONV     Vector4ClampLength(FVector V, float LengthMin, float LengthMax);
		Vector    XM_CALLCONV     Vector4ClampLengthV(FVector V, FVector LengthMin, FVector LengthMax);
		Vector    XM_CALLCONV     Vector4Reflect(FVector Incident, FVector Normal);
		Vector    XM_CALLCONV     Vector4Refract(FVector Incident, FVector Normal, float RefractionIndex);
		Vector    XM_CALLCONV     Vector4RefractV(FVector Incident, FVector Normal, FVector RefractionIndex);
		Vector    XM_CALLCONV     Vector4Orthogonal(FVector V);
		Vector    XM_CALLCONV     Vector4AngleBetweenNormalsEst(FVector N1, FVector N2);
		Vector    XM_CALLCONV     Vector4AngleBetweenNormals(FVector N1, FVector N2);
		Vector    XM_CALLCONV     Vector4AngleBetweenVectors(FVector V1, FVector V2);
		Vector    XM_CALLCONV     Vector4Transform(FVector V, FMatrix M);
		Float4* XM_CALLCONV     Vector4TransformStream(/*_Out_writes_bytes_(sizeof(Float4) + OutputStride * (VectorCount - 1)) */Float4* pOutputStream,
			/*_In_ */size_t OutputStride,
			/*_In_reads_bytes_(sizeof(Float4) + InputStride * (VectorCount - 1)) */const Float4* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount, /*_In_ */FMatrix M);

		/****************************************************************************
		 *
		 * Matrix operations
		 *
		 ****************************************************************************/

		bool        XM_CALLCONV     MatrixIsNaN(FMatrix M);
		bool        XM_CALLCONV     MatrixIsInfinite(FMatrix M);
		bool        XM_CALLCONV     MatrixIsIdentity(FMatrix M);

		Matrix    XM_CALLCONV     MatrixMultiply(FMatrix M1, CMatrix M2);
		Matrix    XM_CALLCONV     MatrixMultiplyTranspose(FMatrix M1, CMatrix M2);
		Matrix    XM_CALLCONV     MatrixTranspose(FMatrix M);
		Matrix    XM_CALLCONV     MatrixInverse(/*_Out_opt_ */Vector* pDeterminant, /*_In_ */FMatrix M);
		Vector    XM_CALLCONV     MatrixDeterminant(FMatrix M);
		_Success_(return)
			bool        XM_CALLCONV     MatrixDecompose(/*_Out_ */ Vector* outScale, /*_Out_ */ Vector* outRotQuat, /*_Out_ */ Vector* outTrans, /*_In_ */FMatrix M);

		Matrix    XM_CALLCONV     MatrixIdentity();
		Matrix    XM_CALLCONV     MatrixSet(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);
		Matrix    XM_CALLCONV     MatrixTranslation(float OffsetX, float OffsetY, float OffsetZ);
		Matrix    XM_CALLCONV     MatrixTranslationFromVector(FVector Offset);
		Matrix    XM_CALLCONV     MatrixScaling(float ScaleX, float ScaleY, float ScaleZ);
		Matrix    XM_CALLCONV     MatrixScalingFromVector(FVector Scale);
		Matrix    XM_CALLCONV     MatrixRotationX(float Angle);
		Matrix    XM_CALLCONV     MatrixRotationY(float Angle);
		Matrix    XM_CALLCONV     MatrixRotationZ(float Angle);
		Matrix    XM_CALLCONV     MatrixRotationRollPitchYaw(float Pitch, float Yaw, float Roll);
		Matrix    XM_CALLCONV     MatrixRotationRollPitchYawFromVector(FVector Angles);
		Matrix    XM_CALLCONV     MatrixRotationNormal(FVector NormalAxis, float Angle);
		Matrix    XM_CALLCONV     MatrixRotationAxis(FVector Axis, float Angle);
		Matrix    XM_CALLCONV     MatrixRotationQuaternion(FVector Quaternion);
		Matrix    XM_CALLCONV     MatrixTransformation2D(FVector ScalingOrigin, float ScalingOrientation, FVector Scaling,
			FVector RotationOrigin, float Rotation, GVector Translation);
		Matrix    XM_CALLCONV     MatrixTransformation(FVector ScalingOrigin, FVector ScalingOrientationQuaternion, FVector Scaling,
			GVector RotationOrigin, HVector RotationQuaternion, HVector Translation);
		Matrix    XM_CALLCONV     MatrixAffineTransformation2D(FVector Scaling, FVector RotationOrigin, float Rotation, FVector Translation);
		Matrix    XM_CALLCONV     MatrixAffineTransformation(FVector Scaling, FVector RotationOrigin, FVector RotationQuaternion, GVector Translation);
		Matrix    XM_CALLCONV     MatrixReflect(FVector ReflectionPlane);
		Matrix    XM_CALLCONV     MatrixShadow(FVector ShadowPlane, FVector LightPosition);

		Matrix    XM_CALLCONV     MatrixLookAtLH(FVector EyePosition, FVector FocusPosition, FVector UpDirection);
		Matrix    XM_CALLCONV     MatrixLookAtRH(FVector EyePosition, FVector FocusPosition, FVector UpDirection);
		Matrix    XM_CALLCONV     MatrixLookToLH(FVector EyePosition, FVector EyeDirection, FVector UpDirection);
		Matrix    XM_CALLCONV     MatrixLookToRH(FVector EyePosition, FVector EyeDirection, FVector UpDirection);
		Matrix    XM_CALLCONV     MatrixPerspectiveLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
		Matrix    XM_CALLCONV     MatrixPerspectiveRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
		Matrix    XM_CALLCONV     MatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ);
		Matrix    XM_CALLCONV     MatrixPerspectiveFovRH(float FovAngleY, float AspectRatio, float NearZ, float FarZ);
		Matrix    XM_CALLCONV     MatrixPerspectiveOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);
		Matrix    XM_CALLCONV     MatrixPerspectiveOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);
		Matrix    XM_CALLCONV     MatrixOrthographicLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
		Matrix    XM_CALLCONV     MatrixOrthographicRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
		Matrix    XM_CALLCONV     MatrixOrthographicOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);
		Matrix    XM_CALLCONV     MatrixOrthographicOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);


		/****************************************************************************
		 *
		 * Quaternion operations
		 *
		 ****************************************************************************/

		bool        XM_CALLCONV     QuaternionEqual(FVector Q1, FVector Q2);
		bool        XM_CALLCONV     QuaternionNotEqual(FVector Q1, FVector Q2);

		bool        XM_CALLCONV     QuaternionIsNaN(FVector Q);
		bool        XM_CALLCONV     QuaternionIsInfinite(FVector Q);
		bool        XM_CALLCONV     QuaternionIsIdentity(FVector Q);

		Vector    XM_CALLCONV     QuaternionDot(FVector Q1, FVector Q2);
		Vector    XM_CALLCONV     QuaternionMultiply(FVector Q1, FVector Q2);
		Vector    XM_CALLCONV     QuaternionLengthSq(FVector Q);
		Vector    XM_CALLCONV     QuaternionReciprocalLength(FVector Q);
		Vector    XM_CALLCONV     QuaternionLength(FVector Q);
		Vector    XM_CALLCONV     QuaternionNormalizeEst(FVector Q);
		Vector    XM_CALLCONV     QuaternionNormalize(FVector Q);
		Vector    XM_CALLCONV     QuaternionConjugate(FVector Q);
		Vector    XM_CALLCONV     QuaternionInverse(FVector Q);
		Vector    XM_CALLCONV     QuaternionLn(FVector Q);
		Vector    XM_CALLCONV     QuaternionExp(FVector Q);
		Vector    XM_CALLCONV     QuaternionSlerp(FVector Q0, FVector Q1, float t);
		Vector    XM_CALLCONV     QuaternionSlerpV(FVector Q0, FVector Q1, FVector T);
		Vector    XM_CALLCONV     QuaternionSquad(FVector Q0, FVector Q1, FVector Q2, GVector Q3, float t);
		Vector    XM_CALLCONV     QuaternionSquadV(FVector Q0, FVector Q1, FVector Q2, GVector Q3, HVector T);
		void        XM_CALLCONV     QuaternionSquadSetup(/*_Out_ */ Vector* pA, /*_Out_ */ Vector* pB, /*_Out_ */ Vector* pC, /*_In_ */FVector Q0, /*_In_ */FVector Q1, /*_In_ */FVector Q2, /*_In_ */GVector Q3);
		Vector    XM_CALLCONV     QuaternionBaryCentric(FVector Q0, FVector Q1, FVector Q2, float f, float g);
		Vector    XM_CALLCONV     QuaternionBaryCentricV(FVector Q0, FVector Q1, FVector Q2, GVector F, HVector G);

		Vector    XM_CALLCONV     QuaternionIdentity();
		Vector    XM_CALLCONV     QuaternionRotationRollPitchYaw(float Pitch, float Yaw, float Roll);
		Vector    XM_CALLCONV     QuaternionRotationRollPitchYawFromVector(FVector Angles);
		Vector    XM_CALLCONV     QuaternionRotationNormal(FVector NormalAxis, float Angle);
		Vector    XM_CALLCONV     QuaternionRotationAxis(FVector Axis, float Angle);
		Vector    XM_CALLCONV     QuaternionRotationMatrix(FMatrix M);

		void        XM_CALLCONV     QuaternionToAxisAngle(/*_Out_ */ Vector* pAxis, /*_Out_ */ float* pAngle, /*_In_ */FVector Q);

		/****************************************************************************
		 *
		 * Plane operations
		 *
		 ****************************************************************************/

		bool        XM_CALLCONV     PlaneEqual(FVector P1, FVector P2);
		bool        XM_CALLCONV     PlaneNearEqual(FVector P1, FVector P2, FVector Epsilon);
		bool        XM_CALLCONV     PlaneNotEqual(FVector P1, FVector P2);

		bool        XM_CALLCONV     PlaneIsNaN(FVector P);
		bool        XM_CALLCONV     PlaneIsInfinite(FVector P);

		Vector    XM_CALLCONV     PlaneDot(FVector P, FVector V);
		Vector    XM_CALLCONV     PlaneDotCoord(FVector P, FVector V);
		Vector    XM_CALLCONV     PlaneDotNormal(FVector P, FVector V);
		Vector    XM_CALLCONV     PlaneNormalizeEst(FVector P);
		Vector    XM_CALLCONV     PlaneNormalize(FVector P);
		Vector    XM_CALLCONV     PlaneIntersectLine(FVector P, FVector LinePoint1, FVector LinePoint2);
		void        XM_CALLCONV     PlaneIntersectPlane(/*_Out_ */ Vector* pLinePoint1, /*_Out_ */ Vector* pLinePoint2, /*_In_ */FVector P1, /*_In_ */FVector P2);
		Vector    XM_CALLCONV     PlaneTransform(FVector P, FMatrix M);
		Float4* XM_CALLCONV     PlaneTransformStream(/*_Out_writes_bytes_(sizeof(Float4) + OutputStride * (PlaneCount - 1)) */Float4* pOutputStream,
			/*_In_ */size_t OutputStride,
			/*_In_reads_bytes_(sizeof(Float4) + InputStride * (PlaneCount - 1)) */const Float4* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t PlaneCount, /*_In_ */FMatrix M);

		Vector    XM_CALLCONV     PlaneFromPointNormal(FVector Point, FVector Normal);
		Vector    XM_CALLCONV     PlaneFromPoints(FVector Point1, FVector Point2, FVector Point3);

		/****************************************************************************
		 *
		 * Color operations
		 *
		 ****************************************************************************/

		bool        XM_CALLCONV     ColorEqual(FVector C1, FVector C2);
		bool        XM_CALLCONV     ColorNotEqual(FVector C1, FVector C2);
		bool        XM_CALLCONV     ColorGreater(FVector C1, FVector C2);
		bool        XM_CALLCONV     ColorGreaterOrEqual(FVector C1, FVector C2);
		bool        XM_CALLCONV     ColorLess(FVector C1, FVector C2);
		bool        XM_CALLCONV     ColorLessOrEqual(FVector C1, FVector C2);

		bool        XM_CALLCONV     ColorIsNaN(FVector C);
		bool        XM_CALLCONV     ColorIsInfinite(FVector C);

		Vector    XM_CALLCONV     ColorNegative(FVector C);
		Vector    XM_CALLCONV     ColorModulate(FVector C1, FVector C2);
		Vector    XM_CALLCONV     ColorAdjustSaturation(FVector C, float Saturation);
		Vector    XM_CALLCONV     ColorAdjustContrast(FVector C, float Contrast);

		Vector    XM_CALLCONV     ColorRGBToHSL(FVector rgb);
		Vector    XM_CALLCONV     ColorHSLToRGB(FVector hsl);

		Vector    XM_CALLCONV     ColorRGBToHSV(FVector rgb);
		Vector    XM_CALLCONV     ColorHSVToRGB(FVector hsv);

		Vector    XM_CALLCONV     ColorRGBToYUV(FVector rgb);
		Vector    XM_CALLCONV     ColorYUVToRGB(FVector yuv);

		Vector    XM_CALLCONV     ColorRGBToYUV_HD(FVector rgb);
		Vector    XM_CALLCONV     ColorYUVToRGB_HD(FVector yuv);

		Vector    XM_CALLCONV     ColorRGBToXYZ(FVector rgb);
		Vector    XM_CALLCONV     ColorXYZToRGB(FVector xyz);

		Vector    XM_CALLCONV     ColorXYZToSRGB(FVector xyz);
		Vector    XM_CALLCONV     ColorSRGBToXYZ(FVector srgb);

		Vector    XM_CALLCONV     ColorRGBToSRGB(FVector rgb);
		Vector    XM_CALLCONV     ColorSRGBToRGB(FVector srgb);


		/****************************************************************************
		 *
		 * Miscellaneous operations
		 *
		 ****************************************************************************/

		bool            VerifyCPUSupport();

		Vector    XM_CALLCONV     FresnelTerm(FVector CosIncidentAngle, FVector RefractionIndex);

		bool            ScalarNearEqual(float S1, float S2, float Epsilon);
		float           ScalarModAngle(float Value);

		float           ScalarSin(float Value);
		float           ScalarSinEst(float Value);

		float           ScalarCos(float Value);
		float           ScalarCosEst(float Value);

		void            ScalarSinCos(/*_Out_ */ float* pSin, /*_Out_ */ float* pCos, float Value);
		void            ScalarSinCosEst(/*_Out_ */ float* pSin, /*_Out_ */ float* pCos, float Value);

		float           ScalarASin(float Value);
		float           ScalarASinEst(float Value);

		float           ScalarACos(float Value);
		float           ScalarACosEst(float Value);

		/****************************************************************************
		 *
		 * Templates
		 *
		 ****************************************************************************/

#if defined(__XNAMATH_H__) && defined(XMMin)
#undef XMMin
#undef XMMax
#endif

		template<class T> inline T XMMin(T a, T b) { return (a < b) ? a : b; }
		template<class T> inline T XMMax(T a, T b) { return (a > b) ? a : b; }

		//------------------------------------------------------------------------------

#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)

// PermuteHelper internal template (SSE only)
		namespace Internal
		{
			// Slow path fallback for permutes that do not map to a single SSE shuffle opcode.
			template<uint32_t Shuffle, bool WhichX, bool WhichY, bool WhichZ, bool WhichW> struct PermuteHelper
			{
				static Vector     XM_CALLCONV     Permute(FVector v1, FVector v2)
				{
					static const VectorU32 selectMask =
					{ { {
							WhichX ? 0xFFFFFFFF : 0,
							WhichY ? 0xFFFFFFFF : 0,
							WhichZ ? 0xFFFFFFFF : 0,
							WhichW ? 0xFFFFFFFF : 0,
					} } };

					Vector shuffled1 = XM_PERMUTE_PS(v1, Shuffle);
					Vector shuffled2 = XM_PERMUTE_PS(v2, Shuffle);

					Vector masked1 = _mm_andnot_ps(selectMask, shuffled1);
					Vector masked2 = _mm_and_ps(selectMask, shuffled2);

					return _mm_or_ps(masked1, masked2);
				}
			};

			// Fast path for permutes that only read from the first vector.
			template<uint32_t Shuffle> struct PermuteHelper<Shuffle, false, false, false, false>
			{
				static Vector     XM_CALLCONV     Permute(FVector v1, FVector) { return XM_PERMUTE_PS(v1, Shuffle); }
			};

			// Fast path for permutes that only read from the second vector.
			template<uint32_t Shuffle> struct PermuteHelper<Shuffle, true, true, true, true>
			{
				static Vector     XM_CALLCONV     Permute(FVector, FVector v2) { return XM_PERMUTE_PS(v2, Shuffle); }
			};

			// Fast path for permutes that read XY from the first vector, ZW from the second.
			template<uint32_t Shuffle> struct PermuteHelper<Shuffle, false, false, true, true>
			{
				static Vector     XM_CALLCONV     Permute(FVector v1, FVector v2) { return _mm_shuffle_ps(v1, v2, Shuffle); }
			};

			// Fast path for permutes that read XY from the second vector, ZW from the first.
			template<uint32_t Shuffle> struct PermuteHelper<Shuffle, true, true, false, false>
			{
				static Vector     XM_CALLCONV     Permute(FVector v1, FVector v2) { return _mm_shuffle_ps(v2, v1, Shuffle); }
			};
		}

#endif // _XM_SSE_INTRINSICS_ && !_XM_NO_INTRINSICS_

		// General permute template
		template<uint32_t PermuteX, uint32_t PermuteY, uint32_t PermuteZ, uint32_t PermuteW>
		inline Vector     XM_CALLCONV     VectorPermute(FVector V1, FVector V2)
		{
			static_assert(PermuteX <= 7, "PermuteX template parameter out of range");
			static_assert(PermuteY <= 7, "PermuteY template parameter out of range");
			static_assert(PermuteZ <= 7, "PermuteZ template parameter out of range");
			static_assert(PermuteW <= 7, "PermuteW template parameter out of range");

#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			const uint32_t Shuffle = _MM_SHUFFLE(PermuteW & 3, PermuteZ & 3, PermuteY & 3, PermuteX & 3);

			const bool WhichX = PermuteX > 3;
			const bool WhichY = PermuteY > 3;
			const bool WhichZ = PermuteZ > 3;
			const bool WhichW = PermuteW > 3;

			return Internal::PermuteHelper<Shuffle, WhichX, WhichY, WhichZ, WhichW>::Permute(V1, V2);
#else

			return VectorPermute(V1, V2, PermuteX, PermuteY, PermuteZ, PermuteW);

#endif
		}

		// Special-case permute templates
		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 1, 2, 3>(FVector V1, FVector) { return V1; }
		template<> inline Vector      XM_CALLCONV     VectorPermute<4, 5, 6, 7>(FVector, FVector V2) { return V2; }

#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 1, 4, 5>(FVector V1, FVector V2) { return _mm_movelh_ps(V1, V2); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<6, 7, 2, 3>(FVector V1, FVector V2) { return _mm_movehl_ps(V1, V2); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 4, 1, 5>(FVector V1, FVector V2) { return _mm_unpacklo_ps(V1, V2); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<2, 6, 3, 7>(FVector V1, FVector V2) { return _mm_unpackhi_ps(V1, V2); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<2, 3, 6, 7>(FVector V1, FVector V2) { return _mm_castpd_ps(_mm_unpackhi_pd(_mm_castps_pd(V1), _mm_castps_pd(V2))); }
#endif

#if defined(_XM_SSE4_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
		template<> inline Vector      XM_CALLCONV     VectorPermute<4, 1, 2, 3>(FVector V1, FVector V2) { return _mm_blend_ps(V1, V2, 0x1); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 5, 2, 3>(FVector V1, FVector V2) { return _mm_blend_ps(V1, V2, 0x2); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<4, 5, 2, 3>(FVector V1, FVector V2) { return _mm_blend_ps(V1, V2, 0x3); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 1, 6, 3>(FVector V1, FVector V2) { return _mm_blend_ps(V1, V2, 0x4); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<4, 1, 6, 3>(FVector V1, FVector V2) { return _mm_blend_ps(V1, V2, 0x5); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 5, 6, 3>(FVector V1, FVector V2) { return _mm_blend_ps(V1, V2, 0x6); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<4, 5, 6, 3>(FVector V1, FVector V2) { return _mm_blend_ps(V1, V2, 0x7); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 1, 2, 7>(FVector V1, FVector V2) { return _mm_blend_ps(V1, V2, 0x8); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<4, 1, 2, 7>(FVector V1, FVector V2) { return _mm_blend_ps(V1, V2, 0x9); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 5, 2, 7>(FVector V1, FVector V2) { return _mm_blend_ps(V1, V2, 0xA); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<4, 5, 2, 7>(FVector V1, FVector V2) { return _mm_blend_ps(V1, V2, 0xB); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 1, 6, 7>(FVector V1, FVector V2) { return _mm_blend_ps(V1, V2, 0xC); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<4, 1, 6, 7>(FVector V1, FVector V2) { return _mm_blend_ps(V1, V2, 0xD); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 5, 6, 7>(FVector V1, FVector V2) { return _mm_blend_ps(V1, V2, 0xE); }
#endif

#if defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)

		// If the indices are all in the range 0-3 or 4-7, then use VectorSwizzle instead
		// The mirror cases are not spelled out here as the programmer can always swap the arguments
		// (i.e. prefer permutes where the X element comes from the V1 vector instead of the V2 vector)

		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 1, 4, 5>(FVector V1, FVector V2) { return vcombine_f32(vget_low_f32(V1), vget_low_f32(V2)); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<1, 0, 4, 5>(FVector V1, FVector V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1)), vget_low_f32(V2)); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 1, 5, 4>(FVector V1, FVector V2) { return vcombine_f32(vget_low_f32(V1), vrev64_f32(vget_low_f32(V2))); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<1, 0, 5, 4>(FVector V1, FVector V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1)), vrev64_f32(vget_low_f32(V2))); }

		template<> inline Vector      XM_CALLCONV     VectorPermute<2, 3, 6, 7>(FVector V1, FVector V2) { return vcombine_f32(vget_high_f32(V1), vget_high_f32(V2)); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<3, 2, 6, 7>(FVector V1, FVector V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1)), vget_high_f32(V2)); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<2, 3, 7, 6>(FVector V1, FVector V2) { return vcombine_f32(vget_high_f32(V1), vrev64_f32(vget_high_f32(V2))); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<3, 2, 7, 6>(FVector V1, FVector V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1)), vrev64_f32(vget_high_f32(V2))); }

		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 1, 6, 7>(FVector V1, FVector V2) { return vcombine_f32(vget_low_f32(V1), vget_high_f32(V2)); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<1, 0, 6, 7>(FVector V1, FVector V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1)), vget_high_f32(V2)); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 1, 7, 6>(FVector V1, FVector V2) { return vcombine_f32(vget_low_f32(V1), vrev64_f32(vget_high_f32(V2))); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<1, 0, 7, 6>(FVector V1, FVector V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1)), vrev64_f32(vget_high_f32(V2))); }

		template<> inline Vector      XM_CALLCONV     VectorPermute<3, 2, 4, 5>(FVector V1, FVector V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1)), vget_low_f32(V2)); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<2, 3, 5, 4>(FVector V1, FVector V2) { return vcombine_f32(vget_high_f32(V1), vrev64_f32(vget_low_f32(V2))); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<3, 2, 5, 4>(FVector V1, FVector V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1)), vrev64_f32(vget_low_f32(V2))); }

		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 4, 2, 6>(FVector V1, FVector V2) { return vtrnq_f32(V1, V2).val[0]; }
		template<> inline Vector      XM_CALLCONV     VectorPermute<1, 5, 3, 7>(FVector V1, FVector V2) { return vtrnq_f32(V1, V2).val[1]; }

		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 4, 1, 5>(FVector V1, FVector V2) { return vzipq_f32(V1, V2).val[0]; }
		template<> inline Vector      XM_CALLCONV     VectorPermute<2, 6, 3, 7>(FVector V1, FVector V2) { return vzipq_f32(V1, V2).val[1]; }

		template<> inline Vector      XM_CALLCONV     VectorPermute<0, 2, 4, 6>(FVector V1, FVector V2) { return vuzpq_f32(V1, V2).val[0]; }
		template<> inline Vector      XM_CALLCONV     VectorPermute<1, 3, 5, 7>(FVector V1, FVector V2) { return vuzpq_f32(V1, V2).val[1]; }

		template<> inline Vector      XM_CALLCONV     VectorPermute<1, 2, 3, 4>(FVector V1, FVector V2) { return vextq_f32(V1, V2, 1); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<2, 3, 4, 5>(FVector V1, FVector V2) { return vextq_f32(V1, V2, 2); }
		template<> inline Vector      XM_CALLCONV     VectorPermute<3, 4, 5, 6>(FVector V1, FVector V2) { return vextq_f32(V1, V2, 3); }

#endif // _XM_ARM_NEON_INTRINSICS_ && !_XM_NO_INTRINSICS_

		//------------------------------------------------------------------------------

		// General swizzle template
		template<uint32_t SwizzleX, uint32_t SwizzleY, uint32_t SwizzleZ, uint32_t SwizzleW>
		inline Vector     XM_CALLCONV     VectorSwizzle(FVector V)
		{
			static_assert(SwizzleX <= 3, "SwizzleX template parameter out of range");
			static_assert(SwizzleY <= 3, "SwizzleY template parameter out of range");
			static_assert(SwizzleZ <= 3, "SwizzleZ template parameter out of range");
			static_assert(SwizzleW <= 3, "SwizzleW template parameter out of range");

#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			return XM_PERMUTE_PS(V, _MM_SHUFFLE(SwizzleW, SwizzleZ, SwizzleY, SwizzleX));
#else

			return VectorSwizzle(V, SwizzleX, SwizzleY, SwizzleZ, SwizzleW);

#endif
		}

		// Specialized swizzles
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<0, 1, 2, 3>(FVector V) { return V; }

#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<0, 1, 0, 1>(FVector V) { return _mm_movelh_ps(V, V); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<2, 3, 2, 3>(FVector V) { return _mm_movehl_ps(V, V); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<0, 0, 1, 1>(FVector V) { return _mm_unpacklo_ps(V, V); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<2, 2, 3, 3>(FVector V) { return _mm_unpackhi_ps(V, V); }
#endif

#if defined(_XM_SSE3_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<0, 0, 2, 2>(FVector V) { return _mm_moveldup_ps(V); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<1, 1, 3, 3>(FVector V) { return _mm_movehdup_ps(V); }
#endif

#if defined(_XM_AVX2_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<0, 0, 0, 0>(FVector V) { return _mm_broadcastss_ps(V); }
#endif

#if defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)

		template<> inline Vector      XM_CALLCONV     VectorSwizzle<0, 0, 0, 0>(FVector V) { return vdupq_lane_f32(vget_low_f32(V), 0); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<1, 1, 1, 1>(FVector V) { return vdupq_lane_f32(vget_low_f32(V), 1); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<2, 2, 2, 2>(FVector V) { return vdupq_lane_f32(vget_high_f32(V), 0); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<3, 3, 3, 3>(FVector V) { return vdupq_lane_f32(vget_high_f32(V), 1); }

		template<> inline Vector      XM_CALLCONV     VectorSwizzle<1, 0, 3, 2>(FVector V) { return vrev64q_f32(V); }

		template<> inline Vector      XM_CALLCONV     VectorSwizzle<0, 1, 0, 1>(FVector V) { float32x2_t vt = vget_low_f32(V); return vcombine_f32(vt, vt); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<2, 3, 2, 3>(FVector V) { float32x2_t vt = vget_high_f32(V); return vcombine_f32(vt, vt); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<1, 0, 1, 0>(FVector V) { float32x2_t vt = vrev64_f32(vget_low_f32(V)); return vcombine_f32(vt, vt); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<3, 2, 3, 2>(FVector V) { float32x2_t vt = vrev64_f32(vget_high_f32(V)); return vcombine_f32(vt, vt); }

		template<> inline Vector      XM_CALLCONV     VectorSwizzle<0, 1, 3, 2>(FVector V) { return vcombine_f32(vget_low_f32(V), vrev64_f32(vget_high_f32(V))); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<1, 0, 2, 3>(FVector V) { return vcombine_f32(vrev64_f32(vget_low_f32(V)), vget_high_f32(V)); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<2, 3, 1, 0>(FVector V) { return vcombine_f32(vget_high_f32(V), vrev64_f32(vget_low_f32(V))); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<3, 2, 0, 1>(FVector V) { return vcombine_f32(vrev64_f32(vget_high_f32(V)), vget_low_f32(V)); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<3, 2, 1, 0>(FVector V) { return vcombine_f32(vrev64_f32(vget_high_f32(V)), vrev64_f32(vget_low_f32(V))); }

		template<> inline Vector      XM_CALLCONV     VectorSwizzle<0, 0, 2, 2>(FVector V) { return vtrnq_f32(V, V).val[0]; }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<1, 1, 3, 3>(FVector V) { return vtrnq_f32(V, V).val[1]; }

		template<> inline Vector      XM_CALLCONV     VectorSwizzle<0, 0, 1, 1>(FVector V) { return vzipq_f32(V, V).val[0]; }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<2, 2, 3, 3>(FVector V) { return vzipq_f32(V, V).val[1]; }

		template<> inline Vector      XM_CALLCONV     VectorSwizzle<0, 2, 0, 2>(FVector V) { return vuzpq_f32(V, V).val[0]; }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<1, 3, 1, 3>(FVector V) { return vuzpq_f32(V, V).val[1]; }

		template<> inline Vector      XM_CALLCONV     VectorSwizzle<1, 2, 3, 0>(FVector V) { return vextq_f32(V, V, 1); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<2, 3, 0, 1>(FVector V) { return vextq_f32(V, V, 2); }
		template<> inline Vector      XM_CALLCONV     VectorSwizzle<3, 0, 1, 2>(FVector V) { return vextq_f32(V, V, 3); }

#endif // _XM_ARM_NEON_INTRINSICS_ && !_XM_NO_INTRINSICS_

		//------------------------------------------------------------------------------

		template<uint32_t Elements>
		inline Vector     XM_CALLCONV     VectorShiftLeft(FVector V1, FVector V2)
		{
			static_assert(Elements < 4, "Elements template parameter out of range");
			return VectorPermute<Elements, (Elements + 1), (Elements + 2), (Elements + 3)>(V1, V2);
		}

		template<uint32_t Elements>
		inline Vector     XM_CALLCONV     VectorRotateLeft(FVector V)
		{
			static_assert(Elements < 4, "Elements template parameter out of range");
			return VectorSwizzle<Elements & 3, (Elements + 1) & 3, (Elements + 2) & 3, (Elements + 3) & 3>(V);
		}

		template<uint32_t Elements>
		inline Vector     XM_CALLCONV     VectorRotateRight(FVector V)
		{
			static_assert(Elements < 4, "Elements template parameter out of range");
			return VectorSwizzle<(4 - Elements) & 3, (5 - Elements) & 3, (6 - Elements) & 3, (7 - Elements) & 3>(V);
		}

		template<uint32_t VSLeftRotateElements, uint32_t Select0, uint32_t Select1, uint32_t Select2, uint32_t Select3>
		inline Vector     XM_CALLCONV     VectorInsert(FVector VD, FVector VS)
		{
			Vector Control = VectorSelectControl(Select0 & 1, Select1 & 1, Select2 & 1, Select3 & 1);
			return VectorSelect(VD, VectorRotateLeft<VSLeftRotateElements>(VS), Control);
		}

		/****************************************************************************
		 *
		 * Globals
		 *
		 ****************************************************************************/

		 // The purpose of the following global constants is to prevent redundant
		 // reloading of the constants when they are referenced by more than one
		 // separate inline math routine called within the same function.  Declaring
		 // a constant locally within a routine is sufficient to prevent redundant
		 // reloads of that constant when that single routine is called multiple
		 // times in a function, but if the constant is used (and declared) in a
		 // separate math routine it would be reloaded.

#ifndef XMGLOBALCONST
		// RAY_TODO: (LINUX) Compatibility
#define XMGLOBALCONST extern const __declspec(selectany)
#endif

		XMGLOBALCONST VectorF32 g_XMSinCoefficients0 = { { { -0.16666667f, +0.0083333310f, -0.00019840874f, +2.7525562e-06f } } };
		XMGLOBALCONST VectorF32 g_XMSinCoefficients1 = { { { -2.3889859e-08f, -0.16665852f /*Est1*/, +0.0083139502f /*Est2*/, -0.00018524670f /*Est3*/ } } };
		XMGLOBALCONST VectorF32 g_XMCosCoefficients0 = { { { -0.5f, +0.041666638f, -0.0013888378f, +2.4760495e-05f } } };
		XMGLOBALCONST VectorF32 g_XMCosCoefficients1 = { { { -2.6051615e-07f, -0.49992746f /*Est1*/, +0.041493919f /*Est2*/, -0.0012712436f /*Est3*/ } } };
		XMGLOBALCONST VectorF32 g_XMTanCoefficients0 = { { { 1.0f, 0.333333333f, 0.133333333f, 5.396825397e-2f } } };
		XMGLOBALCONST VectorF32 g_XMTanCoefficients1 = { { { 2.186948854e-2f, 8.863235530e-3f, 3.592128167e-3f, 1.455834485e-3f } } };
		XMGLOBALCONST VectorF32 g_XMTanCoefficients2 = { { { 5.900274264e-4f, 2.391290764e-4f, 9.691537707e-5f, 3.927832950e-5f } } };
		XMGLOBALCONST VectorF32 g_XMArcCoefficients0 = { { { +1.5707963050f, -0.2145988016f, +0.0889789874f, -0.0501743046f } } };
		XMGLOBALCONST VectorF32 g_XMArcCoefficients1 = { { { +0.0308918810f, -0.0170881256f, +0.0066700901f, -0.0012624911f } } };
		XMGLOBALCONST VectorF32 g_XMATanCoefficients0 = { { { -0.3333314528f, +0.1999355085f, -0.1420889944f, +0.1065626393f } } };
		XMGLOBALCONST VectorF32 g_XMATanCoefficients1 = { { { -0.0752896400f, +0.0429096138f, -0.0161657367f, +0.0028662257f } } };
		XMGLOBALCONST VectorF32 g_XMATanEstCoefficients0 = { { { +0.999866f, +0.999866f, +0.999866f, +0.999866f } } };
		XMGLOBALCONST VectorF32 g_XMATanEstCoefficients1 = { { { -0.3302995f, +0.180141f, -0.085133f, +0.0208351f } } };
		XMGLOBALCONST VectorF32 g_XMTanEstCoefficients = { { { 2.484f, -1.954923183e-1f, 2.467401101f, XM_1DIVPI } } };
		XMGLOBALCONST VectorF32 g_XMArcEstCoefficients = { { { +1.5707288f, -0.2121144f, +0.0742610f, -0.0187293f } } };
		XMGLOBALCONST VectorF32 g_XMPiConstants0 = { { { XM_PI, XM_2PI, XM_1DIVPI, XM_1DIV2PI } } };
		XMGLOBALCONST VectorF32 g_XMIdentityR0 = { { { 1.0f, 0.0f, 0.0f, 0.0f } } };
		XMGLOBALCONST VectorF32 g_XMIdentityR1 = { { { 0.0f, 1.0f, 0.0f, 0.0f } } };
		XMGLOBALCONST VectorF32 g_XMIdentityR2 = { { { 0.0f, 0.0f, 1.0f, 0.0f } } };
		XMGLOBALCONST VectorF32 g_XMIdentityR3 = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
		XMGLOBALCONST VectorF32 g_XMNegIdentityR0 = { { { -1.0f, 0.0f, 0.0f, 0.0f } } };
		XMGLOBALCONST VectorF32 g_XMNegIdentityR1 = { { { 0.0f, -1.0f, 0.0f, 0.0f } } };
		XMGLOBALCONST VectorF32 g_XMNegIdentityR2 = { { { 0.0f, 0.0f, -1.0f, 0.0f } } };
		XMGLOBALCONST VectorF32 g_XMNegIdentityR3 = { { { 0.0f, 0.0f, 0.0f, -1.0f } } };
		XMGLOBALCONST VectorU32 g_XMNegativeZero = { { { 0x80000000, 0x80000000, 0x80000000, 0x80000000 } } };
		XMGLOBALCONST VectorU32 g_XMNegate3 = { { { 0x80000000, 0x80000000, 0x80000000, 0x00000000 } } };
		XMGLOBALCONST VectorU32 g_XMMaskXY = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000 } } };
		XMGLOBALCONST VectorU32 g_XMMask3 = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 } } };
		XMGLOBALCONST VectorU32 g_XMMaskX = { { { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 } } };
		XMGLOBALCONST VectorU32 g_XMMaskY = { { { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 } } };
		XMGLOBALCONST VectorU32 g_XMMaskZ = { { { 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000 } } };
		XMGLOBALCONST VectorU32 g_XMMaskW = { { { 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF } } };
		XMGLOBALCONST VectorF32 g_XMOne = { { { 1.0f, 1.0f, 1.0f, 1.0f } } };
		XMGLOBALCONST VectorF32 g_XMOne3 = { { { 1.0f, 1.0f, 1.0f, 0.0f } } };
		XMGLOBALCONST VectorF32 g_XMZero = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
		XMGLOBALCONST VectorF32 g_XMTwo = { { { 2.f, 2.f, 2.f, 2.f } } };
		XMGLOBALCONST VectorF32 g_XMFour = { { { 4.f, 4.f, 4.f, 4.f } } };
		XMGLOBALCONST VectorF32 g_XMSix = { { { 6.f, 6.f, 6.f, 6.f } } };
		XMGLOBALCONST VectorF32 g_XMNegativeOne = { { { -1.0f, -1.0f, -1.0f, -1.0f } } };
		XMGLOBALCONST VectorF32 g_XMOneHalf = { { { 0.5f, 0.5f, 0.5f, 0.5f } } };
		XMGLOBALCONST VectorF32 g_XMNegativeOneHalf = { { { -0.5f, -0.5f, -0.5f, -0.5f } } };
		XMGLOBALCONST VectorF32 g_XMNegativeTwoPi = { { { -XM_2PI, -XM_2PI, -XM_2PI, -XM_2PI } } };
		XMGLOBALCONST VectorF32 g_XMNegativePi = { { { -XM_PI, -XM_PI, -XM_PI, -XM_PI } } };
		XMGLOBALCONST VectorF32 g_XMHalfPi = { { { XM_PIDIV2, XM_PIDIV2, XM_PIDIV2, XM_PIDIV2 } } };
		XMGLOBALCONST VectorF32 g_XMPi = { { { XM_PI, XM_PI, XM_PI, XM_PI } } };
		XMGLOBALCONST VectorF32 g_XMReciprocalPi = { { { XM_1DIVPI, XM_1DIVPI, XM_1DIVPI, XM_1DIVPI } } };
		XMGLOBALCONST VectorF32 g_XMTwoPi = { { { XM_2PI, XM_2PI, XM_2PI, XM_2PI } } };
		XMGLOBALCONST VectorF32 g_XMReciprocalTwoPi = { { { XM_1DIV2PI, XM_1DIV2PI, XM_1DIV2PI, XM_1DIV2PI } } };
		XMGLOBALCONST VectorF32 g_XMEpsilon = { { { 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f } } };
		XMGLOBALCONST VectorI32 g_XMInfinity = { { { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 } } };
		XMGLOBALCONST VectorI32 g_XMQNaN = { { { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 } } };
		XMGLOBALCONST VectorI32 g_XMQNaNTest = { { { 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF } } };
		XMGLOBALCONST VectorI32 g_XMAbsMask = { { { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF } } };
		XMGLOBALCONST VectorI32 g_XMFltMin = { { { 0x00800000, 0x00800000, 0x00800000, 0x00800000 } } };
		XMGLOBALCONST VectorI32 g_XMFltMax = { { { 0x7F7FFFFF, 0x7F7FFFFF, 0x7F7FFFFF, 0x7F7FFFFF } } };
		XMGLOBALCONST VectorU32 g_XMNegOneMask = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF } } };
		XMGLOBALCONST VectorU32 g_XMMaskA8R8G8B8 = { { { 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000 } } };
		XMGLOBALCONST VectorU32 g_XMFlipA8R8G8B8 = { { { 0x00000000, 0x00000000, 0x00000000, 0x80000000 } } };
		XMGLOBALCONST VectorF32 g_XMFixAA8R8G8B8 = { { { 0.0f, 0.0f, 0.0f, float(0x80000000U) } } };
		XMGLOBALCONST VectorF32 g_XMNormalizeA8R8G8B8 = { { { 1.0f / (255.0f * float(0x10000)), 1.0f / (255.0f * float(0x100)), 1.0f / 255.0f, 1.0f / (255.0f * float(0x1000000)) } } };
		XMGLOBALCONST VectorU32 g_XMMaskA2B10G10R10 = { { { 0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000 } } };
		XMGLOBALCONST VectorU32 g_XMFlipA2B10G10R10 = { { { 0x00000200, 0x00080000, 0x20000000, 0x80000000 } } };
		XMGLOBALCONST VectorF32 g_XMFixAA2B10G10R10 = { { { -512.0f, -512.0f * float(0x400), -512.0f * float(0x100000), float(0x80000000U) } } };
		XMGLOBALCONST VectorF32 g_XMNormalizeA2B10G10R10 = { { { 1.0f / 511.0f, 1.0f / (511.0f * float(0x400)), 1.0f / (511.0f * float(0x100000)), 1.0f / (3.0f * float(0x40000000)) } } };
		XMGLOBALCONST VectorU32 g_XMMaskX16Y16 = { { { 0x0000FFFF, 0xFFFF0000, 0x00000000, 0x00000000 } } };
		XMGLOBALCONST VectorI32 g_XMFlipX16Y16 = { { { 0x00008000, 0x00000000, 0x00000000, 0x00000000 } } };
		XMGLOBALCONST VectorF32 g_XMFixX16Y16 = { { { -32768.0f, 0.0f, 0.0f, 0.0f } } };
		XMGLOBALCONST VectorF32 g_XMNormalizeX16Y16 = { { { 1.0f / 32767.0f, 1.0f / (32767.0f * 65536.0f), 0.0f, 0.0f } } };
		XMGLOBALCONST VectorU32 g_XMMaskX16Y16Z16W16 = { { { 0x0000FFFF, 0x0000FFFF, 0xFFFF0000, 0xFFFF0000 } } };
		XMGLOBALCONST VectorI32 g_XMFlipX16Y16Z16W16 = { { { 0x00008000, 0x00008000, 0x00000000, 0x00000000 } } };
		XMGLOBALCONST VectorF32 g_XMFixX16Y16Z16W16 = { { { -32768.0f, -32768.0f, 0.0f, 0.0f } } };
		XMGLOBALCONST VectorF32 g_XMNormalizeX16Y16Z16W16 = { { { 1.0f / 32767.0f, 1.0f / 32767.0f, 1.0f / (32767.0f * 65536.0f), 1.0f / (32767.0f * 65536.0f) } } };
		XMGLOBALCONST VectorF32 g_XMNoFraction = { { { 8388608.0f, 8388608.0f, 8388608.0f, 8388608.0f } } };
		XMGLOBALCONST VectorI32 g_XMMaskByte = { { { 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF } } };
		XMGLOBALCONST VectorF32 g_XMNegateX = { { { -1.0f, 1.0f, 1.0f, 1.0f } } };
		XMGLOBALCONST VectorF32 g_XMNegateY = { { { 1.0f, -1.0f, 1.0f, 1.0f } } };
		XMGLOBALCONST VectorF32 g_XMNegateZ = { { { 1.0f, 1.0f, -1.0f, 1.0f } } };
		XMGLOBALCONST VectorF32 g_XMNegateW = { { { 1.0f, 1.0f, 1.0f, -1.0f } } };
		XMGLOBALCONST VectorU32 g_XMSelect0101 = { { { XM_SELECT_0, XM_SELECT_1, XM_SELECT_0, XM_SELECT_1 } } };
		XMGLOBALCONST VectorU32 g_XMSelect1010 = { { { XM_SELECT_1, XM_SELECT_0, XM_SELECT_1, XM_SELECT_0 } } };
		XMGLOBALCONST VectorI32 g_XMOneHalfMinusEpsilon = { { { 0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD } } };
		XMGLOBALCONST VectorU32 g_XMSelect1000 = { { { XM_SELECT_1, XM_SELECT_0, XM_SELECT_0, XM_SELECT_0 } } };
		XMGLOBALCONST VectorU32 g_XMSelect1100 = { { { XM_SELECT_1, XM_SELECT_1, XM_SELECT_0, XM_SELECT_0 } } };
		XMGLOBALCONST VectorU32 g_XMSelect1110 = { { { XM_SELECT_1, XM_SELECT_1, XM_SELECT_1, XM_SELECT_0 } } };
		XMGLOBALCONST VectorU32 g_XMSelect1011 = { { { XM_SELECT_1, XM_SELECT_0, XM_SELECT_1, XM_SELECT_1 } } };
		XMGLOBALCONST VectorF32 g_XMFixupY16 = { { { 1.0f, 1.0f / 65536.0f, 0.0f, 0.0f } } };
		XMGLOBALCONST VectorF32 g_XMFixupY16W16 = { { { 1.0f, 1.0f, 1.0f / 65536.0f, 1.0f / 65536.0f } } };
		XMGLOBALCONST VectorU32 g_XMFlipY = { { { 0, 0x80000000, 0, 0 } } };
		XMGLOBALCONST VectorU32 g_XMFlipZ = { { { 0, 0, 0x80000000, 0 } } };
		XMGLOBALCONST VectorU32 g_XMFlipW = { { { 0, 0, 0, 0x80000000 } } };
		XMGLOBALCONST VectorU32 g_XMFlipYZ = { { { 0, 0x80000000, 0x80000000, 0 } } };
		XMGLOBALCONST VectorU32 g_XMFlipZW = { { { 0, 0, 0x80000000, 0x80000000 } } };
		XMGLOBALCONST VectorU32 g_XMFlipYW = { { { 0, 0x80000000, 0, 0x80000000 } } };
		XMGLOBALCONST VectorI32 g_XMMaskDec4 = { { { 0x3FF, 0x3FF << 10, 0x3FF << 20, static_cast<int>(0xC0000000) } } };
		XMGLOBALCONST VectorI32 g_XMXorDec4 = { { { 0x200, 0x200 << 10, 0x200 << 20, 0 } } };
		XMGLOBALCONST VectorF32 g_XMAddUDec4 = { { { 0, 0, 0, 32768.0f * 65536.0f } } };
		XMGLOBALCONST VectorF32 g_XMAddDec4 = { { { -512.0f, -512.0f * 1024.0f, -512.0f * 1024.0f * 1024.0f, 0 } } };
		XMGLOBALCONST VectorF32 g_XMMulDec4 = { { { 1.0f, 1.0f / 1024.0f, 1.0f / (1024.0f * 1024.0f), 1.0f / (1024.0f * 1024.0f * 1024.0f) } } };
		XMGLOBALCONST VectorU32 g_XMMaskByte4 = { { { 0xFF, 0xFF00, 0xFF0000, 0xFF000000 } } };
		XMGLOBALCONST VectorI32 g_XMXorByte4 = { { { 0x80, 0x8000, 0x800000, 0x00000000 } } };
		XMGLOBALCONST VectorF32 g_XMAddByte4 = { { { -128.0f, -128.0f * 256.0f, -128.0f * 65536.0f, 0 } } };
		XMGLOBALCONST VectorF32 g_XMFixUnsigned = { { { 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f } } };
		XMGLOBALCONST VectorF32 g_XMMaxInt = { { { 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f } } };
		XMGLOBALCONST VectorF32 g_XMMaxUInt = { { { 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f } } };
		XMGLOBALCONST VectorF32 g_XMUnsignedFix = { { { 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f } } };
		XMGLOBALCONST VectorF32 g_XMsrgbScale = { { { 12.92f, 12.92f, 12.92f, 1.0f } } };
		XMGLOBALCONST VectorF32 g_XMsrgbA = { { { 0.055f, 0.055f, 0.055f, 0.0f } } };
		XMGLOBALCONST VectorF32 g_XMsrgbA1 = { { { 1.055f, 1.055f, 1.055f, 1.0f } } };
		XMGLOBALCONST VectorI32 g_XMExponentBias = { { { 127, 127, 127, 127 } } };
		XMGLOBALCONST VectorI32 g_XMSubnormalExponent = { { { -126, -126, -126, -126 } } };
		XMGLOBALCONST VectorI32 g_XMNumTrailing = { { { 23, 23, 23, 23 } } };
		XMGLOBALCONST VectorI32 g_XMMinNormal = { { { 0x00800000, 0x00800000, 0x00800000, 0x00800000 } } };
		XMGLOBALCONST VectorU32 g_XMNegInfinity = { { { 0xFF800000, 0xFF800000, 0xFF800000, 0xFF800000 } } };
		XMGLOBALCONST VectorU32 g_XMNegQNaN = { { { 0xFFC00000, 0xFFC00000, 0xFFC00000, 0xFFC00000 } } };
		XMGLOBALCONST VectorI32 g_XMBin128 = { { { 0x43000000, 0x43000000, 0x43000000, 0x43000000 } } };
		XMGLOBALCONST VectorU32 g_XMBinNeg150 = { { { 0xC3160000, 0xC3160000, 0xC3160000, 0xC3160000 } } };
		XMGLOBALCONST VectorI32 g_XM253 = { { { 253, 253, 253, 253 } } };
		XMGLOBALCONST VectorF32 g_XMExpEst1 = { { { -6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f } } };
		XMGLOBALCONST VectorF32 g_XMExpEst2 = { { { +2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f } } };
		XMGLOBALCONST VectorF32 g_XMExpEst3 = { { { -5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f } } };
		XMGLOBALCONST VectorF32 g_XMExpEst4 = { { { +9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f } } };
		XMGLOBALCONST VectorF32 g_XMExpEst5 = { { { -1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f } } };
		XMGLOBALCONST VectorF32 g_XMExpEst6 = { { { +1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f } } };
		XMGLOBALCONST VectorF32 g_XMExpEst7 = { { { -1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f } } };
		XMGLOBALCONST VectorF32 g_XMLogEst0 = { { { +1.442693f, +1.442693f, +1.442693f, +1.442693f } } };
		XMGLOBALCONST VectorF32 g_XMLogEst1 = { { { -0.721242f, -0.721242f, -0.721242f, -0.721242f } } };
		XMGLOBALCONST VectorF32 g_XMLogEst2 = { { { +0.479384f, +0.479384f, +0.479384f, +0.479384f } } };
		XMGLOBALCONST VectorF32 g_XMLogEst3 = { { { -0.350295f, -0.350295f, -0.350295f, -0.350295f } } };
		XMGLOBALCONST VectorF32 g_XMLogEst4 = { { { +0.248590f, +0.248590f, +0.248590f, +0.248590f } } };
		XMGLOBALCONST VectorF32 g_XMLogEst5 = { { { -0.145700f, -0.145700f, -0.145700f, -0.145700f } } };
		XMGLOBALCONST VectorF32 g_XMLogEst6 = { { { +0.057148f, +0.057148f, +0.057148f, +0.057148f } } };
		XMGLOBALCONST VectorF32 g_XMLogEst7 = { { { -0.010578f, -0.010578f, -0.010578f, -0.010578f } } };
		XMGLOBALCONST VectorF32 g_XMLgE = { { { +1.442695f, +1.442695f, +1.442695f, +1.442695f } } };
		XMGLOBALCONST VectorF32 g_XMInvLgE = { { { +6.93147182e-1f, +6.93147182e-1f, +6.93147182e-1f, +6.93147182e-1f } } };
		XMGLOBALCONST VectorF32 g_UByteMax = { { { 255.0f, 255.0f, 255.0f, 255.0f } } };
		XMGLOBALCONST VectorF32 g_ByteMin = { { { -127.0f, -127.0f, -127.0f, -127.0f } } };
		XMGLOBALCONST VectorF32 g_ByteMax = { { { 127.0f, 127.0f, 127.0f, 127.0f } } };
		XMGLOBALCONST VectorF32 g_ShortMin = { { { -32767.0f, -32767.0f, -32767.0f, -32767.0f } } };
		XMGLOBALCONST VectorF32 g_ShortMax = { { { 32767.0f, 32767.0f, 32767.0f, 32767.0f } } };
		XMGLOBALCONST VectorF32 g_UShortMax = { { { 65535.0f, 65535.0f, 65535.0f, 65535.0f } } };

		/****************************************************************************
		 *
		 * Implementation
		 *
		 ****************************************************************************/

#pragma warning(push)
#pragma warning(disable:4068 4214 4204 4365 4616 4640 6001 6101)
		 // C4068/4616: ignore unknown pragmas
		 // C4214/4204: nonstandard extension used
		 // C4365/4640: Off by default noise
		 // C6001/6101: False positives

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 25000, "FVector is 16 bytes")
#pragma prefast(disable : 26495, "Union initialization confuses /analyze")
#endif

//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorSetBinaryConstant(uint32_t C0, uint32_t C1, uint32_t C2, uint32_t C3)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 vResult;
			vResult.u[0] = (0 - (C0 & 1)) & 0x3F800000;
			vResult.u[1] = (0 - (C1 & 1)) & 0x3F800000;
			vResult.u[2] = (0 - (C2 & 1)) & 0x3F800000;
			vResult.u[3] = (0 - (C3 & 1)) & 0x3F800000;
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			VectorU32 vResult;
			vResult.u[0] = (0 - (C0 & 1)) & 0x3F800000;
			vResult.u[1] = (0 - (C1 & 1)) & 0x3F800000;
			vResult.u[2] = (0 - (C2 & 1)) & 0x3F800000;
			vResult.u[3] = (0 - (C3 & 1)) & 0x3F800000;
			return vResult.v;
#else // XM_SSE_INTRINSICS_
			static const VectorU32 g_vMask1 = { { { 1, 1, 1, 1 } } };
			// Move the parms to a vector
			__m128i vTemp = _mm_set_epi32(static_cast<int>(C3), static_cast<int>(C2), static_cast<int>(C1), static_cast<int>(C0));
			// Mask off the low bits
			vTemp = _mm_and_si128(vTemp, g_vMask1);
			// 0xFFFFFFFF on true bits
			vTemp = _mm_cmpeq_epi32(vTemp, g_vMask1);
			// 0xFFFFFFFF -> 1.0f, 0x00000000 -> 0.0f
			vTemp = _mm_and_si128(vTemp, g_XMOne);
			return _mm_castsi128_ps(vTemp);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorSplatConstant(int32_t IntConstant, uint32_t DivExponent)
		{
			assert(IntConstant >= -16 && IntConstant <= 15);
			assert(DivExponent < 32);
#if defined(_XM_NO_INTRINSICS_)

			using DirectX::ConvertVectorIntToFloat;

			VectorI32 V = { { { IntConstant, IntConstant, IntConstant, IntConstant } } };
			return ConvertVectorIntToFloat(V.v, DivExponent);

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Splat the int
			int32x4_t vScale = vdupq_n_s32(IntConstant);
			// Convert to a float
			Vector vResult = vcvtq_f32_s32(vScale);
			// Convert DivExponent into 1.0f/(1<<DivExponent)
			uint32_t uScale = 0x3F800000U - (DivExponent << 23);
			// Splat the scalar value (It's really a float)
			vScale = vdupq_n_u32(uScale);
			// Multiply by the reciprocal (Perform a right shift by DivExponent)
			vResult = vmulq_f32(vResult, reinterpret_cast<const float32x4_t*>(&vScale)[0]);
			return vResult;
#else // XM_SSE_INTRINSICS_
			// Splat the int
			__m128i vScale = _mm_set1_epi32(IntConstant);
			// Convert to a float
			Vector vResult = _mm_cvtepi32_ps(vScale);
			// Convert DivExponent into 1.0f/(1<<DivExponent)
			uint32_t uScale = 0x3F800000U - (DivExponent << 23);
			// Splat the scalar value (It's really a float)
			vScale = _mm_set1_epi32(static_cast<int>(uScale));
			// Multiply by the reciprocal (Perform a right shift by DivExponent)
			vResult = _mm_mul_ps(vResult, _mm_castsi128_ps(vScale));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorSplatConstantInt(int32_t IntConstant)
		{
			assert(IntConstant >= -16 && IntConstant <= 15);
#if defined(_XM_NO_INTRINSICS_)

			VectorI32 V = { { { IntConstant, IntConstant, IntConstant, IntConstant } } };
			return V.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			int32x4_t V = vdupq_n_s32(IntConstant);
			return reinterpret_cast<float32x4_t*>(&V)[0];
#else // XM_SSE_INTRINSICS_
			__m128i V = _mm_set1_epi32(IntConstant);
			return _mm_castsi128_ps(V);
#endif
		}


		//-------------------------------------------------------------------------------------
	// DirectXMathConvert.inl -- SIMD C++ Math library
	//
	// Copyright (c) Microsoft Corporation. All rights reserved.
	// Licensed under the MIT License.
	//
	// http://go.microsoft.com/fwlink/?LinkID=615560
	//-------------------------------------------------------------------------------------

	/****************************************************************************
	 *
	 * Data conversion
	 *
	 ****************************************************************************/

	 //------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable:4701)
// C4701: false positives

		inline Vector XM_CALLCONV ConvertVectorIntToFloat
		(
			FVector    VInt,
			uint32_t     DivExponent
		)
		{
			assert(DivExponent < 32);
#if defined(_XM_NO_INTRINSICS_)
			float fScale = 1.0f / static_cast<float>(1U << DivExponent);
			uint32_t ElementIndex = 0;
			Vector Result;
			do {
				auto iTemp = static_cast<int32_t>(VInt.vector4_u32[ElementIndex]);
				Result.vector4_f32[ElementIndex] = static_cast<float>(iTemp) * fScale;
			} while (++ElementIndex < 4);
			return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float fScale = 1.0f / (float)(1U << DivExponent);
			float32x4_t vResult = vcvtq_f32_s32(VInt);
			return vmulq_n_f32(vResult, fScale);
#else // _XM_SSE_INTRINSICS_
			// Convert to floats
			Vector vResult = _mm_cvtepi32_ps(_mm_castps_si128(VInt));
			// Convert DivExponent into 1.0f/(1<<DivExponent)
			uint32_t uScale = 0x3F800000U - (DivExponent << 23);
			// Splat the scalar value
			__m128i vScale = _mm_set1_epi32(static_cast<int>(uScale));
			vResult = _mm_mul_ps(vResult, _mm_castsi128_ps(vScale));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ConvertVectorFloatToInt
		(
			FVector    VFloat,
			uint32_t     MulExponent
		)
		{
			assert(MulExponent < 32);
#if defined(_XM_NO_INTRINSICS_)
			// Get the scalar factor.
			auto fScale = static_cast<float>(1U << MulExponent);
			uint32_t ElementIndex = 0;
			Vector Result;
			do {
				int32_t iResult;
				float fTemp = VFloat.vector4_f32[ElementIndex] * fScale;
				if (fTemp <= -(65536.0f * 32768.0f)) {
					iResult = (-0x7FFFFFFF) - 1;
				}
				else if (fTemp > (65536.0f * 32768.0f) - 128.0f) {
					iResult = 0x7FFFFFFF;
				}
				else {
					iResult = static_cast<int32_t>(fTemp);
				}
				Result.vector4_u32[ElementIndex] = static_cast<uint32_t>(iResult);
			} while (++ElementIndex < 4);
			return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4_t vResult = vmulq_n_f32(VFloat, (float)(1U << MulExponent));
			// In case of positive overflow, detect it
			uint32x4_t vOverflow = vcgtq_f32(vResult, g_XMMaxInt);
			// Float to int conversion
			int32x4_t vResulti = vcvtq_s32_f32(vResult);
			// If there was positive overflow, set to 0x7FFFFFFF
			vResult = vandq_u32(vOverflow, g_XMAbsMask);
			vOverflow = vbicq_u32(vResulti, vOverflow);
			vOverflow = vorrq_u32(vOverflow, vResult);
			return vOverflow;
#else // _XM_SSE_INTRINSICS_
			Vector vResult = _mm_set_ps1(static_cast<float>(1U << MulExponent));
			vResult = _mm_mul_ps(vResult, VFloat);
			// In case of positive overflow, detect it
			Vector vOverflow = _mm_cmpgt_ps(vResult, g_XMMaxInt);
			// Float to int conversion
			__m128i vResulti = _mm_cvttps_epi32(vResult);
			// If there was positive overflow, set to 0x7FFFFFFF
			vResult = _mm_and_ps(vOverflow, g_XMAbsMask);
			vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
			vOverflow = _mm_or_ps(vOverflow, vResult);
			return vOverflow;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ConvertVectorUIntToFloat
		(
			FVector     VUInt,
			uint32_t      DivExponent
		)
		{
			assert(DivExponent < 32);
#if defined(_XM_NO_INTRINSICS_)
			float fScale = 1.0f / static_cast<float>(1U << DivExponent);
			uint32_t ElementIndex = 0;
			Vector Result;
			do {
				Result.vector4_f32[ElementIndex] = static_cast<float>(VUInt.vector4_u32[ElementIndex]) * fScale;
			} while (++ElementIndex < 4);
			return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float fScale = 1.0f / (float)(1U << DivExponent);
			float32x4_t vResult = vcvtq_f32_u32(VUInt);
			return vmulq_n_f32(vResult, fScale);
#else // _XM_SSE_INTRINSICS_
			// For the values that are higher than 0x7FFFFFFF, a fixup is needed
			// Determine which ones need the fix.
			Vector vMask = _mm_and_ps(VUInt, g_XMNegativeZero);
			// Force all values positive
			Vector vResult = _mm_xor_ps(VUInt, vMask);
			// Convert to floats
			vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
			// Convert 0x80000000 -> 0xFFFFFFFF
			__m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
			// For only the ones that are too big, add the fixup
			vMask = _mm_and_ps(_mm_castsi128_ps(iMask), g_XMFixUnsigned);
			vResult = _mm_add_ps(vResult, vMask);
			// Convert DivExponent into 1.0f/(1<<DivExponent)
			uint32_t uScale = 0x3F800000U - (DivExponent << 23);
			// Splat
			iMask = _mm_set1_epi32(static_cast<int>(uScale));
			vResult = _mm_mul_ps(vResult, _mm_castsi128_ps(iMask));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ConvertVectorFloatToUInt
		(
			FVector     VFloat,
			uint32_t      MulExponent
		)
		{
			assert(MulExponent < 32);
#if defined(_XM_NO_INTRINSICS_)
			// Get the scalar factor.
			auto fScale = static_cast<float>(1U << MulExponent);
			uint32_t ElementIndex = 0;
			Vector Result;
			do {
				uint32_t uResult;
				float fTemp = VFloat.vector4_f32[ElementIndex] * fScale;
				if (fTemp <= 0.0f) {
					uResult = 0;
				}
				else if (fTemp >= (65536.0f * 65536.0f)) {
					uResult = 0xFFFFFFFFU;
				}
				else {
					uResult = static_cast<uint32_t>(fTemp);
				}
				Result.vector4_u32[ElementIndex] = uResult;
			} while (++ElementIndex < 4);
			return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4_t vResult = vmulq_n_f32(VFloat, (float)(1U << MulExponent));
			// In case of overflow, detect it
			uint32x4_t vOverflow = vcgtq_f32(vResult, g_XMMaxUInt);
			// Float to int conversion
			uint32x4_t vResulti = vcvtq_u32_f32(vResult);
			// If there was overflow, set to 0xFFFFFFFFU
			vResult = vbicq_u32(vResulti, vOverflow);
			vOverflow = vorrq_u32(vOverflow, vResult);
			return vOverflow;
#else // _XM_SSE_INTRINSICS_
			Vector vResult = _mm_set_ps1(static_cast<float>(1U << MulExponent));
			vResult = _mm_mul_ps(vResult, VFloat);
			// Clamp to >=0
			vResult = _mm_max_ps(vResult, g_XMZero);
			// Any numbers that are too big, set to 0xFFFFFFFFU
			Vector vOverflow = _mm_cmpgt_ps(vResult, g_XMMaxUInt);
			Vector vValue = g_XMUnsignedFix;
			// Too large for a signed integer?
			Vector vMask = _mm_cmpge_ps(vResult, vValue);
			// Zero for number's lower than 0x80000000, 32768.0f*65536.0f otherwise
			vValue = _mm_and_ps(vValue, vMask);
			// Perform fixup only on numbers too large (Keeps low bit precision)
			vResult = _mm_sub_ps(vResult, vValue);
			__m128i vResulti = _mm_cvttps_epi32(vResult);
			// Convert from signed to unsigned pnly if greater than 0x80000000
			vMask = _mm_and_ps(vMask, g_XMNegativeZero);
			vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
			// On those that are too large, set to 0xFFFFFFFF
			vResult = _mm_or_ps(vResult, vOverflow);
			return vResult;
#endif
		}

#pragma warning(pop)

		/****************************************************************************
		 *
		 * Vector and matrix load operations
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------
		inline Vector XM_CALLCONV LoadInt(const uint32_t* pSource)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_u32[0] = *pSource;
			V.vector4_u32[1] = 0;
			V.vector4_u32[2] = 0;
			V.vector4_u32[3] = 0;
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t zero = vdupq_n_u32(0);
			return vld1q_lane_u32(pSource, zero, 0);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_load_ss(reinterpret_cast<const float*>(pSource));
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadFloat(const float* pSource)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_f32[0] = *pSource;
			V.vector4_f32[1] = 0.f;
			V.vector4_f32[2] = 0.f;
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4_t zero = vdupq_n_f32(0);
			return vld1q_lane_f32(pSource, zero, 0);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_load_ss(pSource);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadInt2
		(
			const uint32_t* pSource
		)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_u32[0] = pSource[0];
			V.vector4_u32[1] = pSource[1];
			V.vector4_u32[2] = 0;
			V.vector4_u32[3] = 0;
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t x = vld1_u32(pSource);
			uint32x2_t zero = vdup_n_u32(0);
			return vcombine_u32(x, zero);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 x = _mm_load_ss(reinterpret_cast<const float*>(pSource));
			__m128 y = _mm_load_ss(reinterpret_cast<const float*>(pSource + 1));
			return _mm_unpacklo_ps(x, y);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadInt2A
		(
			const uint32_t* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_u32[0] = pSource[0];
			V.vector4_u32[1] = pSource[1];
			V.vector4_u32[2] = 0;
			V.vector4_u32[3] = 0;
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t x = vld1_u32_ex(pSource, 64);
			uint32x2_t zero = vdup_n_u32(0);
			return vcombine_u32(x, zero);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i V = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(pSource));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadFloat2
		(
			const Float2* pSource
		)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_f32[0] = pSource->x;
			V.vector4_f32[1] = pSource->y;
			V.vector4_f32[2] = 0.f;
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t x = vld1_f32(reinterpret_cast<const float*>(pSource));
			float32x2_t zero = vdup_n_f32(0);
			return vcombine_f32(x, zero);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 x = _mm_load_ss(&pSource->x);
			__m128 y = _mm_load_ss(&pSource->y);
			return _mm_unpacklo_ps(x, y);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadFloat2A
		(
			const Float2A* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_f32[0] = pSource->x;
			V.vector4_f32[1] = pSource->y;
			V.vector4_f32[2] = 0.f;
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t x = vld1_f32_ex(reinterpret_cast<const float*>(pSource), 64);
			float32x2_t zero = vdup_n_f32(0);
			return vcombine_f32(x, zero);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i V = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(pSource));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadSInt2
		(
			const Int2* pSource
		)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_f32[0] = static_cast<float>(pSource->x);
			V.vector4_f32[1] = static_cast<float>(pSource->y);
			V.vector4_f32[2] = 0.f;
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			int32x2_t x = vld1_s32(reinterpret_cast<const int32_t*>(pSource));
			float32x2_t v = vcvt_f32_s32(x);
			float32x2_t zero = vdup_n_f32(0);
			return vcombine_f32(v, zero);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 x = _mm_load_ss(reinterpret_cast<const float*>(&pSource->x));
			__m128 y = _mm_load_ss(reinterpret_cast<const float*>(&pSource->y));
			__m128 V = _mm_unpacklo_ps(x, y);
			return _mm_cvtepi32_ps(_mm_castps_si128(V));
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadUInt2
		(
			const UInt2* pSource
		)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_f32[0] = static_cast<float>(pSource->x);
			V.vector4_f32[1] = static_cast<float>(pSource->y);
			V.vector4_f32[2] = 0.f;
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t x = vld1_u32(reinterpret_cast<const uint32_t*>(pSource));
			float32x2_t v = vcvt_f32_u32(x);
			float32x2_t zero = vdup_n_f32(0);
			return vcombine_f32(v, zero);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 x = _mm_load_ss(reinterpret_cast<const float*>(&pSource->x));
			__m128 y = _mm_load_ss(reinterpret_cast<const float*>(&pSource->y));
			__m128 V = _mm_unpacklo_ps(x, y);
			// For the values that are higher than 0x7FFFFFFF, a fixup is needed
			// Determine which ones need the fix.
			Vector vMask = _mm_and_ps(V, g_XMNegativeZero);
			// Force all values positive
			Vector vResult = _mm_xor_ps(V, vMask);
			// Convert to floats
			vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
			// Convert 0x80000000 -> 0xFFFFFFFF
			__m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
			// For only the ones that are too big, add the fixup
			vMask = _mm_and_ps(_mm_castsi128_ps(iMask), g_XMFixUnsigned);
			vResult = _mm_add_ps(vResult, vMask);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadInt3
		(
			const uint32_t* pSource
		)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_u32[0] = pSource[0];
			V.vector4_u32[1] = pSource[1];
			V.vector4_u32[2] = pSource[2];
			V.vector4_u32[3] = 0;
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t x = vld1_u32(pSource);
			uint32x2_t zero = vdup_n_u32(0);
			uint32x2_t y = vld1_lane_u32(pSource + 2, zero, 0);
			return vcombine_u32(x, y);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 x = _mm_load_ss(reinterpret_cast<const float*>(pSource));
			__m128 y = _mm_load_ss(reinterpret_cast<const float*>(pSource + 1));
			__m128 z = _mm_load_ss(reinterpret_cast<const float*>(pSource + 2));
			__m128 xy = _mm_unpacklo_ps(x, y);
			return _mm_movelh_ps(xy, z);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadInt3A
		(
			const uint32_t* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_u32[0] = pSource[0];
			V.vector4_u32[1] = pSource[1];
			V.vector4_u32[2] = pSource[2];
			V.vector4_u32[3] = 0;
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Reads an extra integer which is zero'd
			uint32x4_t V = vld1q_u32_ex(pSource, 128);
			return vsetq_lane_u32(0, V, 3);
#elif defined(_XM_SSE_INTRINSICS_)
			// Reads an extra integer which is zero'd
			__m128i V = _mm_load_si128(reinterpret_cast<const __m128i*>(pSource));
			V = _mm_and_si128(V, g_XMMask3);
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadFloat3
		(
			const Float3* pSource
		)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_f32[0] = pSource->x;
			V.vector4_f32[1] = pSource->y;
			V.vector4_f32[2] = pSource->z;
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t x = vld1_f32(reinterpret_cast<const float*>(pSource));
			float32x2_t zero = vdup_n_f32(0);
			float32x2_t y = vld1_lane_f32(reinterpret_cast<const float*>(pSource) + 2, zero, 0);
			return vcombine_f32(x, y);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 x = _mm_load_ss(&pSource->x);
			__m128 y = _mm_load_ss(&pSource->y);
			__m128 z = _mm_load_ss(&pSource->z);
			__m128 xy = _mm_unpacklo_ps(x, y);
			return _mm_movelh_ps(xy, z);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadFloat3A
		(
			const Float3A* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_f32[0] = pSource->x;
			V.vector4_f32[1] = pSource->y;
			V.vector4_f32[2] = pSource->z;
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Reads an extra float which is zero'd
			float32x4_t V = vld1q_f32_ex(reinterpret_cast<const float*>(pSource), 128);
			return vsetq_lane_f32(0, V, 3);
#elif defined(_XM_SSE_INTRINSICS_)
			// Reads an extra float which is zero'd
			__m128 V = _mm_load_ps(&pSource->x);
			return _mm_and_ps(V, g_XMMask3);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadSInt3
		(
			const Int3* pSource
		)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)

			Vector V;
			V.vector4_f32[0] = static_cast<float>(pSource->x);
			V.vector4_f32[1] = static_cast<float>(pSource->y);
			V.vector4_f32[2] = static_cast<float>(pSource->z);
			V.vector4_f32[3] = 0.f;
			return V;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			int32x2_t x = vld1_s32(reinterpret_cast<const int32_t*>(pSource));
			int32x2_t zero = vdup_n_s32(0);
			int32x2_t y = vld1_lane_s32(reinterpret_cast<const int32_t*>(pSource) + 2, zero, 0);
			int32x4_t v = vcombine_s32(x, y);
			return vcvtq_f32_s32(v);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 x = _mm_load_ss(reinterpret_cast<const float*>(&pSource->x));
			__m128 y = _mm_load_ss(reinterpret_cast<const float*>(&pSource->y));
			__m128 z = _mm_load_ss(reinterpret_cast<const float*>(&pSource->z));
			__m128 xy = _mm_unpacklo_ps(x, y);
			__m128 V = _mm_movelh_ps(xy, z);
			return _mm_cvtepi32_ps(_mm_castps_si128(V));
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadUInt3
		(
			const UInt3* pSource
		)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_f32[0] = static_cast<float>(pSource->x);
			V.vector4_f32[1] = static_cast<float>(pSource->y);
			V.vector4_f32[2] = static_cast<float>(pSource->z);
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t x = vld1_u32(reinterpret_cast<const uint32_t*>(pSource));
			uint32x2_t zero = vdup_n_u32(0);
			uint32x2_t y = vld1_lane_u32(reinterpret_cast<const uint32_t*>(pSource) + 2, zero, 0);
			uint32x4_t v = vcombine_u32(x, y);
			return vcvtq_f32_u32(v);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 x = _mm_load_ss(reinterpret_cast<const float*>(&pSource->x));
			__m128 y = _mm_load_ss(reinterpret_cast<const float*>(&pSource->y));
			__m128 z = _mm_load_ss(reinterpret_cast<const float*>(&pSource->z));
			__m128 xy = _mm_unpacklo_ps(x, y);
			__m128 V = _mm_movelh_ps(xy, z);
			// For the values that are higher than 0x7FFFFFFF, a fixup is needed
			// Determine which ones need the fix.
			Vector vMask = _mm_and_ps(V, g_XMNegativeZero);
			// Force all values positive
			Vector vResult = _mm_xor_ps(V, vMask);
			// Convert to floats
			vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
			// Convert 0x80000000 -> 0xFFFFFFFF
			__m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
			// For only the ones that are too big, add the fixup
			vMask = _mm_and_ps(_mm_castsi128_ps(iMask), g_XMFixUnsigned);
			vResult = _mm_add_ps(vResult, vMask);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadInt4
		(
			const uint32_t* pSource
		)
		{
			assert(pSource);

#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_u32[0] = pSource[0];
			V.vector4_u32[1] = pSource[1];
			V.vector4_u32[2] = pSource[2];
			V.vector4_u32[3] = pSource[3];
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vld1q_u32(pSource);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i V = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadInt4A
		(
			const uint32_t* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_u32[0] = pSource[0];
			V.vector4_u32[1] = pSource[1];
			V.vector4_u32[2] = pSource[2];
			V.vector4_u32[3] = pSource[3];
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vld1q_u32_ex(pSource, 128);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i V = _mm_load_si128(reinterpret_cast<const __m128i*>(pSource));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadFloat4
		(
			const Float4* pSource
		)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_f32[0] = pSource->x;
			V.vector4_f32[1] = pSource->y;
			V.vector4_f32[2] = pSource->z;
			V.vector4_f32[3] = pSource->w;
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vld1q_f32(reinterpret_cast<const float*>(pSource));
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_loadu_ps(&pSource->x);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadFloat4A
		(
			const Float4A* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_f32[0] = pSource->x;
			V.vector4_f32[1] = pSource->y;
			V.vector4_f32[2] = pSource->z;
			V.vector4_f32[3] = pSource->w;
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vld1q_f32_ex(reinterpret_cast<const float*>(pSource), 128);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_load_ps(&pSource->x);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadSInt4
		(
			const Int4* pSource
		)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)

			Vector V;
			V.vector4_f32[0] = static_cast<float>(pSource->x);
			V.vector4_f32[1] = static_cast<float>(pSource->y);
			V.vector4_f32[2] = static_cast<float>(pSource->z);
			V.vector4_f32[3] = static_cast<float>(pSource->w);
			return V;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			int32x4_t v = vld1q_s32(reinterpret_cast<const int32_t*>(pSource));
			return vcvtq_f32_s32(v);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i V = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource));
			return _mm_cvtepi32_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV LoadUInt4
		(
			const UInt4* pSource
		)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)
			Vector V;
			V.vector4_f32[0] = static_cast<float>(pSource->x);
			V.vector4_f32[1] = static_cast<float>(pSource->y);
			V.vector4_f32[2] = static_cast<float>(pSource->z);
			V.vector4_f32[3] = static_cast<float>(pSource->w);
			return V;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t v = vld1q_u32(reinterpret_cast<const uint32_t*>(pSource));
			return vcvtq_f32_u32(v);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i V = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource));
			// For the values that are higher than 0x7FFFFFFF, a fixup is needed
			// Determine which ones need the fix.
			Vector vMask = _mm_and_ps(_mm_castsi128_ps(V), g_XMNegativeZero);
			// Force all values positive
			Vector vResult = _mm_xor_ps(_mm_castsi128_ps(V), vMask);
			// Convert to floats
			vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
			// Convert 0x80000000 -> 0xFFFFFFFF
			__m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
			// For only the ones that are too big, add the fixup
			vMask = _mm_and_ps(_mm_castsi128_ps(iMask), g_XMFixUnsigned);
			vResult = _mm_add_ps(vResult, vMask);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV LoadFloat3x3
		(
			const Float3X3* pSource
		)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)

			Matrix M;
			M.r[0].vector4_f32[0] = pSource->m[0][0];
			M.r[0].vector4_f32[1] = pSource->m[0][1];
			M.r[0].vector4_f32[2] = pSource->m[0][2];
			M.r[0].vector4_f32[3] = 0.0f;

			M.r[1].vector4_f32[0] = pSource->m[1][0];
			M.r[1].vector4_f32[1] = pSource->m[1][1];
			M.r[1].vector4_f32[2] = pSource->m[1][2];
			M.r[1].vector4_f32[3] = 0.0f;

			M.r[2].vector4_f32[0] = pSource->m[2][0];
			M.r[2].vector4_f32[1] = pSource->m[2][1];
			M.r[2].vector4_f32[2] = pSource->m[2][2];
			M.r[2].vector4_f32[3] = 0.0f;
			M.r[3].vector4_f32[0] = 0.0f;
			M.r[3].vector4_f32[1] = 0.0f;
			M.r[3].vector4_f32[2] = 0.0f;
			M.r[3].vector4_f32[3] = 1.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4_t v0 = vld1q_f32(&pSource->m[0][0]);
			float32x4_t v1 = vld1q_f32(&pSource->m[1][1]);
			float32x2_t v2 = vcreate_f32(static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&pSource->m[2][2])));
			float32x4_t T = vextq_f32(v0, v1, 3);

			Matrix M;
			M.r[0] = vandq_u32(v0, g_XMMask3);
			M.r[1] = vandq_u32(T, g_XMMask3);
			M.r[2] = vcombine_f32(vget_high_f32(v1), v2);
			M.r[3] = g_XMIdentityR3;
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 Z = _mm_setzero_ps();

			__m128 V1 = _mm_loadu_ps(&pSource->m[0][0]);
			__m128 V2 = _mm_loadu_ps(&pSource->m[1][1]);
			__m128 V3 = _mm_load_ss(&pSource->m[2][2]);

			__m128 T1 = _mm_unpackhi_ps(V1, Z);
			__m128 T2 = _mm_unpacklo_ps(V2, Z);
			__m128 T3 = _mm_shuffle_ps(V3, T2, _MM_SHUFFLE(0, 1, 0, 0));
			__m128 T4 = _mm_movehl_ps(T2, T3);
			__m128 T5 = _mm_movehl_ps(Z, T1);

			Matrix M;
			M.r[0] = _mm_movelh_ps(V1, T1);
			M.r[1] = _mm_add_ps(T4, T5);
			M.r[2] = _mm_shuffle_ps(V2, V3, _MM_SHUFFLE(1, 0, 3, 2));
			M.r[3] = g_XMIdentityR3;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV LoadFloat4x3
		(
			const Float4X3* pSource
		)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)

			Matrix M;
			M.r[0].vector4_f32[0] = pSource->m[0][0];
			M.r[0].vector4_f32[1] = pSource->m[0][1];
			M.r[0].vector4_f32[2] = pSource->m[0][2];
			M.r[0].vector4_f32[3] = 0.0f;

			M.r[1].vector4_f32[0] = pSource->m[1][0];
			M.r[1].vector4_f32[1] = pSource->m[1][1];
			M.r[1].vector4_f32[2] = pSource->m[1][2];
			M.r[1].vector4_f32[3] = 0.0f;

			M.r[2].vector4_f32[0] = pSource->m[2][0];
			M.r[2].vector4_f32[1] = pSource->m[2][1];
			M.r[2].vector4_f32[2] = pSource->m[2][2];
			M.r[2].vector4_f32[3] = 0.0f;

			M.r[3].vector4_f32[0] = pSource->m[3][0];
			M.r[3].vector4_f32[1] = pSource->m[3][1];
			M.r[3].vector4_f32[2] = pSource->m[3][2];
			M.r[3].vector4_f32[3] = 1.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4_t v0 = vld1q_f32(&pSource->m[0][0]);
			float32x4_t v1 = vld1q_f32(&pSource->m[1][1]);
			float32x4_t v2 = vld1q_f32(&pSource->m[2][2]);

			float32x4_t T1 = vextq_f32(v0, v1, 3);
			float32x4_t T2 = vcombine_f32(vget_high_f32(v1), vget_low_f32(v2));
			float32x4_t T3 = vextq_f32(v2, v2, 1);

			Matrix M;
			M.r[0] = vandq_u32(v0, g_XMMask3);
			M.r[1] = vandq_u32(T1, g_XMMask3);
			M.r[2] = vandq_u32(T2, g_XMMask3);
			M.r[3] = vsetq_lane_f32(1.f, T3, 3);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			// Use unaligned load instructions to 
			// load the 12 floats
			// vTemp1 = x1,y1,z1,x2
			Vector vTemp1 = _mm_loadu_ps(&pSource->m[0][0]);
			// vTemp2 = y2,z2,x3,y3
			Vector vTemp2 = _mm_loadu_ps(&pSource->m[1][1]);
			// vTemp4 = z3,x4,y4,z4
			Vector vTemp4 = _mm_loadu_ps(&pSource->m[2][2]);
			// vTemp3 = x3,y3,z3,z3
			Vector vTemp3 = _mm_shuffle_ps(vTemp2, vTemp4, _MM_SHUFFLE(0, 0, 3, 2));
			// vTemp2 = y2,z2,x2,x2
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp1, _MM_SHUFFLE(3, 3, 1, 0));
			// vTemp2 = x2,y2,z2,z2
			vTemp2 = XM_PERMUTE_PS(vTemp2, _MM_SHUFFLE(1, 1, 0, 2));
			// vTemp1 = x1,y1,z1,0
			vTemp1 = _mm_and_ps(vTemp1, g_XMMask3);
			// vTemp2 = x2,y2,z2,0
			vTemp2 = _mm_and_ps(vTemp2, g_XMMask3);
			// vTemp3 = x3,y3,z3,0
			vTemp3 = _mm_and_ps(vTemp3, g_XMMask3);
			// vTemp4i = x4,y4,z4,0
			__m128i vTemp4i = _mm_srli_si128(_mm_castps_si128(vTemp4), 32 / 8);
			// vTemp4i = x4,y4,z4,1.0f
			vTemp4i = _mm_or_si128(vTemp4i, g_XMIdentityR3);
			Matrix M(vTemp1,
				vTemp2,
				vTemp3,
				_mm_castsi128_ps(vTemp4i));
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV LoadFloat4x3A
		(
			const Float4X3A* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)

			Matrix M;
			M.r[0].vector4_f32[0] = pSource->m[0][0];
			M.r[0].vector4_f32[1] = pSource->m[0][1];
			M.r[0].vector4_f32[2] = pSource->m[0][2];
			M.r[0].vector4_f32[3] = 0.0f;

			M.r[1].vector4_f32[0] = pSource->m[1][0];
			M.r[1].vector4_f32[1] = pSource->m[1][1];
			M.r[1].vector4_f32[2] = pSource->m[1][2];
			M.r[1].vector4_f32[3] = 0.0f;

			M.r[2].vector4_f32[0] = pSource->m[2][0];
			M.r[2].vector4_f32[1] = pSource->m[2][1];
			M.r[2].vector4_f32[2] = pSource->m[2][2];
			M.r[2].vector4_f32[3] = 0.0f;

			M.r[3].vector4_f32[0] = pSource->m[3][0];
			M.r[3].vector4_f32[1] = pSource->m[3][1];
			M.r[3].vector4_f32[2] = pSource->m[3][2];
			M.r[3].vector4_f32[3] = 1.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4_t v0 = vld1q_f32_ex(&pSource->m[0][0], 128);
			float32x4_t v1 = vld1q_f32_ex(&pSource->m[1][1], 128);
			float32x4_t v2 = vld1q_f32_ex(&pSource->m[2][2], 128);

			float32x4_t T1 = vextq_f32(v0, v1, 3);
			float32x4_t T2 = vcombine_f32(vget_high_f32(v1), vget_low_f32(v2));
			float32x4_t T3 = vextq_f32(v2, v2, 1);

			Matrix M;
			M.r[0] = vandq_u32(v0, g_XMMask3);
			M.r[1] = vandq_u32(T1, g_XMMask3);
			M.r[2] = vandq_u32(T2, g_XMMask3);
			M.r[3] = vsetq_lane_f32(1.f, T3, 3);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			// Use aligned load instructions to 
			// load the 12 floats
			// vTemp1 = x1,y1,z1,x2
			Vector vTemp1 = _mm_load_ps(&pSource->m[0][0]);
			// vTemp2 = y2,z2,x3,y3
			Vector vTemp2 = _mm_load_ps(&pSource->m[1][1]);
			// vTemp4 = z3,x4,y4,z4
			Vector vTemp4 = _mm_load_ps(&pSource->m[2][2]);
			// vTemp3 = x3,y3,z3,z3
			Vector vTemp3 = _mm_shuffle_ps(vTemp2, vTemp4, _MM_SHUFFLE(0, 0, 3, 2));
			// vTemp2 = y2,z2,x2,x2
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp1, _MM_SHUFFLE(3, 3, 1, 0));
			// vTemp2 = x2,y2,z2,z2
			vTemp2 = XM_PERMUTE_PS(vTemp2, _MM_SHUFFLE(1, 1, 0, 2));
			// vTemp1 = x1,y1,z1,0
			vTemp1 = _mm_and_ps(vTemp1, g_XMMask3);
			// vTemp2 = x2,y2,z2,0
			vTemp2 = _mm_and_ps(vTemp2, g_XMMask3);
			// vTemp3 = x3,y3,z3,0
			vTemp3 = _mm_and_ps(vTemp3, g_XMMask3);
			// vTemp4i = x4,y4,z4,0
			__m128i vTemp4i = _mm_srli_si128(_mm_castps_si128(vTemp4), 32 / 8);
			// vTemp4i = x4,y4,z4,1.0f
			vTemp4i = _mm_or_si128(vTemp4i, g_XMIdentityR3);
			Matrix M(vTemp1,
				vTemp2,
				vTemp3,
				_mm_castsi128_ps(vTemp4i));
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV LoadFloat3x4
		(
			const Float3X4* pSource
		)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)

			Matrix M;
			M.r[0].vector4_f32[0] = pSource->m[0][0];
			M.r[0].vector4_f32[1] = pSource->m[1][0];
			M.r[0].vector4_f32[2] = pSource->m[2][0];
			M.r[0].vector4_f32[3] = 0.0f;

			M.r[1].vector4_f32[0] = pSource->m[0][1];
			M.r[1].vector4_f32[1] = pSource->m[1][1];
			M.r[1].vector4_f32[2] = pSource->m[2][1];
			M.r[1].vector4_f32[3] = 0.0f;

			M.r[2].vector4_f32[0] = pSource->m[0][2];
			M.r[2].vector4_f32[1] = pSource->m[1][2];
			M.r[2].vector4_f32[2] = pSource->m[2][2];
			M.r[2].vector4_f32[3] = 0.0f;

			M.r[3].vector4_f32[0] = pSource->m[0][3];
			M.r[3].vector4_f32[1] = pSource->m[1][3];
			M.r[3].vector4_f32[2] = pSource->m[2][3];
			M.r[3].vector4_f32[3] = 1.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2x4_t vTemp0 = vld4_f32(&pSource->_11);
			float32x4_t vTemp1 = vld1q_f32(&pSource->_31);

			float32x2_t l = vget_low_f32(vTemp1);
			float32x4_t T0 = vcombine_f32(vTemp0.val[0], l);
			float32x2_t rl = vrev64_f32(l);
			float32x4_t T1 = vcombine_f32(vTemp0.val[1], rl);

			float32x2_t h = vget_high_f32(vTemp1);
			float32x4_t T2 = vcombine_f32(vTemp0.val[2], h);
			float32x2_t rh = vrev64_f32(h);
			float32x4_t T3 = vcombine_f32(vTemp0.val[3], rh);

			Matrix M = {};
			M.r[0] = vandq_u32(T0, g_XMMask3);
			M.r[1] = vandq_u32(T1, g_XMMask3);
			M.r[2] = vandq_u32(T2, g_XMMask3);
			M.r[3] = vsetq_lane_f32(1.f, T3, 3);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			Matrix M;
			M.r[0] = _mm_loadu_ps(&pSource->_11);
			M.r[1] = _mm_loadu_ps(&pSource->_21);
			M.r[2] = _mm_loadu_ps(&pSource->_31);
			M.r[3] = g_XMIdentityR3;

			// x.x,x.y,y.x,y.y
			Vector vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
			// x.z,x.w,y.z,y.w
			Vector vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
			// z.x,z.y,w.x,w.y
			Vector vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
			// z.z,z.w,w.z,w.w
			Vector vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));
			Matrix mResult;

			// x.x,y.x,z.x,w.x
			mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			// x.y,y.y,z.y,w.y
			mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			// x.z,y.z,z.z,w.z
			mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
			// x.w,y.w,z.w,w.w
			mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
			return mResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV LoadFloat3x4A
		(
			const Float3X4A* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)

			Matrix M;
			M.r[0].vector4_f32[0] = pSource->m[0][0];
			M.r[0].vector4_f32[1] = pSource->m[1][0];
			M.r[0].vector4_f32[2] = pSource->m[2][0];
			M.r[0].vector4_f32[3] = 0.0f;

			M.r[1].vector4_f32[0] = pSource->m[0][1];
			M.r[1].vector4_f32[1] = pSource->m[1][1];
			M.r[1].vector4_f32[2] = pSource->m[2][1];
			M.r[1].vector4_f32[3] = 0.0f;

			M.r[2].vector4_f32[0] = pSource->m[0][2];
			M.r[2].vector4_f32[1] = pSource->m[1][2];
			M.r[2].vector4_f32[2] = pSource->m[2][2];
			M.r[2].vector4_f32[3] = 0.0f;

			M.r[3].vector4_f32[0] = pSource->m[0][3];
			M.r[3].vector4_f32[1] = pSource->m[1][3];
			M.r[3].vector4_f32[2] = pSource->m[2][3];
			M.r[3].vector4_f32[3] = 1.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2x4_t vTemp0 = vld4_f32_ex(&pSource->_11, 128);
			float32x4_t vTemp1 = vld1q_f32_ex(&pSource->_31, 128);

			float32x2_t l = vget_low_f32(vTemp1);
			float32x4_t T0 = vcombine_f32(vTemp0.val[0], l);
			float32x2_t rl = vrev64_f32(l);
			float32x4_t T1 = vcombine_f32(vTemp0.val[1], rl);

			float32x2_t h = vget_high_f32(vTemp1);
			float32x4_t T2 = vcombine_f32(vTemp0.val[2], h);
			float32x2_t rh = vrev64_f32(h);
			float32x4_t T3 = vcombine_f32(vTemp0.val[3], rh);

			Matrix M = {};
			M.r[0] = vandq_u32(T0, g_XMMask3);
			M.r[1] = vandq_u32(T1, g_XMMask3);
			M.r[2] = vandq_u32(T2, g_XMMask3);
			M.r[3] = vsetq_lane_f32(1.f, T3, 3);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			Matrix M;
			M.r[0] = _mm_load_ps(&pSource->_11);
			M.r[1] = _mm_load_ps(&pSource->_21);
			M.r[2] = _mm_load_ps(&pSource->_31);
			M.r[3] = g_XMIdentityR3;

			// x.x,x.y,y.x,y.y
			Vector vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
			// x.z,x.w,y.z,y.w
			Vector vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
			// z.x,z.y,w.x,w.y
			Vector vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
			// z.z,z.w,w.z,w.w
			Vector vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));
			Matrix mResult;

			// x.x,y.x,z.x,w.x
			mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			// x.y,y.y,z.y,w.y
			mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			// x.z,y.z,z.z,w.z
			mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
			// x.w,y.w,z.w,w.w
			mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
			return mResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV LoadFloat4x4
		(
			const Float4X4* pSource
		)
		{
			assert(pSource);
#if defined(_XM_NO_INTRINSICS_)

			Matrix M;
			M.r[0].vector4_f32[0] = pSource->m[0][0];
			M.r[0].vector4_f32[1] = pSource->m[0][1];
			M.r[0].vector4_f32[2] = pSource->m[0][2];
			M.r[0].vector4_f32[3] = pSource->m[0][3];

			M.r[1].vector4_f32[0] = pSource->m[1][0];
			M.r[1].vector4_f32[1] = pSource->m[1][1];
			M.r[1].vector4_f32[2] = pSource->m[1][2];
			M.r[1].vector4_f32[3] = pSource->m[1][3];

			M.r[2].vector4_f32[0] = pSource->m[2][0];
			M.r[2].vector4_f32[1] = pSource->m[2][1];
			M.r[2].vector4_f32[2] = pSource->m[2][2];
			M.r[2].vector4_f32[3] = pSource->m[2][3];

			M.r[3].vector4_f32[0] = pSource->m[3][0];
			M.r[3].vector4_f32[1] = pSource->m[3][1];
			M.r[3].vector4_f32[2] = pSource->m[3][2];
			M.r[3].vector4_f32[3] = pSource->m[3][3];
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Matrix M;
			M.r[0] = vld1q_f32(reinterpret_cast<const float*>(&pSource->_11));
			M.r[1] = vld1q_f32(reinterpret_cast<const float*>(&pSource->_21));
			M.r[2] = vld1q_f32(reinterpret_cast<const float*>(&pSource->_31));
			M.r[3] = vld1q_f32(reinterpret_cast<const float*>(&pSource->_41));
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			Matrix M;
			M.r[0] = _mm_loadu_ps(&pSource->_11);
			M.r[1] = _mm_loadu_ps(&pSource->_21);
			M.r[2] = _mm_loadu_ps(&pSource->_31);
			M.r[3] = _mm_loadu_ps(&pSource->_41);
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV LoadFloat4x4A
		(
			const Float4X4A* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)

			Matrix M;
			M.r[0].vector4_f32[0] = pSource->m[0][0];
			M.r[0].vector4_f32[1] = pSource->m[0][1];
			M.r[0].vector4_f32[2] = pSource->m[0][2];
			M.r[0].vector4_f32[3] = pSource->m[0][3];

			M.r[1].vector4_f32[0] = pSource->m[1][0];
			M.r[1].vector4_f32[1] = pSource->m[1][1];
			M.r[1].vector4_f32[2] = pSource->m[1][2];
			M.r[1].vector4_f32[3] = pSource->m[1][3];

			M.r[2].vector4_f32[0] = pSource->m[2][0];
			M.r[2].vector4_f32[1] = pSource->m[2][1];
			M.r[2].vector4_f32[2] = pSource->m[2][2];
			M.r[2].vector4_f32[3] = pSource->m[2][3];

			M.r[3].vector4_f32[0] = pSource->m[3][0];
			M.r[3].vector4_f32[1] = pSource->m[3][1];
			M.r[3].vector4_f32[2] = pSource->m[3][2];
			M.r[3].vector4_f32[3] = pSource->m[3][3];
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Matrix M;
			M.r[0] = vld1q_f32_ex(reinterpret_cast<const float*>(&pSource->_11), 128);
			M.r[1] = vld1q_f32_ex(reinterpret_cast<const float*>(&pSource->_21), 128);
			M.r[2] = vld1q_f32_ex(reinterpret_cast<const float*>(&pSource->_31), 128);
			M.r[3] = vld1q_f32_ex(reinterpret_cast<const float*>(&pSource->_41), 128);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			Matrix M;
			M.r[0] = _mm_load_ps(&pSource->_11);
			M.r[1] = _mm_load_ps(&pSource->_21);
			M.r[2] = _mm_load_ps(&pSource->_31);
			M.r[3] = _mm_load_ps(&pSource->_41);
			return M;
#endif
		}

		/****************************************************************************
		 *
		 * Vector and matrix store operations
		 *
		 ****************************************************************************/

		inline void XM_CALLCONV StoreInt
		(
			uint32_t* pDestination,
			FVector V
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
			*pDestination = VectorGetIntX(V);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_lane_u32(pDestination, *reinterpret_cast<const uint32x4_t*>(&V), 0);
#elif defined(_XM_SSE_INTRINSICS_)
			_mm_store_ss(reinterpret_cast<float*>(pDestination), V);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreFloat
		(
			float* pDestination,
			FVector V
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
			*pDestination = VectorGetX(V);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_lane_f32(pDestination, V, 0);
#elif defined(_XM_SSE_INTRINSICS_)
			_mm_store_ss(pDestination, V);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreInt2
		(
			uint32_t* pDestination,
			FVector V
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
			pDestination[0] = V.vector4_u32[0];
			pDestination[1] = V.vector4_u32[1];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t VL = vget_low_u32(V);
			vst1_u32(pDestination, VL);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector T = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(reinterpret_cast<float*>(&pDestination[0]), V);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination[1]), T);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreInt2A
		(
			uint32_t* pDestination,
			FVector V
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)
			pDestination[0] = V.vector4_u32[0];
			pDestination[1] = V.vector4_u32[1];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t VL = vget_low_u32(V);
			vst1_u32_ex(pDestination, VL, 64);
#elif defined(_XM_SSE_INTRINSICS_)
			_mm_storel_epi64(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreFloat2
		(
			Float2* pDestination,
			FVector  V
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
			pDestination->x = V.vector4_f32[0];
			pDestination->y = V.vector4_f32[1];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			vst1_f32(reinterpret_cast<float*>(pDestination), VL);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector T = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(&pDestination->x, V);
			_mm_store_ss(&pDestination->y, T);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreFloat2A
		(
			Float2A* pDestination,
			FVector     V
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)
			pDestination->x = V.vector4_f32[0];
			pDestination->y = V.vector4_f32[1];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			vst1_f32_ex(reinterpret_cast<float*>(pDestination), VL, 64);
#elif defined(_XM_SSE_INTRINSICS_)
			_mm_storel_epi64(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreSInt2
		(
			Int2* pDestination,
			FVector V
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
			pDestination->x = static_cast<int32_t>(V.vector4_f32[0]);
			pDestination->y = static_cast<int32_t>(V.vector4_f32[1]);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			int32x2_t v = vget_low_s32(V);
			v = vcvt_s32_f32(v);
			vst1_s32(reinterpret_cast<int32_t*>(pDestination), v);
#elif defined(_XM_SSE_INTRINSICS_)
			// In case of positive overflow, detect it
			Vector vOverflow = _mm_cmpgt_ps(V, g_XMMaxInt);
			// Float to int conversion
			__m128i vResulti = _mm_cvttps_epi32(V);
			// If there was positive overflow, set to 0x7FFFFFFF
			Vector vResult = _mm_and_ps(vOverflow, g_XMAbsMask);
			vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
			vOverflow = _mm_or_ps(vOverflow, vResult);
			// Write two ints
			Vector T = XM_PERMUTE_PS(vOverflow, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->x), vOverflow);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->y), T);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreUInt2
		(
			UInt2* pDestination,
			FVector V
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
			pDestination->x = static_cast<uint32_t>(V.vector4_f32[0]);
			pDestination->y = static_cast<uint32_t>(V.vector4_f32[1]);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t v = vget_low_f32(V);
			uint32x2_t iv = vcvt_u32_f32(v);
			vst1_u32(reinterpret_cast<uint32_t*>(pDestination), iv);
#elif defined(_XM_SSE_INTRINSICS_)
			// Clamp to >=0
			Vector vResult = _mm_max_ps(V, g_XMZero);
			// Any numbers that are too big, set to 0xFFFFFFFFU
			Vector vOverflow = _mm_cmpgt_ps(vResult, g_XMMaxUInt);
			Vector vValue = g_XMUnsignedFix;
			// Too large for a signed integer?
			Vector vMask = _mm_cmpge_ps(vResult, vValue);
			// Zero for number's lower than 0x80000000, 32768.0f*65536.0f otherwise
			vValue = _mm_and_ps(vValue, vMask);
			// Perform fixup only on numbers too large (Keeps low bit precision)
			vResult = _mm_sub_ps(vResult, vValue);
			__m128i vResulti = _mm_cvttps_epi32(vResult);
			// Convert from signed to unsigned pnly if greater than 0x80000000
			vMask = _mm_and_ps(vMask, g_XMNegativeZero);
			vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
			// On those that are too large, set to 0xFFFFFFFF
			vResult = _mm_or_ps(vResult, vOverflow);
			// Write two uints
			Vector T = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->x), vResult);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->y), T);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreInt3
		(
			uint32_t* pDestination,
			FVector V
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
			pDestination[0] = V.vector4_u32[0];
			pDestination[1] = V.vector4_u32[1];
			pDestination[2] = V.vector4_u32[2];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t VL = vget_low_u32(V);
			vst1_u32(pDestination, VL);
			vst1q_lane_u32(pDestination + 2, *reinterpret_cast<const uint32x4_t*>(&V), 2);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector T1 = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			Vector T2 = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_store_ss(reinterpret_cast<float*>(pDestination), V);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination[1]), T1);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination[2]), T2);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreInt3A
		(
			uint32_t* pDestination,
			FVector V
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)
			pDestination[0] = V.vector4_u32[0];
			pDestination[1] = V.vector4_u32[1];
			pDestination[2] = V.vector4_u32[2];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t VL = vget_low_u32(V);
			vst1_u32_ex(pDestination, VL, 64);
			vst1q_lane_u32(pDestination + 2, *reinterpret_cast<const uint32x4_t*>(&V), 2);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector T = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_storel_epi64(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
			_mm_store_ss(reinterpret_cast<float*>(&pDestination[2]), T);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreFloat3
		(
			Float3* pDestination,
			FVector V
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
			pDestination->x = V.vector4_f32[0];
			pDestination->y = V.vector4_f32[1];
			pDestination->z = V.vector4_f32[2];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			vst1_f32(reinterpret_cast<float*>(pDestination), VL);
			vst1q_lane_f32(reinterpret_cast<float*>(pDestination) + 2, V, 2);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector T1 = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			Vector T2 = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_store_ss(&pDestination->x, V);
			_mm_store_ss(&pDestination->y, T1);
			_mm_store_ss(&pDestination->z, T2);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreFloat3A
		(
			Float3A* pDestination,
			FVector     V
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)
			pDestination->x = V.vector4_f32[0];
			pDestination->y = V.vector4_f32[1];
			pDestination->z = V.vector4_f32[2];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			vst1_f32_ex(reinterpret_cast<float*>(pDestination), VL, 64);
			vst1q_lane_f32(reinterpret_cast<float*>(pDestination) + 2, V, 2);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector T = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_storel_epi64(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
			_mm_store_ss(&pDestination->z, T);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreSInt3
		(
			Int3* pDestination,
			FVector V
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
			pDestination->x = static_cast<int32_t>(V.vector4_f32[0]);
			pDestination->y = static_cast<int32_t>(V.vector4_f32[1]);
			pDestination->z = static_cast<int32_t>(V.vector4_f32[2]);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			int32x4_t v = vcvtq_s32_f32(V);
			int32x2_t vL = vget_low_s32(v);
			vst1_s32(reinterpret_cast<int32_t*>(pDestination), vL);
			vst1q_lane_s32(reinterpret_cast<int32_t*>(pDestination) + 2, v, 2);
#elif defined(_XM_SSE_INTRINSICS_)
			// In case of positive overflow, detect it
			Vector vOverflow = _mm_cmpgt_ps(V, g_XMMaxInt);
			// Float to int conversion
			__m128i vResulti = _mm_cvttps_epi32(V);
			// If there was positive overflow, set to 0x7FFFFFFF
			Vector vResult = _mm_and_ps(vOverflow, g_XMAbsMask);
			vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
			vOverflow = _mm_or_ps(vOverflow, vResult);
			// Write 3 uints
			Vector T1 = XM_PERMUTE_PS(vOverflow, _MM_SHUFFLE(1, 1, 1, 1));
			Vector T2 = XM_PERMUTE_PS(vOverflow, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->x), vOverflow);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->y), T1);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->z), T2);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreUInt3
		(
			UInt3* pDestination,
			FVector V
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
			pDestination->x = static_cast<uint32_t>(V.vector4_f32[0]);
			pDestination->y = static_cast<uint32_t>(V.vector4_f32[1]);
			pDestination->z = static_cast<uint32_t>(V.vector4_f32[2]);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t v = vcvtq_u32_f32(V);
			uint32x2_t vL = vget_low_u32(v);
			vst1_u32(reinterpret_cast<uint32_t*>(pDestination), vL);
			vst1q_lane_u32(reinterpret_cast<uint32_t*>(pDestination) + 2, v, 2);
#elif defined(_XM_SSE_INTRINSICS_)
			// Clamp to >=0
			Vector vResult = _mm_max_ps(V, g_XMZero);
			// Any numbers that are too big, set to 0xFFFFFFFFU
			Vector vOverflow = _mm_cmpgt_ps(vResult, g_XMMaxUInt);
			Vector vValue = g_XMUnsignedFix;
			// Too large for a signed integer?
			Vector vMask = _mm_cmpge_ps(vResult, vValue);
			// Zero for number's lower than 0x80000000, 32768.0f*65536.0f otherwise
			vValue = _mm_and_ps(vValue, vMask);
			// Perform fixup only on numbers too large (Keeps low bit precision)
			vResult = _mm_sub_ps(vResult, vValue);
			__m128i vResulti = _mm_cvttps_epi32(vResult);
			// Convert from signed to unsigned pnly if greater than 0x80000000
			vMask = _mm_and_ps(vMask, g_XMNegativeZero);
			vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
			// On those that are too large, set to 0xFFFFFFFF
			vResult = _mm_or_ps(vResult, vOverflow);
			// Write 3 uints
			Vector T1 = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(1, 1, 1, 1));
			Vector T2 = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->x), vResult);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->y), T1);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->z), T2);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreInt4
		(
			uint32_t* pDestination,
			FVector V
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
			pDestination[0] = V.vector4_u32[0];
			pDestination[1] = V.vector4_u32[1];
			pDestination[2] = V.vector4_u32[2];
			pDestination[3] = V.vector4_u32[3];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_u32(pDestination, V);
#elif defined(_XM_SSE_INTRINSICS_)
			_mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreInt4A
		(
			uint32_t* pDestination,
			FVector V
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)
			pDestination[0] = V.vector4_u32[0];
			pDestination[1] = V.vector4_u32[1];
			pDestination[2] = V.vector4_u32[2];
			pDestination[3] = V.vector4_u32[3];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_u32_ex(pDestination, V, 128);
#elif defined(_XM_SSE_INTRINSICS_)
			_mm_store_si128(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreFloat4
		(
			Float4* pDestination,
			FVector  V
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
			pDestination->x = V.vector4_f32[0];
			pDestination->y = V.vector4_f32[1];
			pDestination->z = V.vector4_f32[2];
			pDestination->w = V.vector4_f32[3];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_f32(reinterpret_cast<float*>(pDestination), V);
#elif defined(_XM_SSE_INTRINSICS_)
			_mm_storeu_ps(&pDestination->x, V);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreFloat4A
		(
			Float4A* pDestination,
			FVector     V
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)
			pDestination->x = V.vector4_f32[0];
			pDestination->y = V.vector4_f32[1];
			pDestination->z = V.vector4_f32[2];
			pDestination->w = V.vector4_f32[3];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_f32_ex(reinterpret_cast<float*>(pDestination), V, 128);
#elif defined(_XM_SSE_INTRINSICS_)
			_mm_store_ps(&pDestination->x, V);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreSInt4
		(
			Int4* pDestination,
			FVector V
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
			pDestination->x = static_cast<int32_t>(V.vector4_f32[0]);
			pDestination->y = static_cast<int32_t>(V.vector4_f32[1]);
			pDestination->z = static_cast<int32_t>(V.vector4_f32[2]);
			pDestination->w = static_cast<int32_t>(V.vector4_f32[3]);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			int32x4_t v = vcvtq_s32_f32(V);
			vst1q_s32(reinterpret_cast<int32_t*>(pDestination), v);
#elif defined(_XM_SSE_INTRINSICS_)
			// In case of positive overflow, detect it
			Vector vOverflow = _mm_cmpgt_ps(V, g_XMMaxInt);
			// Float to int conversion
			__m128i vResulti = _mm_cvttps_epi32(V);
			// If there was positive overflow, set to 0x7FFFFFFF
			Vector vResult = _mm_and_ps(vOverflow, g_XMAbsMask);
			vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
			vOverflow = _mm_or_ps(vOverflow, vResult);
			_mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(vOverflow));
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreUInt4
		(
			UInt4* pDestination,
			FVector V
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
			pDestination->x = static_cast<uint32_t>(V.vector4_f32[0]);
			pDestination->y = static_cast<uint32_t>(V.vector4_f32[1]);
			pDestination->z = static_cast<uint32_t>(V.vector4_f32[2]);
			pDestination->w = static_cast<uint32_t>(V.vector4_f32[3]);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t v = vcvtq_u32_f32(V);
			vst1q_u32(reinterpret_cast<uint32_t*>(pDestination), v);
#elif defined(_XM_SSE_INTRINSICS_)
			// Clamp to >=0
			Vector vResult = _mm_max_ps(V, g_XMZero);
			// Any numbers that are too big, set to 0xFFFFFFFFU
			Vector vOverflow = _mm_cmpgt_ps(vResult, g_XMMaxUInt);
			Vector vValue = g_XMUnsignedFix;
			// Too large for a signed integer?
			Vector vMask = _mm_cmpge_ps(vResult, vValue);
			// Zero for number's lower than 0x80000000, 32768.0f*65536.0f otherwise
			vValue = _mm_and_ps(vValue, vMask);
			// Perform fixup only on numbers too large (Keeps low bit precision)
			vResult = _mm_sub_ps(vResult, vValue);
			__m128i vResulti = _mm_cvttps_epi32(vResult);
			// Convert from signed to unsigned pnly if greater than 0x80000000
			vMask = _mm_and_ps(vMask, g_XMNegativeZero);
			vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
			// On those that are too large, set to 0xFFFFFFFF
			vResult = _mm_or_ps(vResult, vOverflow);
			_mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(vResult));
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreFloat3x3
		(
			Float3X3* pDestination,
			FMatrix	M
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)

			pDestination->m[0][0] = M.r[0].vector4_f32[0];
			pDestination->m[0][1] = M.r[0].vector4_f32[1];
			pDestination->m[0][2] = M.r[0].vector4_f32[2];

			pDestination->m[1][0] = M.r[1].vector4_f32[0];
			pDestination->m[1][1] = M.r[1].vector4_f32[1];
			pDestination->m[1][2] = M.r[1].vector4_f32[2];

			pDestination->m[2][0] = M.r[2].vector4_f32[0];
			pDestination->m[2][1] = M.r[2].vector4_f32[1];
			pDestination->m[2][2] = M.r[2].vector4_f32[2];

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4_t T1 = vextq_f32(M.r[0], M.r[1], 1);
			float32x4_t T2 = vbslq_f32(g_XMMask3, M.r[0], T1);
			vst1q_f32(&pDestination->m[0][0], T2);

			T1 = vextq_f32(M.r[1], M.r[1], 1);
			T2 = vcombine_f32(vget_low_f32(T1), vget_low_f32(M.r[2]));
			vst1q_f32(&pDestination->m[1][1], T2);

			vst1q_lane_f32(&pDestination->m[2][2], M.r[2], 2);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp1 = M.r[0];
			Vector vTemp2 = M.r[1];
			Vector vTemp3 = M.r[2];
			Vector vWork = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(0, 0, 2, 2));
			vTemp1 = _mm_shuffle_ps(vTemp1, vWork, _MM_SHUFFLE(2, 0, 1, 0));
			_mm_storeu_ps(&pDestination->m[0][0], vTemp1);
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp3, _MM_SHUFFLE(1, 0, 2, 1));
			_mm_storeu_ps(&pDestination->m[1][1], vTemp2);
			vTemp3 = XM_PERMUTE_PS(vTemp3, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_store_ss(&pDestination->m[2][2], vTemp3);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreFloat4x3
		(
			Float4X3* pDestination,
			FMatrix M
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)

			pDestination->m[0][0] = M.r[0].vector4_f32[0];
			pDestination->m[0][1] = M.r[0].vector4_f32[1];
			pDestination->m[0][2] = M.r[0].vector4_f32[2];

			pDestination->m[1][0] = M.r[1].vector4_f32[0];
			pDestination->m[1][1] = M.r[1].vector4_f32[1];
			pDestination->m[1][2] = M.r[1].vector4_f32[2];

			pDestination->m[2][0] = M.r[2].vector4_f32[0];
			pDestination->m[2][1] = M.r[2].vector4_f32[1];
			pDestination->m[2][2] = M.r[2].vector4_f32[2];

			pDestination->m[3][0] = M.r[3].vector4_f32[0];
			pDestination->m[3][1] = M.r[3].vector4_f32[1];
			pDestination->m[3][2] = M.r[3].vector4_f32[2];

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4_t T1 = vextq_f32(M.r[0], M.r[1], 1);
			float32x4_t T2 = vbslq_f32(g_XMMask3, M.r[0], T1);
			vst1q_f32(&pDestination->m[0][0], T2);

			T1 = vextq_f32(M.r[1], M.r[1], 1);
			T2 = vcombine_f32(vget_low_f32(T1), vget_low_f32(M.r[2]));
			vst1q_f32(&pDestination->m[1][1], T2);

			T1 = vdupq_lane_f32(vget_high_f32(M.r[2]), 0);
			T2 = vextq_f32(T1, M.r[3], 3);
			vst1q_f32(&pDestination->m[2][2], T2);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp1 = M.r[0];
			Vector vTemp2 = M.r[1];
			Vector vTemp3 = M.r[2];
			Vector vTemp4 = M.r[3];
			Vector vTemp2x = _mm_shuffle_ps(vTemp2, vTemp3, _MM_SHUFFLE(1, 0, 2, 1));
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp1, _MM_SHUFFLE(2, 2, 0, 0));
			vTemp1 = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(0, 2, 1, 0));
			vTemp3 = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(0, 0, 2, 2));
			vTemp3 = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 1, 2, 0));
			_mm_storeu_ps(&pDestination->m[0][0], vTemp1);
			_mm_storeu_ps(&pDestination->m[1][1], vTemp2x);
			_mm_storeu_ps(&pDestination->m[2][2], vTemp3);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreFloat4x3A
		(
			Float4X3A* pDestination,
			FMatrix		M
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)

			pDestination->m[0][0] = M.r[0].vector4_f32[0];
			pDestination->m[0][1] = M.r[0].vector4_f32[1];
			pDestination->m[0][2] = M.r[0].vector4_f32[2];

			pDestination->m[1][0] = M.r[1].vector4_f32[0];
			pDestination->m[1][1] = M.r[1].vector4_f32[1];
			pDestination->m[1][2] = M.r[1].vector4_f32[2];

			pDestination->m[2][0] = M.r[2].vector4_f32[0];
			pDestination->m[2][1] = M.r[2].vector4_f32[1];
			pDestination->m[2][2] = M.r[2].vector4_f32[2];

			pDestination->m[3][0] = M.r[3].vector4_f32[0];
			pDestination->m[3][1] = M.r[3].vector4_f32[1];
			pDestination->m[3][2] = M.r[3].vector4_f32[2];

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4_t T1 = vextq_f32(M.r[0], M.r[1], 1);
			float32x4_t T2 = vbslq_f32(g_XMMask3, M.r[0], T1);
			vst1q_f32_ex(&pDestination->m[0][0], T2, 128);

			T1 = vextq_f32(M.r[1], M.r[1], 1);
			T2 = vcombine_f32(vget_low_f32(T1), vget_low_f32(M.r[2]));
			vst1q_f32_ex(&pDestination->m[1][1], T2, 128);

			T1 = vdupq_lane_f32(vget_high_f32(M.r[2]), 0);
			T2 = vextq_f32(T1, M.r[3], 3);
			vst1q_f32_ex(&pDestination->m[2][2], T2, 128);
#elif defined(_XM_SSE_INTRINSICS_)
			// x1,y1,z1,w1
			Vector vTemp1 = M.r[0];
			// x2,y2,z2,w2
			Vector vTemp2 = M.r[1];
			// x3,y3,z3,w3
			Vector vTemp3 = M.r[2];
			// x4,y4,z4,w4
			Vector vTemp4 = M.r[3];
			// z1,z1,x2,y2
			Vector vTemp = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(1, 0, 2, 2));
			// y2,z2,x3,y3 (Final)
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp3, _MM_SHUFFLE(1, 0, 2, 1));
			// x1,y1,z1,x2 (Final)
			vTemp1 = _mm_shuffle_ps(vTemp1, vTemp, _MM_SHUFFLE(2, 0, 1, 0));
			// z3,z3,x4,x4
			vTemp3 = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(0, 0, 2, 2));
			// z3,x4,y4,z4 (Final)
			vTemp3 = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 1, 2, 0));
			// Store in 3 operations
			_mm_store_ps(&pDestination->m[0][0], vTemp1);
			_mm_store_ps(&pDestination->m[1][1], vTemp2);
			_mm_store_ps(&pDestination->m[2][2], vTemp3);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreFloat3x4
		(
			Float3X4* pDestination,
			FMatrix M
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)

			pDestination->m[0][0] = M.r[0].vector4_f32[0];
			pDestination->m[0][1] = M.r[1].vector4_f32[0];
			pDestination->m[0][2] = M.r[2].vector4_f32[0];
			pDestination->m[0][3] = M.r[3].vector4_f32[0];

			pDestination->m[1][0] = M.r[0].vector4_f32[1];
			pDestination->m[1][1] = M.r[1].vector4_f32[1];
			pDestination->m[1][2] = M.r[2].vector4_f32[1];
			pDestination->m[1][3] = M.r[3].vector4_f32[1];

			pDestination->m[2][0] = M.r[0].vector4_f32[2];
			pDestination->m[2][1] = M.r[1].vector4_f32[2];
			pDestination->m[2][2] = M.r[2].vector4_f32[2];
			pDestination->m[2][3] = M.r[3].vector4_f32[2];

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4x2_t P0 = vzipq_f32(M.r[0], M.r[2]);
			float32x4x2_t P1 = vzipq_f32(M.r[1], M.r[3]);

			float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
			float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);

			vst1q_f32(&pDestination->m[0][0], T0.val[0]);
			vst1q_f32(&pDestination->m[1][0], T0.val[1]);
			vst1q_f32(&pDestination->m[2][0], T1.val[0]);
#elif defined(_XM_SSE_INTRINSICS_)
			// x.x,x.y,y.x,y.y
			Vector vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
			// x.z,x.w,y.z,y.w
			Vector vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
			// z.x,z.y,w.x,w.y
			Vector vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
			// z.z,z.w,w.z,w.w
			Vector vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));

			// x.x,y.x,z.x,w.x
			Vector r0 = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			// x.y,y.y,z.y,w.y
			Vector r1 = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			// x.z,y.z,z.z,w.z
			Vector r2 = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));

			_mm_storeu_ps(&pDestination->m[0][0], r0);
			_mm_storeu_ps(&pDestination->m[1][0], r1);
			_mm_storeu_ps(&pDestination->m[2][0], r2);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreFloat3x4A
		(
			Float3X4A* pDestination,
			FMatrix M
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)

			pDestination->m[0][0] = M.r[0].vector4_f32[0];
			pDestination->m[0][1] = M.r[1].vector4_f32[0];
			pDestination->m[0][2] = M.r[2].vector4_f32[0];
			pDestination->m[0][3] = M.r[3].vector4_f32[0];

			pDestination->m[1][0] = M.r[0].vector4_f32[1];
			pDestination->m[1][1] = M.r[1].vector4_f32[1];
			pDestination->m[1][2] = M.r[2].vector4_f32[1];
			pDestination->m[1][3] = M.r[3].vector4_f32[1];

			pDestination->m[2][0] = M.r[0].vector4_f32[2];
			pDestination->m[2][1] = M.r[1].vector4_f32[2];
			pDestination->m[2][2] = M.r[2].vector4_f32[2];
			pDestination->m[2][3] = M.r[3].vector4_f32[2];

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4x2_t P0 = vzipq_f32(M.r[0], M.r[2]);
			float32x4x2_t P1 = vzipq_f32(M.r[1], M.r[3]);

			float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
			float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);

			vst1q_f32_ex(&pDestination->m[0][0], T0.val[0], 128);
			vst1q_f32_ex(&pDestination->m[1][0], T0.val[1], 128);
			vst1q_f32_ex(&pDestination->m[2][0], T1.val[0], 128);
#elif defined(_XM_SSE_INTRINSICS_)
			// x.x,x.y,y.x,y.y
			Vector vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
			// x.z,x.w,y.z,y.w
			Vector vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
			// z.x,z.y,w.x,w.y
			Vector vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
			// z.z,z.w,w.z,w.w
			Vector vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));

			// x.x,y.x,z.x,w.x
			Vector r0 = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			// x.y,y.y,z.y,w.y
			Vector r1 = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			// x.z,y.z,z.z,w.z
			Vector r2 = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));

			_mm_store_ps(&pDestination->m[0][0], r0);
			_mm_store_ps(&pDestination->m[1][0], r1);
			_mm_store_ps(&pDestination->m[2][0], r2);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreFloat4x4
		(
			Float4X4* pDestination,
			FMatrix M
		)
		{
			assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)

			pDestination->m[0][0] = M.r[0].vector4_f32[0];
			pDestination->m[0][1] = M.r[0].vector4_f32[1];
			pDestination->m[0][2] = M.r[0].vector4_f32[2];
			pDestination->m[0][3] = M.r[0].vector4_f32[3];

			pDestination->m[1][0] = M.r[1].vector4_f32[0];
			pDestination->m[1][1] = M.r[1].vector4_f32[1];
			pDestination->m[1][2] = M.r[1].vector4_f32[2];
			pDestination->m[1][3] = M.r[1].vector4_f32[3];

			pDestination->m[2][0] = M.r[2].vector4_f32[0];
			pDestination->m[2][1] = M.r[2].vector4_f32[1];
			pDestination->m[2][2] = M.r[2].vector4_f32[2];
			pDestination->m[2][3] = M.r[2].vector4_f32[3];

			pDestination->m[3][0] = M.r[3].vector4_f32[0];
			pDestination->m[3][1] = M.r[3].vector4_f32[1];
			pDestination->m[3][2] = M.r[3].vector4_f32[2];
			pDestination->m[3][3] = M.r[3].vector4_f32[3];

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_f32(reinterpret_cast<float*>(&pDestination->_11), M.r[0]);
			vst1q_f32(reinterpret_cast<float*>(&pDestination->_21), M.r[1]);
			vst1q_f32(reinterpret_cast<float*>(&pDestination->_31), M.r[2]);
			vst1q_f32(reinterpret_cast<float*>(&pDestination->_41), M.r[3]);
#elif defined(_XM_SSE_INTRINSICS_)
			_mm_storeu_ps(&pDestination->_11, M.r[0]);
			_mm_storeu_ps(&pDestination->_21, M.r[1]);
			_mm_storeu_ps(&pDestination->_31, M.r[2]);
			_mm_storeu_ps(&pDestination->_41, M.r[3]);
#endif
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV StoreFloat4x4A
		(
			Float4X4A* pDestination,
			FMatrix		M
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(_XM_NO_INTRINSICS_)

			pDestination->m[0][0] = M.r[0].vector4_f32[0];
			pDestination->m[0][1] = M.r[0].vector4_f32[1];
			pDestination->m[0][2] = M.r[0].vector4_f32[2];
			pDestination->m[0][3] = M.r[0].vector4_f32[3];

			pDestination->m[1][0] = M.r[1].vector4_f32[0];
			pDestination->m[1][1] = M.r[1].vector4_f32[1];
			pDestination->m[1][2] = M.r[1].vector4_f32[2];
			pDestination->m[1][3] = M.r[1].vector4_f32[3];

			pDestination->m[2][0] = M.r[2].vector4_f32[0];
			pDestination->m[2][1] = M.r[2].vector4_f32[1];
			pDestination->m[2][2] = M.r[2].vector4_f32[2];
			pDestination->m[2][3] = M.r[2].vector4_f32[3];

			pDestination->m[3][0] = M.r[3].vector4_f32[0];
			pDestination->m[3][1] = M.r[3].vector4_f32[1];
			pDestination->m[3][2] = M.r[3].vector4_f32[2];
			pDestination->m[3][3] = M.r[3].vector4_f32[3];

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_f32_ex(reinterpret_cast<float*>(&pDestination->_11), M.r[0], 128);
			vst1q_f32_ex(reinterpret_cast<float*>(&pDestination->_21), M.r[1], 128);
			vst1q_f32_ex(reinterpret_cast<float*>(&pDestination->_31), M.r[2], 128);
			vst1q_f32_ex(reinterpret_cast<float*>(&pDestination->_41), M.r[3], 128);
#elif defined(_XM_SSE_INTRINSICS_)
			_mm_store_ps(&pDestination->_11, M.r[0]);
			_mm_store_ps(&pDestination->_21, M.r[1]);
			_mm_store_ps(&pDestination->_31, M.r[2]);
			_mm_store_ps(&pDestination->_41, M.r[3]);
#endif
		}




		//-------------------------------------------------------------------------------------
	// DirectXMathVector.inl -- SIMD C++ Math library
	//
	// Copyright (c) Microsoft Corporation. All rights reserved.
	// Licensed under the MIT License.
	//
	// http://go.microsoft.com/fwlink/?LinkID=615560
	//-------------------------------------------------------------------------------------


#if defined(_XM_NO_INTRINSICS_)
#define XMISNAN(x)  ((*(const uint32_t*)&(x) & 0x7F800000) == 0x7F800000 && (*(const uint32_t*)&(x) & 0x7FFFFF) != 0)
#define XMISINF(x)  ((*(const uint32_t*)&(x) & 0x7FFFFFFF) == 0x7F800000)
#endif

#if defined(_XM_SSE_INTRINSICS_)

#define XM3UNPACK3INTO4(l1,l2,l3) \
    Vector V3 = _mm_shuffle_ps(l2,l3,_MM_SHUFFLE(0,0,3,2));\
    Vector V2 = _mm_shuffle_ps(l2,l1,_MM_SHUFFLE(3,3,1,0));\
    V2 = XM_PERMUTE_PS(V2,_MM_SHUFFLE(1,1,0,2));\
    Vector V4 = _mm_castsi128_ps( _mm_srli_si128(_mm_castps_si128(L3),32/8) );

#define XM3PACK4INTO3(v2x) \
    v2x = _mm_shuffle_ps(V2,V3,_MM_SHUFFLE(1,0,2,1));\
    V2 = _mm_shuffle_ps(V2,V1,_MM_SHUFFLE(2,2,0,0));\
    V1 = _mm_shuffle_ps(V1,V2,_MM_SHUFFLE(0,2,1,0));\
    V3 = _mm_shuffle_ps(V3,V4,_MM_SHUFFLE(0,0,2,2));\
    V3 = _mm_shuffle_ps(V3,V4,_MM_SHUFFLE(2,1,2,0));\

#endif

/****************************************************************************
 *
 * General Vector
 *
 ****************************************************************************/

 //------------------------------------------------------------------------------
 // Assignment operations
 //------------------------------------------------------------------------------

 //------------------------------------------------------------------------------
 // Return a vector with all elements equaling zero
		inline Vector XM_CALLCONV VectorZero()
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 vResult = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vdupq_n_f32(0);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_setzero_ps();
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with four floating point values
		inline Vector XM_CALLCONV VectorSet
		(
			float x,
			float y,
			float z,
			float w
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 vResult = { { { x, y, z, w } } };
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t V0 = vcreate_f32(
				static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&x))
				| (static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&y)) << 32));
			float32x2_t V1 = vcreate_f32(
				static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&z))
				| (static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&w)) << 32));
			return vcombine_f32(V0, V1);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_set_ps(w, z, y, x);
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with four integer values
		inline Vector XM_CALLCONV VectorSetInt
		(
			uint32_t x,
			uint32_t y,
			uint32_t z,
			uint32_t w
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 vResult = { { { x, y, z, w } } };
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t V0 = vcreate_u32(static_cast<uint64_t>(x) | (static_cast<uint64_t>(y) << 32));
			uint32x2_t V1 = vcreate_u32(static_cast<uint64_t>(z) | (static_cast<uint64_t>(w) << 32));
			return vcombine_u32(V0, V1);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i V = _mm_set_epi32(static_cast<int>(w), static_cast<int>(z), static_cast<int>(y), static_cast<int>(x));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with a replicated floating point value
		inline Vector XM_CALLCONV VectorReplicate
		(
			float Value
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = Value;
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vdupq_n_f32(Value);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_set_ps1(Value);
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with a replicated floating point value passed by pointer

		inline Vector XM_CALLCONV VectorReplicatePtr
		(
			const float* pValue
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			float Value = pValue[0];
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = Value;
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vld1q_dup_f32(pValue);
#elif defined(_XM_AVX_INTRINSICS_)
			return _mm_broadcast_ss(pValue);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_load_ps1(pValue);
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with a replicated integer value
		inline Vector XM_CALLCONV VectorReplicateInt
		(
			uint32_t Value
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 vResult;
			vResult.u[0] =
				vResult.u[1] =
				vResult.u[2] =
				vResult.u[3] = Value;
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vdupq_n_u32(Value);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i vTemp = _mm_set1_epi32(static_cast<int>(Value));
			return _mm_castsi128_ps(vTemp);
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with a replicated integer value passed by pointer

		inline Vector XM_CALLCONV VectorReplicateIntPtr
		(
			const uint32_t* pValue
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			uint32_t Value = pValue[0];
			VectorU32 vResult;
			vResult.u[0] =
				vResult.u[1] =
				vResult.u[2] =
				vResult.u[3] = Value;
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vld1q_dup_u32(pValue);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_load_ps1(reinterpret_cast<const float*>(pValue));
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with all bits set (true mask)
		inline Vector XM_CALLCONV VectorTrueInt()
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 vResult = { { { 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU } } };
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vdupq_n_s32(-1);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i V = _mm_set1_epi32(-1);
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with all bits clear (false mask)
		inline Vector XM_CALLCONV VectorFalseInt()
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 vResult = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
			return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vdupq_n_u32(0);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_setzero_ps();
#endif
		}

		//------------------------------------------------------------------------------
		// Replicate the x component of the vector
		inline Vector XM_CALLCONV VectorSplatX
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = V.vector4_f32[0];
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vdupq_lane_f32(vget_low_f32(V), 0);
#elif defined(_XM_AVX2_INTRINSICS_)
			return _mm_broadcastss_ps(V);
#elif defined(_XM_SSE_INTRINSICS_)
			return XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
#endif
		}

		//------------------------------------------------------------------------------
		// Replicate the y component of the vector
		inline Vector XM_CALLCONV VectorSplatY
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = V.vector4_f32[1];
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vdupq_lane_f32(vget_low_f32(V), 1);
#elif defined(_XM_SSE_INTRINSICS_)
			return XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
#endif
		}

		//------------------------------------------------------------------------------
		// Replicate the z component of the vector
		inline Vector XM_CALLCONV VectorSplatZ
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = V.vector4_f32[2];
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vdupq_lane_f32(vget_high_f32(V), 0);
#elif defined(_XM_SSE_INTRINSICS_)
			return XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
#endif
		}

		//------------------------------------------------------------------------------
		// Replicate the w component of the vector
		inline Vector XM_CALLCONV VectorSplatW
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = V.vector4_f32[3];
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vdupq_lane_f32(vget_high_f32(V), 1);
#elif defined(_XM_SSE_INTRINSICS_)
			return XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
#endif
		}

		//------------------------------------------------------------------------------
		// Return a vector of 1.0f,1.0f,1.0f,1.0f
		inline Vector XM_CALLCONV VectorSplatOne()
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = 1.0f;
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vdupq_n_f32(1.0f);
#elif defined(_XM_SSE_INTRINSICS_)
			return g_XMOne;
#endif
		}

		//------------------------------------------------------------------------------
		// Return a vector of INF,INF,INF,INF
		inline Vector XM_CALLCONV VectorSplatInfinity()
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 vResult;
			vResult.u[0] =
				vResult.u[1] =
				vResult.u[2] =
				vResult.u[3] = 0x7F800000;
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vdupq_n_u32(0x7F800000);
#elif defined(_XM_SSE_INTRINSICS_)
			return g_XMInfinity;
#endif
		}

		//------------------------------------------------------------------------------
		// Return a vector of Q_NAN,Q_NAN,Q_NAN,Q_NAN
		inline Vector XM_CALLCONV VectorSplatQNaN()
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 vResult;
			vResult.u[0] =
				vResult.u[1] =
				vResult.u[2] =
				vResult.u[3] = 0x7FC00000;
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vdupq_n_u32(0x7FC00000);
#elif defined(_XM_SSE_INTRINSICS_)
			return g_XMQNaN;
#endif
		}

		//------------------------------------------------------------------------------
		// Return a vector of 1.192092896e-7f,1.192092896e-7f,1.192092896e-7f,1.192092896e-7f
		inline Vector XM_CALLCONV VectorSplatEpsilon()
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 vResult;
			vResult.u[0] =
				vResult.u[1] =
				vResult.u[2] =
				vResult.u[3] = 0x34000000;
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vdupq_n_u32(0x34000000);
#elif defined(_XM_SSE_INTRINSICS_)
			return g_XMEpsilon;
#endif
		}

		//------------------------------------------------------------------------------
		// Return a vector of -0.0f (0x80000000),-0.0f,-0.0f,-0.0f
		inline Vector XM_CALLCONV VectorSplatSignMask()
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 vResult;
			vResult.u[0] =
				vResult.u[1] =
				vResult.u[2] =
				vResult.u[3] = 0x80000000U;
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vdupq_n_u32(0x80000000U);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i V = _mm_set1_epi32(static_cast<int>(0x80000000));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------
		// Return a floating point value via an index. This is not a recommended
		// function to use due to performance loss.
		inline float XM_CALLCONV VectorGetByIndex(FVector V, size_t i)
		{
			assert(i < 4);
			_Analysis_assume_(i < 4);
#if defined(_XM_NO_INTRINSICS_)
			return V.vector4_f32[i];
#else
			VectorF32 U;
			U.v = V;
			return U.f[i];
#endif
		}

		//------------------------------------------------------------------------------
		// Return the X component in an FPU register. 
		inline float XM_CALLCONV VectorGetX(FVector V)
		{
#if defined(_XM_NO_INTRINSICS_)
			return V.vector4_f32[0];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vgetq_lane_f32(V, 0);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_cvtss_f32(V);
#endif
		}

		// Return the Y component in an FPU register. 
		inline float XM_CALLCONV VectorGetY(FVector V)
		{
#if defined(_XM_NO_INTRINSICS_)
			return V.vector4_f32[1];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vgetq_lane_f32(V, 1);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			return _mm_cvtss_f32(vTemp);
#endif
		}

		// Return the Z component in an FPU register. 
		inline float XM_CALLCONV VectorGetZ(FVector V)
		{
#if defined(_XM_NO_INTRINSICS_)
			return V.vector4_f32[2];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vgetq_lane_f32(V, 2);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			return _mm_cvtss_f32(vTemp);
#endif
		}

		// Return the W component in an FPU register. 
		inline float XM_CALLCONV VectorGetW(FVector V)
		{
#if defined(_XM_NO_INTRINSICS_)
			return V.vector4_f32[3];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vgetq_lane_f32(V, 3);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
			return _mm_cvtss_f32(vTemp);
#endif
		}

		//------------------------------------------------------------------------------

		// Store a component indexed by i into a 32 bit float location in memory.

		inline void XM_CALLCONV VectorGetByIndexPtr(float* f, FVector V, size_t i)
		{
			assert(f != nullptr);
			assert(i < 4);
			_Analysis_assume_(i < 4);
#if defined(_XM_NO_INTRINSICS_)
			*f = V.vector4_f32[i];
#else
			VectorF32 U;
			U.v = V;
			*f = U.f[i];
#endif
		}

		//------------------------------------------------------------------------------

		// Store the X component into a 32 bit float location in memory.

		inline void XM_CALLCONV VectorGetXPtr(float* x, FVector V)
		{
			assert(x != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			*x = V.vector4_f32[0];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_lane_f32(x, V, 0);
#elif defined(_XM_SSE_INTRINSICS_)
			_mm_store_ss(x, V);
#endif
		}

		// Store the Y component into a 32 bit float location in memory.

		inline void XM_CALLCONV VectorGetYPtr(float* y, FVector V)
		{
			assert(y != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			*y = V.vector4_f32[1];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_lane_f32(y, V, 1);
#elif defined(_XM_SSE4_INTRINSICS_)
			* (reinterpret_cast<int*>(y)) = _mm_extract_ps(V, 1);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(y, vResult);
#endif
		}

		// Store the Z component into a 32 bit float location in memory.

		inline void XM_CALLCONV VectorGetZPtr(float* z, FVector V)
		{
			assert(z != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			*z = V.vector4_f32[2];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_lane_f32(z, V, 2);
#elif defined(_XM_SSE4_INTRINSICS_)
			* (reinterpret_cast<int*>(z)) = _mm_extract_ps(V, 2);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_store_ss(z, vResult);
#endif
		}

		// Store the W component into a 32 bit float location in memory.

		inline void XM_CALLCONV VectorGetWPtr(float* w, FVector V)
		{
			assert(w != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			*w = V.vector4_f32[3];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_lane_f32(w, V, 3);
#elif defined(_XM_SSE4_INTRINSICS_)
			* (reinterpret_cast<int*>(w)) = _mm_extract_ps(V, 3);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
			_mm_store_ss(w, vResult);
#endif
		}

		//------------------------------------------------------------------------------

		// Return an integer value via an index. This is not a recommended
		// function to use due to performance loss.
		inline uint32_t XM_CALLCONV VectorGetIntByIndex(FVector V, size_t i)
		{
			assert(i < 4);
			_Analysis_assume_(i < 4);
#if defined(_XM_NO_INTRINSICS_)
			return V.vector4_u32[i];
#else
			VectorU32 U;
			U.v = V;
			return U.u[i];
#endif
		}

		//------------------------------------------------------------------------------

		// Return the X component in an integer register. 
		inline uint32_t XM_CALLCONV VectorGetIntX(FVector V)
		{
#if defined(_XM_NO_INTRINSICS_)
			return V.vector4_u32[0];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vgetq_lane_u32(V, 0);
#elif defined(_XM_SSE_INTRINSICS_)
			return static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(V)));
#endif
		}

		// Return the Y component in an integer register. 
		inline uint32_t XM_CALLCONV VectorGetIntY(FVector V)
		{
#if defined(_XM_NO_INTRINSICS_)
			return V.vector4_u32[1];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vgetq_lane_u32(V, 1);
#elif defined(_XM_SSE4_INTRINSICS_)
			__m128i V1 = _mm_castps_si128(V);
			return static_cast<uint32_t>(_mm_extract_epi32(V1, 1));
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i vResulti = _mm_shuffle_epi32(_mm_castps_si128(V), _MM_SHUFFLE(1, 1, 1, 1));
			return static_cast<uint32_t>(_mm_cvtsi128_si32(vResulti));
#endif
		}

		// Return the Z component in an integer register. 
		inline uint32_t XM_CALLCONV VectorGetIntZ(FVector V)
		{
#if defined(_XM_NO_INTRINSICS_)
			return V.vector4_u32[2];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vgetq_lane_u32(V, 2);
#elif defined(_XM_SSE4_INTRINSICS_)
			__m128i V1 = _mm_castps_si128(V);
			return static_cast<uint32_t>(_mm_extract_epi32(V1, 2));
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i vResulti = _mm_shuffle_epi32(_mm_castps_si128(V), _MM_SHUFFLE(2, 2, 2, 2));
			return static_cast<uint32_t>(_mm_cvtsi128_si32(vResulti));
#endif
		}

		// Return the W component in an integer register. 
		inline uint32_t XM_CALLCONV VectorGetIntW(FVector V)
		{
#if defined(_XM_NO_INTRINSICS_)
			return V.vector4_u32[3];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vgetq_lane_u32(V, 3);
#elif defined(_XM_SSE4_INTRINSICS_)
			__m128i V1 = _mm_castps_si128(V);
			return static_cast<uint32_t>(_mm_extract_epi32(V1, 3));
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i vResulti = _mm_shuffle_epi32(_mm_castps_si128(V), _MM_SHUFFLE(3, 3, 3, 3));
			return static_cast<uint32_t>(_mm_cvtsi128_si32(vResulti));
#endif
		}

		//------------------------------------------------------------------------------

		// Store a component indexed by i into a 32 bit integer location in memory.

		inline void XM_CALLCONV VectorGetIntByIndexPtr(uint32_t* x, FVector V, size_t i)
		{
			assert(x != nullptr);
			assert(i < 4);
			_Analysis_assume_(i < 4);
#if defined(_XM_NO_INTRINSICS_)
			*x = V.vector4_u32[i];
#else
			VectorU32 U;
			U.v = V;
			*x = U.u[i];
#endif
		}

		//------------------------------------------------------------------------------

		// Store the X component into a 32 bit integer location in memory.

		inline void XM_CALLCONV VectorGetIntXPtr(uint32_t* x, FVector V)
		{
			assert(x != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			*x = V.vector4_u32[0];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_lane_u32(x, *reinterpret_cast<const uint32x4_t*>(&V), 0);
#elif defined(_XM_SSE_INTRINSICS_)
			_mm_store_ss(reinterpret_cast<float*>(x), V);
#endif
		}

		// Store the Y component into a 32 bit integer location in memory.

		inline void XM_CALLCONV VectorGetIntYPtr(uint32_t* y, FVector V)
		{
			assert(y != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			*y = V.vector4_u32[1];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_lane_u32(y, *reinterpret_cast<const uint32x4_t*>(&V), 1);
#elif defined(_XM_SSE4_INTRINSICS_)
			__m128i V1 = _mm_castps_si128(V);
			*y = static_cast<uint32_t>(_mm_extract_epi32(V1, 1));
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(reinterpret_cast<float*>(y), vResult);
#endif
		}

		// Store the Z component into a 32 bit integer locaCantion in memory.

		inline void XM_CALLCONV VectorGetIntZPtr(uint32_t* z, FVector V)
		{
			assert(z != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			*z = V.vector4_u32[2];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_lane_u32(z, *reinterpret_cast<const uint32x4_t*>(&V), 2);
#elif defined(_XM_SSE4_INTRINSICS_)
			__m128i V1 = _mm_castps_si128(V);
			*z = static_cast<uint32_t>(_mm_extract_epi32(V1, 2));
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_store_ss(reinterpret_cast<float*>(z), vResult);
#endif
		}

		// Store the W component into a 32 bit integer location in memory.

		inline void XM_CALLCONV VectorGetIntWPtr(uint32_t* w, FVector V)
		{
			assert(w != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			*w = V.vector4_u32[3];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			vst1q_lane_u32(w, *reinterpret_cast<const uint32x4_t*>(&V), 3);
#elif defined(_XM_SSE4_INTRINSICS_)
			__m128i V1 = _mm_castps_si128(V);
			*w = static_cast<uint32_t>(_mm_extract_epi32(V1, 3));
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
			_mm_store_ss(reinterpret_cast<float*>(w), vResult);
#endif
		}

		//------------------------------------------------------------------------------

		// Set a single indexed floating point component
		inline Vector XM_CALLCONV VectorSetByIndex(FVector V, float f, size_t i)
		{
			assert(i < 4);
			_Analysis_assume_(i < 4);
			VectorF32 U;
			U.v = V;
			U.f[i] = f;
			return U.v;
		}

		//------------------------------------------------------------------------------

		// Sets the X component of a vector to a passed floating point value
		inline Vector XM_CALLCONV VectorSetX(FVector V, float x)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 U = { { {
					x,
					V.vector4_f32[1],
					V.vector4_f32[2],
					V.vector4_f32[3]
				}
 }
			};
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vsetq_lane_f32(x, V, 0);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = _mm_set_ss(x);
			vResult = _mm_move_ss(V, vResult);
			return vResult;
#endif
		}

		// Sets the Y component of a vector to a passed floating point value
		inline Vector XM_CALLCONV VectorSetY(FVector V, float y)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 U = { { {
					V.vector4_f32[0],
					y,
					V.vector4_f32[2],
					V.vector4_f32[3]
				}
 }
			};
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vsetq_lane_f32(y, V, 1);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vResult = _mm_set_ss(y);
			vResult = _mm_insert_ps(V, vResult, 0x10);
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			// Swap y and x
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
			// Convert input to vector
			Vector vTemp = _mm_set_ss(y);
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap y and x again
			vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
			return vResult;
#endif
		}
		// Sets the Z component of a vector to a passed floating point value
		inline Vector XM_CALLCONV VectorSetZ(FVector V, float z)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 U = { { {
					V.vector4_f32[0],
					V.vector4_f32[1],
					z,
					V.vector4_f32[3]
				}
 }
			};
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vsetq_lane_f32(z, V, 2);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vResult = _mm_set_ss(z);
			vResult = _mm_insert_ps(V, vResult, 0x20);
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			// Swap z and x
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
			// Convert input to vector
			Vector vTemp = _mm_set_ss(z);
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap z and x again
			vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
			return vResult;
#endif
		}

		// Sets the W component of a vector to a passed floating point value
		inline Vector XM_CALLCONV VectorSetW(FVector V, float w)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 U = { { {
					V.vector4_f32[0],
					V.vector4_f32[1],
					V.vector4_f32[2],
					w
				}
 } };
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vsetq_lane_f32(w, V, 3);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vResult = _mm_set_ss(w);
			vResult = _mm_insert_ps(V, vResult, 0x30);
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			// Swap w and x
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
			// Convert input to vector
			Vector vTemp = _mm_set_ss(w);
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap w and x again
			vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		// Sets a component of a vector to a floating point value passed by pointer

		inline Vector XM_CALLCONV VectorSetByIndexPtr(FVector V, const float* f, size_t i)
		{
			assert(f != nullptr);
			assert(i < 4);
			_Analysis_assume_(i < 4);
			VectorF32 U;
			U.v = V;
			U.f[i] = *f;
			return U.v;
		}

		//------------------------------------------------------------------------------

		// Sets the X component of a vector to a floating point value passed by pointer

		inline Vector XM_CALLCONV VectorSetXPtr(FVector V, const float* x)
		{
			assert(x != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 U = { { {
					*x,
					V.vector4_f32[1],
					V.vector4_f32[2],
					V.vector4_f32[3]
				} } };
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vld1q_lane_f32(x, V, 0);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = _mm_load_ss(x);
			vResult = _mm_move_ss(V, vResult);
			return vResult;
#endif
		}

		// Sets the Y component of a vector to a floating point value passed by pointer

		inline Vector XM_CALLCONV VectorSetYPtr(FVector V, const float* y)
		{
			assert(y != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 U = { { {
					V.vector4_f32[0],
					*y,
					V.vector4_f32[2],
					V.vector4_f32[3]
				} } };
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vld1q_lane_f32(y, V, 1);
#elif defined(_XM_SSE_INTRINSICS_)
			// Swap y and x
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
			// Convert input to vector
			Vector vTemp = _mm_load_ss(y);
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap y and x again
			vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
			return vResult;
#endif
		}

		// Sets the Z component of a vector to a floating point value passed by pointer

		inline Vector XM_CALLCONV VectorSetZPtr(FVector V, const float* z)
		{
			assert(z != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 U = { { {
					V.vector4_f32[0],
					V.vector4_f32[1],
					*z,
					V.vector4_f32[3]
				} } };
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vld1q_lane_f32(z, V, 2);
#elif defined(_XM_SSE_INTRINSICS_)
			// Swap z and x
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
			// Convert input to vector
			Vector vTemp = _mm_load_ss(z);
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap z and x again
			vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
			return vResult;
#endif
		}

		// Sets the W component of a vector to a floating point value passed by pointer

		inline Vector XM_CALLCONV VectorSetWPtr(FVector V, const float* w)
		{
			assert(w != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 U = { { {
					V.vector4_f32[0],
					V.vector4_f32[1],
					V.vector4_f32[2],
					*w
				} } };
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vld1q_lane_f32(w, V, 3);
#elif defined(_XM_SSE_INTRINSICS_)
			// Swap w and x
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
			// Convert input to vector
			Vector vTemp = _mm_load_ss(w);
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap w and x again
			vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		// Sets a component of a vector to an integer passed by value
		inline Vector XM_CALLCONV VectorSetIntByIndex(FVector V, uint32_t x, size_t i)
		{
			assert(i < 4);
			_Analysis_assume_(i < 4);
			VectorU32 tmp;
			tmp.v = V;
			tmp.u[i] = x;
			return tmp;
		}

		//------------------------------------------------------------------------------

		// Sets the X component of a vector to an integer passed by value
		inline Vector XM_CALLCONV VectorSetIntX(FVector V, uint32_t x)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 U = { { {
					x,
					V.vector4_u32[1],
					V.vector4_u32[2],
					V.vector4_u32[3]
				} } };
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vsetq_lane_u32(x, V, 0);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i vTemp = _mm_cvtsi32_si128(static_cast<int>(x));
			Vector vResult = _mm_move_ss(V, _mm_castsi128_ps(vTemp));
			return vResult;
#endif
		}

		// Sets the Y component of a vector to an integer passed by value
		inline Vector XM_CALLCONV VectorSetIntY(FVector V, uint32_t y)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 U = { { {
					V.vector4_u32[0],
					y,
					V.vector4_u32[2],
					V.vector4_u32[3]
				} } };
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vsetq_lane_u32(y, V, 1);
#elif defined(_XM_SSE4_INTRINSICS_)
			__m128i vResult = _mm_castps_si128(V);
			vResult = _mm_insert_epi32(vResult, static_cast<int>(y), 1);
			return _mm_castsi128_ps(vResult);
#elif defined(_XM_SSE_INTRINSICS_)
			// Swap y and x
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
			// Convert input to vector
			__m128i vTemp = _mm_cvtsi32_si128(static_cast<int>(y));
			// Replace the x component
			vResult = _mm_move_ss(vResult, _mm_castsi128_ps(vTemp));
			// Swap y and x again
			vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
			return vResult;
#endif
		}

		// Sets the Z component of a vector to an integer passed by value
		inline Vector XM_CALLCONV VectorSetIntZ(FVector V, uint32_t z)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 U = { { {
					V.vector4_u32[0],
					V.vector4_u32[1],
					z,
					V.vector4_u32[3]
				} } };
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vsetq_lane_u32(z, V, 2);
#elif defined(_XM_SSE4_INTRINSICS_)
			__m128i vResult = _mm_castps_si128(V);
			vResult = _mm_insert_epi32(vResult, static_cast<int>(z), 2);
			return _mm_castsi128_ps(vResult);
#elif defined(_XM_SSE_INTRINSICS_)
			// Swap z and x
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
			// Convert input to vector
			__m128i vTemp = _mm_cvtsi32_si128(static_cast<int>(z));
			// Replace the x component
			vResult = _mm_move_ss(vResult, _mm_castsi128_ps(vTemp));
			// Swap z and x again
			vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
			return vResult;
#endif
		}

		// Sets the W component of a vector to an integer passed by value
		inline Vector XM_CALLCONV VectorSetIntW(FVector V, uint32_t w)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 U = { { {
					V.vector4_u32[0],
					V.vector4_u32[1],
					V.vector4_u32[2],
					w
				} } };
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vsetq_lane_u32(w, V, 3);
#elif defined(_XM_SSE4_INTRINSICS_)
			__m128i vResult = _mm_castps_si128(V);
			vResult = _mm_insert_epi32(vResult, static_cast<int>(w), 3);
			return _mm_castsi128_ps(vResult);
#elif defined(_XM_SSE_INTRINSICS_)
			// Swap w and x
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
			// Convert input to vector
			__m128i vTemp = _mm_cvtsi32_si128(static_cast<int>(w));
			// Replace the x component
			vResult = _mm_move_ss(vResult, _mm_castsi128_ps(vTemp));
			// Swap w and x again
			vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		// Sets a component of a vector to an integer value passed by pointer

		inline Vector XM_CALLCONV VectorSetIntByIndexPtr(FVector V, const uint32_t* x, size_t i)
		{
			assert(x != nullptr);
			assert(i < 4);
			_Analysis_assume_(i < 4);
			VectorU32 tmp;
			tmp.v = V;
			tmp.u[i] = *x;
			return tmp;
		}

		//------------------------------------------------------------------------------

		// Sets the X component of a vector to an integer value passed by pointer

		inline Vector XM_CALLCONV VectorSetIntXPtr(FVector V, const uint32_t* x)
		{
			assert(x != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 U = { { {
					*x,
					V.vector4_u32[1],
					V.vector4_u32[2],
					V.vector4_u32[3]
				} } };
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vld1q_lane_u32(x, *reinterpret_cast<const uint32x4_t*>(&V), 0);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_load_ss(reinterpret_cast<const float*>(x));
			Vector vResult = _mm_move_ss(V, vTemp);
			return vResult;
#endif
		}

		// Sets the Y component of a vector to an integer value passed by pointer

		inline Vector XM_CALLCONV VectorSetIntYPtr(FVector V, const uint32_t* y)
		{
			assert(y != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 U = { { {
					V.vector4_u32[0],
					*y,
					V.vector4_u32[2],
					V.vector4_u32[3]
				} } };
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vld1q_lane_u32(y, *reinterpret_cast<const uint32x4_t*>(&V), 1);
#elif defined(_XM_SSE_INTRINSICS_)
			// Swap y and x
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
			// Convert input to vector
			Vector vTemp = _mm_load_ss(reinterpret_cast<const float*>(y));
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap y and x again
			vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
			return vResult;
#endif
		}

		// Sets the Z component of a vector to an integer value passed by pointer

		inline Vector XM_CALLCONV VectorSetIntZPtr(FVector V, const uint32_t* z)
		{
			assert(z != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 U = { { {
					V.vector4_u32[0],
					V.vector4_u32[1],
					*z,
					V.vector4_u32[3]
				} } };
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vld1q_lane_u32(z, *reinterpret_cast<const uint32x4_t*>(&V), 2);
#elif defined(_XM_SSE_INTRINSICS_)
			// Swap z and x
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
			// Convert input to vector
			Vector vTemp = _mm_load_ss(reinterpret_cast<const float*>(z));
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap z and x again
			vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
			return vResult;
#endif
		}

		// Sets the W component of a vector to an integer value passed by pointer

		inline Vector XM_CALLCONV VectorSetIntWPtr(FVector V, const uint32_t* w)
		{
			assert(w != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			VectorU32 U = { { {
					V.vector4_u32[0],
					V.vector4_u32[1],
					V.vector4_u32[2],
					*w
				} } };
			return U.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vld1q_lane_u32(w, *reinterpret_cast<const uint32x4_t*>(&V), 3);
#elif defined(_XM_SSE_INTRINSICS_)
			// Swap w and x
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
			// Convert input to vector
			Vector vTemp = _mm_load_ss(reinterpret_cast<const float*>(w));
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap w and x again
			vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorSwizzle
		(
			FVector V,
			uint32_t E0,
			uint32_t E1,
			uint32_t E2,
			uint32_t E3
		)
		{
			assert((E0 < 4) && (E1 < 4) && (E2 < 4) && (E3 < 4));
			_Analysis_assume_((E0 < 4) && (E1 < 4) && (E2 < 4) && (E3 < 4));
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result = { { {
					V.vector4_f32[E0],
					V.vector4_f32[E1],
					V.vector4_f32[E2],
					V.vector4_f32[E3]
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			static const uint32_t ControlElement[4] =
			{
				0x03020100, // XM_SWIZZLE_X
				0x07060504, // XM_SWIZZLE_Y
				0x0B0A0908, // XM_SWIZZLE_Z
				0x0F0E0D0C, // XM_SWIZZLE_W
			};

			int8x8x2_t tbl;
			tbl.val[0] = vget_low_f32(V);
			tbl.val[1] = vget_high_f32(V);

			uint32x2_t idx = vcreate_u32(static_cast<uint64_t>(ControlElement[E0]) | (static_cast<uint64_t>(ControlElement[E1]) << 32));
			const uint8x8_t rL = vtbl2_u8(tbl, idx);

			idx = vcreate_u32(static_cast<uint64_t>(ControlElement[E2]) | (static_cast<uint64_t>(ControlElement[E3]) << 32));
			const uint8x8_t rH = vtbl2_u8(tbl, idx);

			return vcombine_f32(rL, rH);
#elif defined(_XM_AVX_INTRINSICS_)
			unsigned int elem[4] = { E0, E1, E2, E3 };
			__m128i vControl = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&elem[0]));
			return _mm_permutevar_ps(V, vControl);
#else
			auto aPtr = reinterpret_cast<const uint32_t*>(&V);

			Vector Result;
			auto pWork = reinterpret_cast<uint32_t*>(&Result);

			pWork[0] = aPtr[E0];
			pWork[1] = aPtr[E1];
			pWork[2] = aPtr[E2];
			pWork[3] = aPtr[E3];

			return Result;
#endif
		}

		//------------------------------------------------------------------------------
		inline Vector XM_CALLCONV VectorPermute
		(
			FVector V1,
			FVector V2,
			uint32_t PermuteX,
			uint32_t PermuteY,
			uint32_t PermuteZ,
			uint32_t PermuteW
		)
		{
			assert(PermuteX <= 7 && PermuteY <= 7 && PermuteZ <= 7 && PermuteW <= 7);
			_Analysis_assume_(PermuteX <= 7 && PermuteY <= 7 && PermuteZ <= 7 && PermuteW <= 7);

#if defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			static const uint32_t ControlElement[8] =
			{
				0x03020100, // XM_PERMUTE_0X
				0x07060504, // XM_PERMUTE_0Y
				0x0B0A0908, // XM_PERMUTE_0Z
				0x0F0E0D0C, // XM_PERMUTE_0W
				0x13121110, // XM_PERMUTE_1X
				0x17161514, // XM_PERMUTE_1Y
				0x1B1A1918, // XM_PERMUTE_1Z
				0x1F1E1D1C, // XM_PERMUTE_1W
			};

			int8x8x4_t tbl;
			tbl.val[0] = vget_low_f32(V1);
			tbl.val[1] = vget_high_f32(V1);
			tbl.val[2] = vget_low_f32(V2);
			tbl.val[3] = vget_high_f32(V2);

			uint32x2_t idx = vcreate_u32(static_cast<uint64_t>(ControlElement[PermuteX]) | (static_cast<uint64_t>(ControlElement[PermuteY]) << 32));
			const uint8x8_t rL = vtbl4_u8(tbl, idx);

			idx = vcreate_u32(static_cast<uint64_t>(ControlElement[PermuteZ]) | (static_cast<uint64_t>(ControlElement[PermuteW]) << 32));
			const uint8x8_t rH = vtbl4_u8(tbl, idx);

			return vcombine_f32(rL, rH);
#elif defined(_XM_AVX_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			static const VectorU32 three = { { { 3, 3, 3, 3 } } };

			XM_ALIGNED(16) unsigned int elem[4] = { PermuteX, PermuteY, PermuteZ, PermuteW };
			__m128i vControl = _mm_load_si128(reinterpret_cast<const __m128i*>(&elem[0]));

			__m128i vSelect = _mm_cmpgt_epi32(vControl, three);
			vControl = _mm_castps_si128(_mm_and_ps(_mm_castsi128_ps(vControl), three));

			__m128 shuffled1 = _mm_permutevar_ps(V1, vControl);
			__m128 shuffled2 = _mm_permutevar_ps(V2, vControl);

			__m128 masked1 = _mm_andnot_ps(_mm_castsi128_ps(vSelect), shuffled1);
			__m128 masked2 = _mm_and_ps(_mm_castsi128_ps(vSelect), shuffled2);

			return _mm_or_ps(masked1, masked2);
#else

			const uint32_t* aPtr[2];
			aPtr[0] = reinterpret_cast<const uint32_t*>(&V1);
			aPtr[1] = reinterpret_cast<const uint32_t*>(&V2);

			Vector Result;
			auto pWork = reinterpret_cast<uint32_t*>(&Result);

			const uint32_t i0 = PermuteX & 3;
			const uint32_t vi0 = PermuteX >> 2;
			pWork[0] = aPtr[vi0][i0];

			const uint32_t i1 = PermuteY & 3;
			const uint32_t vi1 = PermuteY >> 2;
			pWork[1] = aPtr[vi1][i1];

			const uint32_t i2 = PermuteZ & 3;
			const uint32_t vi2 = PermuteZ >> 2;
			pWork[2] = aPtr[vi2][i2];

			const uint32_t i3 = PermuteW & 3;
			const uint32_t vi3 = PermuteW >> 2;
			pWork[3] = aPtr[vi3][i3];

			return Result;
#endif
		}

		//------------------------------------------------------------------------------
		// Define a control vector to be used in VectorSelect 
		// operations.  The four integers specified in VectorSelectControl
		// serve as indices to select between components in two vectors.
		// The first index controls selection for the first component of 
		// the vectors involved in a select operation, the second index 
		// controls selection for the second component etc.  A value of
		// zero for an index causes the corresponding component from the first 
		// vector to be selected whereas a one causes the component from the
		// second vector to be selected instead.

		inline Vector XM_CALLCONV VectorSelectControl
		(
			uint32_t VectorIndex0,
			uint32_t VectorIndex1,
			uint32_t VectorIndex2,
			uint32_t VectorIndex3
		)
		{
#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			// x=Index0,y=Index1,z=Index2,w=Index3
			__m128i vTemp = _mm_set_epi32(static_cast<int>(VectorIndex3), static_cast<int>(VectorIndex2), static_cast<int>(VectorIndex1), static_cast<int>(VectorIndex0));
			// Any non-zero entries become 0xFFFFFFFF else 0
			vTemp = _mm_cmpgt_epi32(vTemp, g_XMZero);
			return _mm_castsi128_ps(vTemp);
#elif defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			int32x2_t V0 = vcreate_s32(static_cast<uint64_t>(VectorIndex0) | (static_cast<uint64_t>(VectorIndex1) << 32));
			int32x2_t V1 = vcreate_s32(static_cast<uint64_t>(VectorIndex2) | (static_cast<uint64_t>(VectorIndex3) << 32));
			int32x4_t vTemp = vcombine_s32(V0, V1);
			// Any non-zero entries become 0xFFFFFFFF else 0
			return vcgtq_s32(vTemp, g_XMZero);
#else
			Vector    ControlVector;
			const uint32_t  ControlElement[] =
			{
				XM_SELECT_0,
				XM_SELECT_1
			};

			assert(VectorIndex0 < 2);
			assert(VectorIndex1 < 2);
			assert(VectorIndex2 < 2);
			assert(VectorIndex3 < 2);
			_Analysis_assume_(VectorIndex0 < 2);
			_Analysis_assume_(VectorIndex1 < 2);
			_Analysis_assume_(VectorIndex2 < 2);
			_Analysis_assume_(VectorIndex3 < 2);

			ControlVector.vector4_u32[0] = ControlElement[VectorIndex0];
			ControlVector.vector4_u32[1] = ControlElement[VectorIndex1];
			ControlVector.vector4_u32[2] = ControlElement[VectorIndex2];
			ControlVector.vector4_u32[3] = ControlElement[VectorIndex3];

			return ControlVector;

#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorSelect
		(
			FVector V1,
			FVector V2,
			FVector Control
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Result = { { {
					(V1.vector4_u32[0] & ~Control.vector4_u32[0]) | (V2.vector4_u32[0] & Control.vector4_u32[0]),
					(V1.vector4_u32[1] & ~Control.vector4_u32[1]) | (V2.vector4_u32[1] & Control.vector4_u32[1]),
					(V1.vector4_u32[2] & ~Control.vector4_u32[2]) | (V2.vector4_u32[2] & Control.vector4_u32[2]),
					(V1.vector4_u32[3] & ~Control.vector4_u32[3]) | (V2.vector4_u32[3] & Control.vector4_u32[3]),
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vbslq_f32(Control, V2, V1);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp1 = _mm_andnot_ps(Control, V1);
			Vector vTemp2 = _mm_and_ps(V2, Control);
			return _mm_or_ps(vTemp1, vTemp2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorMergeXY
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Result = { { {
					V1.vector4_u32[0],
					V2.vector4_u32[0],
					V1.vector4_u32[1],
					V2.vector4_u32[1],
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vzipq_f32(V1, V2).val[0];
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_unpacklo_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorMergeZW
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Result = { { {
					V1.vector4_u32[2],
					V2.vector4_u32[2],
					V1.vector4_u32[3],
					V2.vector4_u32[3]
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vzipq_f32(V1, V2).val[1];
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_unpackhi_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorShiftLeft(FVector V1, FVector V2, uint32_t Elements)
		{
			assert(Elements < 4);
			_Analysis_assume_(Elements < 4);
			return VectorPermute(V1, V2, Elements, ((Elements)+1), ((Elements)+2), ((Elements)+3));
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorRotateLeft(FVector V, uint32_t Elements)
		{
			assert(Elements < 4);
			_Analysis_assume_(Elements < 4);
			return VectorSwizzle(V, Elements & 3, (Elements + 1) & 3, (Elements + 2) & 3, (Elements + 3) & 3);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorRotateRight(FVector V, uint32_t Elements)
		{
			assert(Elements < 4);
			_Analysis_assume_(Elements < 4);
			return VectorSwizzle(V, (4 - (Elements)) & 3, (5 - (Elements)) & 3, (6 - (Elements)) & 3, (7 - (Elements)) & 3);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorInsert(FVector VD, FVector VS, uint32_t VSLeftRotateElements,
			uint32_t Select0, uint32_t Select1, uint32_t Select2, uint32_t Select3)
		{
			Vector Control = VectorSelectControl(Select0 & 1, Select1 & 1, Select2 & 1, Select3 & 1);
			return VectorSelect(VD, VectorRotateLeft(VS, VSLeftRotateElements), Control);
		}

		//------------------------------------------------------------------------------
		// Comparison operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorEqual
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Control = { { {
					(V1.vector4_f32[0] == V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[1] == V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[2] == V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[3] == V2.vector4_f32[3]) ? 0xFFFFFFFF : 0,
				} } };
			return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vceqq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_cmpeq_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------


		inline Vector XM_CALLCONV VectorEqualR
		(
			uint32_t* pCR,
			FVector V1,
			FVector V2
		)
		{
			assert(pCR != nullptr);
#if defined(_XM_NO_INTRINSICS_)
			uint32_t ux = (V1.vector4_f32[0] == V2.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
			uint32_t uy = (V1.vector4_f32[1] == V2.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
			uint32_t uz = (V1.vector4_f32[2] == V2.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
			uint32_t uw = (V1.vector4_f32[3] == V2.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
			uint32_t CR = 0;
			if (ux & uy & uz & uw)
			{
				// All elements are greater
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!(ux | uy | uz | uw))
			{
				// All elements are not greater
				CR = XM_CRMASK_CR6FALSE;
			}
			*pCR = CR;

			VectorU32 Control = { { { ux, uy, uz, uw } } };
			return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vceqq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				// All elements are equal
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				// All elements are not equal
				CR = XM_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpeq_ps(V1, V2);
			uint32_t CR = 0;
			int iTest = _mm_movemask_ps(vTemp);
			if (iTest == 0xf)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				// All elements are not greater
				CR = XM_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return vTemp;
#endif
		}

		//------------------------------------------------------------------------------
		// Treat the components of the vectors as unsigned integers and
		// compare individual bits between the two.  This is useful for
		// comparing control vectors and result vectors returned from
		// other comparison operations.

		inline Vector XM_CALLCONV VectorEqualInt
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Control = { { {
					(V1.vector4_u32[0] == V2.vector4_u32[0]) ? 0xFFFFFFFF : 0,
					(V1.vector4_u32[1] == V2.vector4_u32[1]) ? 0xFFFFFFFF : 0,
					(V1.vector4_u32[2] == V2.vector4_u32[2]) ? 0xFFFFFFFF : 0,
					(V1.vector4_u32[3] == V2.vector4_u32[3]) ? 0xFFFFFFFF : 0,
				} } };
			return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vceqq_u32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i V = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------


		inline Vector XM_CALLCONV VectorEqualIntR
		(
			uint32_t* pCR,
			FVector V1,
			FVector V2
		)
		{
			assert(pCR != nullptr);
#if defined(_XM_NO_INTRINSICS_)

			Vector Control = VectorEqualInt(V1, V2);

			*pCR = 0;
			if (Vector4EqualInt(Control, VectorTrueInt()))
			{
				// All elements are equal
				*pCR |= XM_CRMASK_CR6TRUE;
			}
			else if (Vector4EqualInt(Control, VectorFalseInt()))
			{
				// All elements are not equal
				*pCR |= XM_CRMASK_CR6FALSE;
			}
			return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vceqq_u32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				// All elements are equal
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				// All elements are not equal
				CR = XM_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i V = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			int iTemp = _mm_movemask_ps(_mm_castsi128_ps(V));
			uint32_t CR = 0;
			if (iTemp == 0x0F)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!iTemp)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorNearEqual
		(
			FVector V1,
			FVector V2,
			FVector Epsilon
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			float fDeltax = V1.vector4_f32[0] - V2.vector4_f32[0];
			float fDeltay = V1.vector4_f32[1] - V2.vector4_f32[1];
			float fDeltaz = V1.vector4_f32[2] - V2.vector4_f32[2];
			float fDeltaw = V1.vector4_f32[3] - V2.vector4_f32[3];

			fDeltax = fabsf(fDeltax);
			fDeltay = fabsf(fDeltay);
			fDeltaz = fabsf(fDeltaz);
			fDeltaw = fabsf(fDeltaw);

			VectorU32 Control = { { {
					(fDeltax <= Epsilon.vector4_f32[0]) ? 0xFFFFFFFFU : 0,
					(fDeltay <= Epsilon.vector4_f32[1]) ? 0xFFFFFFFFU : 0,
					(fDeltaz <= Epsilon.vector4_f32[2]) ? 0xFFFFFFFFU : 0,
					(fDeltaw <= Epsilon.vector4_f32[3]) ? 0xFFFFFFFFU : 0,
				} } };
			return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Vector vDelta = vsubq_f32(V1, V2);
			return vacleq_f32(vDelta, Epsilon);
#elif defined(_XM_SSE_INTRINSICS_)
			// Get the difference
			Vector vDelta = _mm_sub_ps(V1, V2);
			// Get the absolute value of the difference
			Vector vTemp = _mm_setzero_ps();
			vTemp = _mm_sub_ps(vTemp, vDelta);
			vTemp = _mm_max_ps(vTemp, vDelta);
			vTemp = _mm_cmple_ps(vTemp, Epsilon);
			return vTemp;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorNotEqual
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Control = { { {
					(V1.vector4_f32[0] != V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[1] != V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[2] != V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[3] != V2.vector4_f32[3]) ? 0xFFFFFFFF : 0,
				} } };
			return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vmvnq_u32(vceqq_f32(V1, V2));
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_cmpneq_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorNotEqualInt
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Control = { { {
					(V1.vector4_u32[0] != V2.vector4_u32[0]) ? 0xFFFFFFFFU : 0,
					(V1.vector4_u32[1] != V2.vector4_u32[1]) ? 0xFFFFFFFFU : 0,
					(V1.vector4_u32[2] != V2.vector4_u32[2]) ? 0xFFFFFFFFU : 0,
					(V1.vector4_u32[3] != V2.vector4_u32[3]) ? 0xFFFFFFFFU : 0
				} } };
			return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vmvnq_u32(vceqq_u32(V1, V2));
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i V = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return _mm_xor_ps(_mm_castsi128_ps(V), g_XMNegOneMask);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorGreater
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Control = { { {
					(V1.vector4_f32[0] > V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[1] > V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[2] > V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[3] > V2.vector4_f32[3]) ? 0xFFFFFFFF : 0
				} } };
			return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vcgtq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_cmpgt_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------


		inline Vector XM_CALLCONV VectorGreaterR
		(
			uint32_t* pCR,
			FVector V1,
			FVector V2
		)
		{
			assert(pCR != nullptr);
#if defined(_XM_NO_INTRINSICS_)

			uint32_t ux = (V1.vector4_f32[0] > V2.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
			uint32_t uy = (V1.vector4_f32[1] > V2.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
			uint32_t uz = (V1.vector4_f32[2] > V2.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
			uint32_t uw = (V1.vector4_f32[3] > V2.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
			uint32_t CR = 0;
			if (ux & uy & uz & uw)
			{
				// All elements are greater
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!(ux | uy | uz | uw))
			{
				// All elements are not greater
				CR = XM_CRMASK_CR6FALSE;
			}
			*pCR = CR;

			VectorU32 Control = { { { ux, uy, uz, uw } } };
			return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vcgtq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				// All elements are greater
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				// All elements are not greater
				CR = XM_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpgt_ps(V1, V2);
			uint32_t CR = 0;
			int iTest = _mm_movemask_ps(vTemp);
			if (iTest == 0xf)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				// All elements are not greater
				CR = XM_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return vTemp;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorGreaterOrEqual
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Control = { { {
					(V1.vector4_f32[0] >= V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[1] >= V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[2] >= V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[3] >= V2.vector4_f32[3]) ? 0xFFFFFFFF : 0
				} } };
			return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vcgeq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_cmpge_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------


		inline Vector XM_CALLCONV VectorGreaterOrEqualR
		(
			uint32_t* pCR,
			FVector V1,
			FVector V2
		)
		{
			assert(pCR != nullptr);
#if defined(_XM_NO_INTRINSICS_)

			uint32_t ux = (V1.vector4_f32[0] >= V2.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
			uint32_t uy = (V1.vector4_f32[1] >= V2.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
			uint32_t uz = (V1.vector4_f32[2] >= V2.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
			uint32_t uw = (V1.vector4_f32[3] >= V2.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
			uint32_t CR = 0;
			if (ux & uy & uz & uw)
			{
				// All elements are greater
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!(ux | uy | uz | uw))
			{
				// All elements are not greater
				CR = XM_CRMASK_CR6FALSE;
			}
			*pCR = CR;

			VectorU32 Control = { { { ux, uy, uz, uw } } };
			return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vcgeq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				// All elements are greater or equal
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				// All elements are not greater or equal
				CR = XM_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpge_ps(V1, V2);
			uint32_t CR = 0;
			int iTest = _mm_movemask_ps(vTemp);
			if (iTest == 0xf)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				// All elements are not greater
				CR = XM_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return vTemp;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorLess
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Control = { { {
					(V1.vector4_f32[0] < V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[1] < V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[2] < V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[3] < V2.vector4_f32[3]) ? 0xFFFFFFFF : 0
				} } };
			return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vcltq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_cmplt_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorLessOrEqual
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Control = { { {
					(V1.vector4_f32[0] <= V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[1] <= V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[2] <= V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[3] <= V2.vector4_f32[3]) ? 0xFFFFFFFF : 0
				} } };
			return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vcleq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_cmple_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorInBounds
		(
			FVector V,
			FVector Bounds
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Control = { { {
					(V.vector4_f32[0] <= Bounds.vector4_f32[0] && V.vector4_f32[0] >= -Bounds.vector4_f32[0]) ? 0xFFFFFFFF : 0,
					(V.vector4_f32[1] <= Bounds.vector4_f32[1] && V.vector4_f32[1] >= -Bounds.vector4_f32[1]) ? 0xFFFFFFFF : 0,
					(V.vector4_f32[2] <= Bounds.vector4_f32[2] && V.vector4_f32[2] >= -Bounds.vector4_f32[2]) ? 0xFFFFFFFF : 0,
					(V.vector4_f32[3] <= Bounds.vector4_f32[3] && V.vector4_f32[3] >= -Bounds.vector4_f32[3]) ? 0xFFFFFFFF : 0
				} } };
			return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Test if less than or equal
			Vector vTemp1 = vcleq_f32(V, Bounds);
			// Negate the bounds
			Vector vTemp2 = vnegq_f32(Bounds);
			// Test if greater or equal (Reversed)
			vTemp2 = vcleq_f32(vTemp2, V);
			// Blend answers
			vTemp1 = vandq_u32(vTemp1, vTemp2);
			return vTemp1;
#elif defined(_XM_SSE_INTRINSICS_)
			// Test if less than or equal
			Vector vTemp1 = _mm_cmple_ps(V, Bounds);
			// Negate the bounds
			Vector vTemp2 = _mm_mul_ps(Bounds, g_XMNegativeOne);
			// Test if greater or equal (Reversed)
			vTemp2 = _mm_cmple_ps(vTemp2, V);
			// Blend answers
			vTemp1 = _mm_and_ps(vTemp1, vTemp2);
			return vTemp1;
#endif
		}

		//------------------------------------------------------------------------------


		inline Vector XM_CALLCONV VectorInBoundsR
		(
			uint32_t* pCR,
			FVector V,
			FVector Bounds
		)
		{
			assert(pCR != nullptr);
#if defined(_XM_NO_INTRINSICS_)

			uint32_t ux = (V.vector4_f32[0] <= Bounds.vector4_f32[0] && V.vector4_f32[0] >= -Bounds.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
			uint32_t uy = (V.vector4_f32[1] <= Bounds.vector4_f32[1] && V.vector4_f32[1] >= -Bounds.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
			uint32_t uz = (V.vector4_f32[2] <= Bounds.vector4_f32[2] && V.vector4_f32[2] >= -Bounds.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
			uint32_t uw = (V.vector4_f32[3] <= Bounds.vector4_f32[3] && V.vector4_f32[3] >= -Bounds.vector4_f32[3]) ? 0xFFFFFFFFU : 0;

			uint32_t CR = 0;
			if (ux & uy & uz & uw)
			{
				// All elements are in bounds
				CR = XM_CRMASK_CR6BOUNDS;
			}
			*pCR = CR;

			VectorU32 Control = { { { ux, uy, uz, uw } } };
			return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Test if less than or equal
			Vector vTemp1 = vcleq_f32(V, Bounds);
			// Negate the bounds
			Vector vTemp2 = vnegq_f32(Bounds);
			// Test if greater or equal (Reversed)
			vTemp2 = vcleq_f32(vTemp2, V);
			// Blend answers
			vTemp1 = vandq_u32(vTemp1, vTemp2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTemp1), vget_high_u8(vTemp1));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				// All elements are in bounds
				CR = XM_CRMASK_CR6BOUNDS;
			}
			*pCR = CR;
			return vTemp1;
#elif defined(_XM_SSE_INTRINSICS_)
			// Test if less than or equal
			Vector vTemp1 = _mm_cmple_ps(V, Bounds);
			// Negate the bounds
			Vector vTemp2 = _mm_mul_ps(Bounds, g_XMNegativeOne);
			// Test if greater or equal (Reversed)
			vTemp2 = _mm_cmple_ps(vTemp2, V);
			// Blend answers
			vTemp1 = _mm_and_ps(vTemp1, vTemp2);

			uint32_t CR = 0;
			if (_mm_movemask_ps(vTemp1) == 0xf) {
				// All elements are in bounds
				CR = XM_CRMASK_CR6BOUNDS;
			}
			*pCR = CR;
			return vTemp1;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorIsNaN
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Control = { { {
					XMISNAN(V.vector4_f32[0]) ? 0xFFFFFFFFU : 0,
					XMISNAN(V.vector4_f32[1]) ? 0xFFFFFFFFU : 0,
					XMISNAN(V.vector4_f32[2]) ? 0xFFFFFFFFU : 0,
					XMISNAN(V.vector4_f32[3]) ? 0xFFFFFFFFU : 0
				} } };
			return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Test against itself. NaN is always not equal
			uint32x4_t vTempNan = vceqq_f32(V, V);
			// Flip results
			return vmvnq_u32(vTempNan);
#elif defined(_XM_SSE_INTRINSICS_)
			// Test against itself. NaN is always not equal
			return _mm_cmpneq_ps(V, V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorIsInfinite
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Control = { { {
					XMISINF(V.vector4_f32[0]) ? 0xFFFFFFFFU : 0,
					XMISINF(V.vector4_f32[1]) ? 0xFFFFFFFFU : 0,
					XMISINF(V.vector4_f32[2]) ? 0xFFFFFFFFU : 0,
					XMISINF(V.vector4_f32[3]) ? 0xFFFFFFFFU : 0
				} } };
			return Control.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Mask off the sign bit
			uint32x4_t vTemp = vandq_u32(V, g_XMAbsMask);
			// Compare to infinity
			vTemp = vceqq_f32(vTemp, g_XMInfinity);
			// If any are infinity, the signs are true.
			return vTemp;
#elif defined(_XM_SSE_INTRINSICS_)
			// Mask off the sign bit
			__m128 vTemp = _mm_and_ps(V, g_XMAbsMask);
			// Compare to infinity
			vTemp = _mm_cmpeq_ps(vTemp, g_XMInfinity);
			// If any are infinity, the signs are true.
			return vTemp;
#endif
		}

		//------------------------------------------------------------------------------
		// Rounding and clamping operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorMin
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result = { { {
					(V1.vector4_f32[0] < V2.vector4_f32[0]) ? V1.vector4_f32[0] : V2.vector4_f32[0],
					(V1.vector4_f32[1] < V2.vector4_f32[1]) ? V1.vector4_f32[1] : V2.vector4_f32[1],
					(V1.vector4_f32[2] < V2.vector4_f32[2]) ? V1.vector4_f32[2] : V2.vector4_f32[2],
					(V1.vector4_f32[3] < V2.vector4_f32[3]) ? V1.vector4_f32[3] : V2.vector4_f32[3]
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vminq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_min_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorMax
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result = { { {
					(V1.vector4_f32[0] > V2.vector4_f32[0]) ? V1.vector4_f32[0] : V2.vector4_f32[0],
					(V1.vector4_f32[1] > V2.vector4_f32[1]) ? V1.vector4_f32[1] : V2.vector4_f32[1],
					(V1.vector4_f32[2] > V2.vector4_f32[2]) ? V1.vector4_f32[2] : V2.vector4_f32[2],
					(V1.vector4_f32[3] > V2.vector4_f32[3]) ? V1.vector4_f32[3] : V2.vector4_f32[3]
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vmaxq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_max_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		namespace Internal
		{
			// Round to nearest (even) a.k.a. banker's rounding
			inline float round_to_nearest(float x)
			{
				float i = floorf(x);
				x -= i;
				if (x < 0.5f)
					return i;
				if (x > 0.5f)
					return i + 1.f;

				float int_part;
				(void)modff(i / 2.f, &int_part);
				if ((2.f * int_part) == i)
				{
					return i;
				}

				return i + 1.f;
			}
		}

#if !defined(_XM_NO_INTRINSICS_) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(push)
#pragma float_control(precise, on)
#endif

		inline Vector XM_CALLCONV VectorRound
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result = { { {
					Internal::round_to_nearest(V.vector4_f32[0]),
					Internal::round_to_nearest(V.vector4_f32[1]),
					Internal::round_to_nearest(V.vector4_f32[2]),
					Internal::round_to_nearest(V.vector4_f32[3])
				}
 } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			return vrndnq_f32(V);
#else
			uint32x4_t sign = vandq_u32(V, g_XMNegativeZero);
			uint32x4_t sMagic = vorrq_u32(g_XMNoFraction, sign);
			float32x4_t R1 = vaddq_f32(V, sMagic);
			R1 = vsubq_f32(R1, sMagic);
			float32x4_t R2 = vabsq_f32(V);
			uint32x4_t mask = vcleq_f32(R2, g_XMNoFraction);
			Vector vResult = vbslq_f32(mask, R1, V);
			return vResult;
#endif
#elif defined(_XM_SSE4_INTRINSICS_)
			return _mm_round_ps(V, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 sign = _mm_and_ps(V, g_XMNegativeZero);
			__m128 sMagic = _mm_or_ps(g_XMNoFraction, sign);
			__m128 R1 = _mm_add_ps(V, sMagic);
			R1 = _mm_sub_ps(R1, sMagic);
			__m128 R2 = _mm_and_ps(V, g_XMAbsMask);
			__m128 mask = _mm_cmple_ps(R2, g_XMNoFraction);
			R2 = _mm_andnot_ps(mask, V);
			R1 = _mm_and_ps(R1, mask);
			Vector vResult = _mm_xor_ps(R1, R2);
			return vResult;
#endif
		}

#if !defined(_XM_NO_INTRINSICS_) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(pop)
#endif

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorTruncate
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			Vector Result;
			uint32_t     i;

			// Avoid C4701
			Result.vector4_f32[0] = 0.0f;

			for (i = 0; i < 4; i++)
			{
				if (XMISNAN(V.vector4_f32[i]))
				{
					Result.vector4_u32[i] = 0x7FC00000;
				}
				else if (fabsf(V.vector4_f32[i]) < 8388608.0f)
				{
					Result.vector4_f32[i] = static_cast<float>(static_cast<int32_t>(V.vector4_f32[i]));
				}
				else
				{
					Result.vector4_f32[i] = V.vector4_f32[i];
				}
			}
			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			return vrndq_f32(V);
#else
			float32x4_t vTest = vabsq_f32(V);
			vTest = vcltq_f32(vTest, g_XMNoFraction);

			int32x4_t vInt = vcvtq_s32_f32(V);
			Vector vResult = vcvtq_f32_s32(vInt);

			// All numbers less than 8388608 will use the round to int
			// All others, use the ORIGINAL value
			return vbslq_f32(vTest, vResult, V);
#endif
#elif defined(_XM_SSE4_INTRINSICS_)
			return _mm_round_ps(V, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
#elif defined(_XM_SSE_INTRINSICS_)
			// To handle NAN, INF and numbers greater than 8388608, use masking
			// Get the abs value
			__m128i vTest = _mm_and_si128(_mm_castps_si128(V), g_XMAbsMask);
			// Test for greater than 8388608 (All floats with NO fractionals, NAN and INF
			vTest = _mm_cmplt_epi32(vTest, g_XMNoFraction);
			// Convert to int and back to float for rounding with truncation
			__m128i vInt = _mm_cvttps_epi32(V);
			// Convert back to floats
			Vector vResult = _mm_cvtepi32_ps(vInt);
			// All numbers less than 8388608 will use the round to int
			vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));
			// All others, use the ORIGINAL value
			vTest = _mm_andnot_si128(vTest, _mm_castps_si128(V));
			vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorFloor
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					floorf(V.vector4_f32[0]),
					floorf(V.vector4_f32[1]),
					floorf(V.vector4_f32[2]),
					floorf(V.vector4_f32[3])
				}
 } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			return vrndmq_f32(V);
#else
			float32x4_t vTest = vabsq_f32(V);
			vTest = vcltq_f32(vTest, g_XMNoFraction);
			// Truncate
			int32x4_t vInt = vcvtq_s32_f32(V);
			Vector vResult = vcvtq_f32_s32(vInt);
			Vector vLarger = vcgtq_f32(vResult, V);
			// 0 -> 0, 0xffffffff -> -1.0f
			vLarger = vcvtq_f32_s32(vLarger);
			vResult = vaddq_f32(vResult, vLarger);
			// All numbers less than 8388608 will use the round to int
			// All others, use the ORIGINAL value
			return vbslq_f32(vTest, vResult, V);
#endif
#elif defined(_XM_SSE4_INTRINSICS_)
			return _mm_floor_ps(V);
#elif defined(_XM_SSE_INTRINSICS_)
			// To handle NAN, INF and numbers greater than 8388608, use masking
			__m128i vTest = _mm_and_si128(_mm_castps_si128(V), g_XMAbsMask);
			vTest = _mm_cmplt_epi32(vTest, g_XMNoFraction);
			// Truncate
			__m128i vInt = _mm_cvttps_epi32(V);
			Vector vResult = _mm_cvtepi32_ps(vInt);
			__m128 vLarger = _mm_cmpgt_ps(vResult, V);
			// 0 -> 0, 0xffffffff -> -1.0f
			vLarger = _mm_cvtepi32_ps(_mm_castps_si128(vLarger));
			vResult = _mm_add_ps(vResult, vLarger);
			// All numbers less than 8388608 will use the round to int
			vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));
			// All others, use the ORIGINAL value
			vTest = _mm_andnot_si128(vTest, _mm_castps_si128(V));
			vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorCeiling
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					ceilf(V.vector4_f32[0]),
					ceilf(V.vector4_f32[1]),
					ceilf(V.vector4_f32[2]),
					ceilf(V.vector4_f32[3])
				} }
			};
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			return vrndpq_f32(V);
#else
			float32x4_t vTest = vabsq_f32(V);
			vTest = vcltq_f32(vTest, g_XMNoFraction);
			// Truncate
			int32x4_t vInt = vcvtq_s32_f32(V);
			Vector vResult = vcvtq_f32_s32(vInt);
			Vector vSmaller = vcltq_f32(vResult, V);
			// 0 -> 0, 0xffffffff -> -1.0f
			vSmaller = vcvtq_f32_s32(vSmaller);
			vResult = vsubq_f32(vResult, vSmaller);
			// All numbers less than 8388608 will use the round to int
			// All others, use the ORIGINAL value
			return vbslq_f32(vTest, vResult, V);
#endif
#elif defined(_XM_SSE4_INTRINSICS_)
			return _mm_ceil_ps(V);
#elif defined(_XM_SSE_INTRINSICS_)
			// To handle NAN, INF and numbers greater than 8388608, use masking
			__m128i vTest = _mm_and_si128(_mm_castps_si128(V), g_XMAbsMask);
			vTest = _mm_cmplt_epi32(vTest, g_XMNoFraction);
			// Truncate
			__m128i vInt = _mm_cvttps_epi32(V);
			Vector vResult = _mm_cvtepi32_ps(vInt);
			__m128 vSmaller = _mm_cmplt_ps(vResult, V);
			// 0 -> 0, 0xffffffff -> -1.0f
			vSmaller = _mm_cvtepi32_ps(_mm_castps_si128(vSmaller));
			vResult = _mm_sub_ps(vResult, vSmaller);
			// All numbers less than 8388608 will use the round to int
			vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));
			// All others, use the ORIGINAL value
			vTest = _mm_andnot_si128(vTest, _mm_castps_si128(V));
			vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorClamp
		(
			FVector V,
			FVector Min,
			FVector Max
		)
		{
			assert(Vector4LessOrEqual(Min, Max));

#if defined(_XM_NO_INTRINSICS_)

			Vector Result;
			Result = VectorMax(Min, V);
			Result = VectorMin(Max, Result);
			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Vector vResult;
			vResult = vmaxq_f32(Min, V);
			vResult = vminq_f32(Max, vResult);
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult;
			vResult = _mm_max_ps(Min, V);
			vResult = _mm_min_ps(Max, vResult);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorSaturate
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			const Vector Zero = VectorZero();

			return VectorClamp(V, Zero, g_XMOne.v);

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Set <0 to 0
			Vector vResult = vmaxq_f32(V, vdupq_n_f32(0));
			// Set>1 to 1
			return vminq_f32(vResult, vdupq_n_f32(1.0f));
#elif defined(_XM_SSE_INTRINSICS_)
			// Set <0 to 0
			Vector vResult = _mm_max_ps(V, g_XMZero);
			// Set>1 to 1
			return _mm_min_ps(vResult, g_XMOne);
#endif
		}

		//------------------------------------------------------------------------------
		// Bitwise logical operations
		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorAndInt
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Result = { { {
					V1.vector4_u32[0] & V2.vector4_u32[0],
					V1.vector4_u32[1] & V2.vector4_u32[1],
					V1.vector4_u32[2] & V2.vector4_u32[2],
					V1.vector4_u32[3] & V2.vector4_u32[3]
				} } };
			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vandq_u32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_and_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorAndCInt
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Result = { { {
					V1.vector4_u32[0] & ~V2.vector4_u32[0],
					V1.vector4_u32[1] & ~V2.vector4_u32[1],
					V1.vector4_u32[2] & ~V2.vector4_u32[2],
					V1.vector4_u32[3] & ~V2.vector4_u32[3]
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vbicq_u32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i V = _mm_andnot_si128(_mm_castps_si128(V2), _mm_castps_si128(V1));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorOrInt
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Result = { { {
					V1.vector4_u32[0] | V2.vector4_u32[0],
					V1.vector4_u32[1] | V2.vector4_u32[1],
					V1.vector4_u32[2] | V2.vector4_u32[2],
					V1.vector4_u32[3] | V2.vector4_u32[3]
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vorrq_u32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i V = _mm_or_si128(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorNorInt
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Result = { { {
					~(V1.vector4_u32[0] | V2.vector4_u32[0]),
					~(V1.vector4_u32[1] | V2.vector4_u32[1]),
					~(V1.vector4_u32[2] | V2.vector4_u32[2]),
					~(V1.vector4_u32[3] | V2.vector4_u32[3])
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t Result = vorrq_u32(V1, V2);
			return vbicq_u32(g_XMNegOneMask, Result);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i Result;
			Result = _mm_or_si128(_mm_castps_si128(V1), _mm_castps_si128(V2));
			Result = _mm_andnot_si128(Result, g_XMNegOneMask);
			return _mm_castsi128_ps(Result);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorXorInt
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorU32 Result = { { {
					V1.vector4_u32[0] ^ V2.vector4_u32[0],
					V1.vector4_u32[1] ^ V2.vector4_u32[1],
					V1.vector4_u32[2] ^ V2.vector4_u32[2],
					V1.vector4_u32[3] ^ V2.vector4_u32[3]
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return veorq_u32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i V = _mm_xor_si128(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorNegate
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result = { { {
					-V.vector4_f32[0],
					-V.vector4_f32[1],
					-V.vector4_f32[2],
					-V.vector4_f32[3]
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vnegq_f32(V);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector Z;

			Z = _mm_setzero_ps();

			return _mm_sub_ps(Z, V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorAdd
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result = { { {
					V1.vector4_f32[0] + V2.vector4_f32[0],
					V1.vector4_f32[1] + V2.vector4_f32[1],
					V1.vector4_f32[2] + V2.vector4_f32[2],
					V1.vector4_f32[3] + V2.vector4_f32[3]
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vaddq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_add_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorSum
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result;
			Result.f[0] =
				Result.f[1] =
				Result.f[2] =
				Result.f[3] = V.vector4_f32[0] + V.vector4_f32[1] + V.vector4_f32[2] + V.vector4_f32[3];
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			Vector vTemp = vpaddq_f32(V, V);
			return vpaddq_f32(vTemp, vTemp);
#else
			float32x2_t v1 = vget_low_f32(V);
			float32x2_t v2 = vget_high_f32(V);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			return vcombine_f32(v1, v1);
#endif
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vTemp = _mm_hadd_ps(V, V);
			return _mm_hadd_ps(vTemp, vTemp);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 3, 0, 1));
			Vector vTemp2 = _mm_add_ps(V, vTemp);
			vTemp = XM_PERMUTE_PS(vTemp2, _MM_SHUFFLE(1, 0, 3, 2));
			return _mm_add_ps(vTemp, vTemp2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorAddAngles
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			const Vector Zero = VectorZero();

			// Add the given angles together.  If the range of V1 is such
			// that -Pi <= V1 < Pi and the range of V2 is such that
			// -2Pi <= V2 <= 2Pi, then the range of the resulting angle
			// will be -Pi <= Result < Pi.
			Vector Result = VectorAdd(V1, V2);

			Vector Mask = VectorLess(Result, g_XMNegativePi.v);
			Vector Offset = VectorSelect(Zero, g_XMTwoPi.v, Mask);

			Mask = VectorGreaterOrEqual(Result, g_XMPi.v);
			Offset = VectorSelect(Offset, g_XMNegativeTwoPi.v, Mask);

			Result = VectorAdd(Result, Offset);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Adjust the angles
			Vector vResult = vaddq_f32(V1, V2);
			// Less than Pi?
			uint32x4_t vOffset = vcltq_f32(vResult, g_XMNegativePi);
			vOffset = vandq_u32(vOffset, g_XMTwoPi);
			// Add 2Pi to all entries less than -Pi
			vResult = vaddq_f32(vResult, vOffset);
			// Greater than or equal to Pi?
			vOffset = vcgeq_f32(vResult, g_XMPi);
			vOffset = vandq_u32(vOffset, g_XMTwoPi);
			// Sub 2Pi to all entries greater than Pi
			vResult = vsubq_f32(vResult, vOffset);
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			// Adjust the angles
			Vector vResult = _mm_add_ps(V1, V2);
			// Less than Pi?
			Vector vOffset = _mm_cmplt_ps(vResult, g_XMNegativePi);
			vOffset = _mm_and_ps(vOffset, g_XMTwoPi);
			// Add 2Pi to all entries less than -Pi
			vResult = _mm_add_ps(vResult, vOffset);
			// Greater than or equal to Pi?
			vOffset = _mm_cmpge_ps(vResult, g_XMPi);
			vOffset = _mm_and_ps(vOffset, g_XMTwoPi);
			// Sub 2Pi to all entries greater than Pi
			vResult = _mm_sub_ps(vResult, vOffset);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorSubtract
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result = { { {
					V1.vector4_f32[0] - V2.vector4_f32[0],
					V1.vector4_f32[1] - V2.vector4_f32[1],
					V1.vector4_f32[2] - V2.vector4_f32[2],
					V1.vector4_f32[3] - V2.vector4_f32[3]
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vsubq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_sub_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorSubtractAngles
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			const Vector Zero = VectorZero();

			// Subtract the given angles.  If the range of V1 is such
			// that -Pi <= V1 < Pi and the range of V2 is such that
			// -2Pi <= V2 <= 2Pi, then the range of the resulting angle
			// will be -Pi <= Result < Pi.
			Vector Result = VectorSubtract(V1, V2);

			Vector Mask = VectorLess(Result, g_XMNegativePi.v);
			Vector Offset = VectorSelect(Zero, g_XMTwoPi.v, Mask);

			Mask = VectorGreaterOrEqual(Result, g_XMPi.v);
			Offset = VectorSelect(Offset, g_XMNegativeTwoPi.v, Mask);

			Result = VectorAdd(Result, Offset);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Adjust the angles
			Vector vResult = vsubq_f32(V1, V2);
			// Less than Pi?
			uint32x4_t vOffset = vcltq_f32(vResult, g_XMNegativePi);
			vOffset = vandq_u32(vOffset, g_XMTwoPi);
			// Add 2Pi to all entries less than -Pi
			vResult = vaddq_f32(vResult, vOffset);
			// Greater than or equal to Pi?
			vOffset = vcgeq_f32(vResult, g_XMPi);
			vOffset = vandq_u32(vOffset, g_XMTwoPi);
			// Sub 2Pi to all entries greater than Pi
			vResult = vsubq_f32(vResult, vOffset);
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			// Adjust the angles
			Vector vResult = _mm_sub_ps(V1, V2);
			// Less than Pi?
			Vector vOffset = _mm_cmplt_ps(vResult, g_XMNegativePi);
			vOffset = _mm_and_ps(vOffset, g_XMTwoPi);
			// Add 2Pi to all entries less than -Pi
			vResult = _mm_add_ps(vResult, vOffset);
			// Greater than or equal to Pi?
			vOffset = _mm_cmpge_ps(vResult, g_XMPi);
			vOffset = _mm_and_ps(vOffset, g_XMTwoPi);
			// Sub 2Pi to all entries greater than Pi
			vResult = _mm_sub_ps(vResult, vOffset);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorMultiply
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					V1.vector4_f32[0] * V2.vector4_f32[0],
					V1.vector4_f32[1] * V2.vector4_f32[1],
					V1.vector4_f32[2] * V2.vector4_f32[2],
					V1.vector4_f32[3] * V2.vector4_f32[3]
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vmulq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_mul_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorMultiplyAdd
		(
			FVector V1,
			FVector V2,
			FVector V3
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					V1.vector4_f32[0] * V2.vector4_f32[0] + V3.vector4_f32[0],
					V1.vector4_f32[1] * V2.vector4_f32[1] + V3.vector4_f32[1],
					V1.vector4_f32[2] * V2.vector4_f32[2] + V3.vector4_f32[2],
					V1.vector4_f32[3] * V2.vector4_f32[3] + V3.vector4_f32[3]
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			return vfmaq_f32(V3, V1, V2);
#else
			return vmlaq_f32(V3, V1, V2);
#endif
#elif defined(_XM_FMA3_INTRINSICS_)
			return _mm_fmadd_ps(V1, V2, V3);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = _mm_mul_ps(V1, V2);
			return _mm_add_ps(vResult, V3);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorDivide
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					V1.vector4_f32[0] / V2.vector4_f32[0],
					V1.vector4_f32[1] / V2.vector4_f32[1],
					V1.vector4_f32[2] / V2.vector4_f32[2],
					V1.vector4_f32[3] / V2.vector4_f32[3]
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			return vdivq_f32(V1, V2);
#else
			// 2 iterations of Newton-Raphson refinement of reciprocal
			float32x4_t Reciprocal = vrecpeq_f32(V2);
			float32x4_t S = vrecpsq_f32(Reciprocal, V2);
			Reciprocal = vmulq_f32(S, Reciprocal);
			S = vrecpsq_f32(Reciprocal, V2);
			Reciprocal = vmulq_f32(S, Reciprocal);
			return vmulq_f32(V1, Reciprocal);
#endif
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_div_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorNegativeMultiplySubtract
		(
			FVector V1,
			FVector V2,
			FVector V3
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					V3.vector4_f32[0] - (V1.vector4_f32[0] * V2.vector4_f32[0]),
					V3.vector4_f32[1] - (V1.vector4_f32[1] * V2.vector4_f32[1]),
					V3.vector4_f32[2] - (V1.vector4_f32[2] * V2.vector4_f32[2]),
					V3.vector4_f32[3] - (V1.vector4_f32[3] * V2.vector4_f32[3])
				} } };
			return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			return vfmsq_f32(V3, V1, V2);
#else
			return vmlsq_f32(V3, V1, V2);
#endif
#elif defined(_XM_FMA3_INTRINSICS_)
			return _mm_fnmadd_ps(V1, V2, V3);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector R = _mm_mul_ps(V1, V2);
			return _mm_sub_ps(V3, R);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorScale
		(
			FVector V,
			float    ScaleFactor
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					V.vector4_f32[0] * ScaleFactor,
					V.vector4_f32[1] * ScaleFactor,
					V.vector4_f32[2] * ScaleFactor,
					V.vector4_f32[3] * ScaleFactor
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vmulq_n_f32(V, ScaleFactor);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = _mm_set_ps1(ScaleFactor);
			return _mm_mul_ps(vResult, V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorReciprocalEst
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					1.f / V.vector4_f32[0],
					1.f / V.vector4_f32[1],
					1.f / V.vector4_f32[2],
					1.f / V.vector4_f32[3]
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vrecpeq_f32(V);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_rcp_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorReciprocal
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					1.f / V.vector4_f32[0],
					1.f / V.vector4_f32[1],
					1.f / V.vector4_f32[2],
					1.f / V.vector4_f32[3]
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			float32x4_t one = vdupq_n_f32(1.0f);
			return vdivq_f32(one, V);
#else
			// 2 iterations of Newton-Raphson refinement
			float32x4_t Reciprocal = vrecpeq_f32(V);
			float32x4_t S = vrecpsq_f32(Reciprocal, V);
			Reciprocal = vmulq_f32(S, Reciprocal);
			S = vrecpsq_f32(Reciprocal, V);
			return vmulq_f32(S, Reciprocal);
#endif
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_div_ps(g_XMOne, V);
#endif
		}

		//------------------------------------------------------------------------------
		// Return an estimated square root
		inline Vector XM_CALLCONV VectorSqrtEst
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					sqrtf(V.vector4_f32[0]),
					sqrtf(V.vector4_f32[1]),
					sqrtf(V.vector4_f32[2]),
					sqrtf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// 1 iteration of Newton-Raphson refinment of sqrt
			float32x4_t S0 = vrsqrteq_f32(V);
			float32x4_t P0 = vmulq_f32(V, S0);
			float32x4_t R0 = vrsqrtsq_f32(P0, S0);
			float32x4_t S1 = vmulq_f32(S0, R0);

			Vector VEqualsInfinity = VectorEqualInt(V, g_XMInfinity.v);
			Vector VEqualsZero = VectorEqual(V, vdupq_n_f32(0));
			Vector Result = vmulq_f32(V, S1);
			Vector Select = VectorEqualInt(VEqualsInfinity, VEqualsZero);
			return VectorSelect(V, Result, Select);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_sqrt_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorSqrt
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					sqrtf(V.vector4_f32[0]),
					sqrtf(V.vector4_f32[1]),
					sqrtf(V.vector4_f32[2]),
					sqrtf(V.vector4_f32[3])
				}
 }
			};
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// 3 iterations of Newton-Raphson refinment of sqrt
			float32x4_t S0 = vrsqrteq_f32(V);
			float32x4_t P0 = vmulq_f32(V, S0);
			float32x4_t R0 = vrsqrtsq_f32(P0, S0);
			float32x4_t S1 = vmulq_f32(S0, R0);
			float32x4_t P1 = vmulq_f32(V, S1);
			float32x4_t R1 = vrsqrtsq_f32(P1, S1);
			float32x4_t S2 = vmulq_f32(S1, R1);
			float32x4_t P2 = vmulq_f32(V, S2);
			float32x4_t R2 = vrsqrtsq_f32(P2, S2);
			float32x4_t S3 = vmulq_f32(S2, R2);

			Vector VEqualsInfinity = VectorEqualInt(V, g_XMInfinity.v);
			Vector VEqualsZero = VectorEqual(V, vdupq_n_f32(0));
			Vector Result = vmulq_f32(V, S3);
			Vector Select = VectorEqualInt(VEqualsInfinity, VEqualsZero);
			return VectorSelect(V, Result, Select);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_sqrt_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorReciprocalSqrtEst
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					1.f / sqrtf(V.vector4_f32[0]),
					1.f / sqrtf(V.vector4_f32[1]),
					1.f / sqrtf(V.vector4_f32[2]),
					1.f / sqrtf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vrsqrteq_f32(V);
#elif defined(_XM_SSE_INTRINSICS_)
			return _mm_rsqrt_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorReciprocalSqrt
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					1.f / sqrtf(V.vector4_f32[0]),
					1.f / sqrtf(V.vector4_f32[1]),
					1.f / sqrtf(V.vector4_f32[2]),
					1.f / sqrtf(V.vector4_f32[3])
				} } };
			return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// 2 iterations of Newton-Raphson refinement of reciprocal
			float32x4_t S0 = vrsqrteq_f32(V);

			float32x4_t P0 = vmulq_f32(V, S0);
			float32x4_t R0 = vrsqrtsq_f32(P0, S0);

			float32x4_t S1 = vmulq_f32(S0, R0);
			float32x4_t P1 = vmulq_f32(V, S1);
			float32x4_t R1 = vrsqrtsq_f32(P1, S1);

			return vmulq_f32(S1, R1);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = _mm_sqrt_ps(V);
			vResult = _mm_div_ps(g_XMOne, vResult);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorExp2
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result = { { {
					powf(2.0f, V.vector4_f32[0]),
					powf(2.0f, V.vector4_f32[1]),
					powf(2.0f, V.vector4_f32[2]),
					powf(2.0f, V.vector4_f32[3])
				} } };
			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			int32x4_t itrunc = vcvtq_s32_f32(V);
			float32x4_t ftrunc = vcvtq_f32_s32(itrunc);
			float32x4_t y = vsubq_f32(V, ftrunc);

			float32x4_t poly = vmlaq_f32(g_XMExpEst6, g_XMExpEst7, y);
			poly = vmlaq_f32(g_XMExpEst5, poly, y);
			poly = vmlaq_f32(g_XMExpEst4, poly, y);
			poly = vmlaq_f32(g_XMExpEst3, poly, y);
			poly = vmlaq_f32(g_XMExpEst2, poly, y);
			poly = vmlaq_f32(g_XMExpEst1, poly, y);
			poly = vmlaq_f32(g_XMOne, poly, y);

			int32x4_t biased = vaddq_s32(itrunc, g_XMExponentBias);
			biased = vshlq_n_s32(biased, 23);
			float32x4_t result0 = VectorDivide(biased, poly);

			biased = vaddq_s32(itrunc, g_XM253);
			biased = vshlq_n_s32(biased, 23);
			float32x4_t result1 = VectorDivide(biased, poly);
			result1 = vmulq_f32(g_XMMinNormal.v, result1);

			// Use selection to handle the cases
			//  if (V is NaN) -> QNaN;
			//  else if (V sign bit set)
			//      if (V > -150)
			//         if (V.exponent < -126) -> result1
			//         else -> result0
			//      else -> +0
			//  else
			//      if (V < 128) -> result0
			//      else -> +inf

			int32x4_t comp = vcltq_s32(V, g_XMBin128);
			float32x4_t result2 = vbslq_f32(comp, result0, g_XMInfinity);

			comp = vcltq_s32(itrunc, g_XMSubnormalExponent);
			float32x4_t result3 = vbslq_f32(comp, result1, result0);

			comp = vcltq_s32(V, g_XMBinNeg150);
			float32x4_t result4 = vbslq_f32(comp, result3, g_XMZero);

			int32x4_t sign = vandq_s32(V, g_XMNegativeZero);
			comp = vceqq_s32(sign, g_XMNegativeZero);
			float32x4_t result5 = vbslq_f32(comp, result4, result2);

			int32x4_t t0 = vandq_s32(V, g_XMQNaNTest);
			int32x4_t t1 = vandq_s32(V, g_XMInfinity);
			t0 = vceqq_s32(t0, g_XMZero);
			t1 = vceqq_s32(t1, g_XMInfinity);
			int32x4_t isNaN = vbicq_s32(t1, t0);

			float32x4_t vResult = vbslq_f32(isNaN, g_XMQNaN, result5);
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i itrunc = _mm_cvttps_epi32(V);
			__m128 ftrunc = _mm_cvtepi32_ps(itrunc);
			__m128 y = _mm_sub_ps(V, ftrunc);
			__m128 poly = _mm_mul_ps(g_XMExpEst7, y);
			poly = _mm_add_ps(g_XMExpEst6, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(g_XMExpEst5, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(g_XMExpEst4, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(g_XMExpEst3, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(g_XMExpEst2, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(g_XMExpEst1, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(g_XMOne, poly);

			__m128i biased = _mm_add_epi32(itrunc, g_XMExponentBias);
			biased = _mm_slli_epi32(biased, 23);
			__m128 result0 = _mm_div_ps(_mm_castsi128_ps(biased), poly);

			biased = _mm_add_epi32(itrunc, g_XM253);
			biased = _mm_slli_epi32(biased, 23);
			__m128 result1 = _mm_div_ps(_mm_castsi128_ps(biased), poly);
			result1 = _mm_mul_ps(g_XMMinNormal.v, result1);

			// Use selection to handle the cases
			//  if (V is NaN) -> QNaN;
			//  else if (V sign bit set)
			//      if (V > -150)
			//         if (V.exponent < -126) -> result1
			//         else -> result0
			//      else -> +0
			//  else
			//      if (V < 128) -> result0
			//      else -> +inf

			__m128i comp = _mm_cmplt_epi32(_mm_castps_si128(V), g_XMBin128);
			__m128i select0 = _mm_and_si128(comp, _mm_castps_si128(result0));
			__m128i select1 = _mm_andnot_si128(comp, g_XMInfinity);
			__m128i result2 = _mm_or_si128(select0, select1);

			comp = _mm_cmplt_epi32(itrunc, g_XMSubnormalExponent);
			select1 = _mm_and_si128(comp, _mm_castps_si128(result1));
			select0 = _mm_andnot_si128(comp, _mm_castps_si128(result0));
			__m128i result3 = _mm_or_si128(select0, select1);

			comp = _mm_cmplt_epi32(_mm_castps_si128(V), g_XMBinNeg150);
			select0 = _mm_and_si128(comp, result3);
			select1 = _mm_andnot_si128(comp, g_XMZero);
			__m128i result4 = _mm_or_si128(select0, select1);

			__m128i sign = _mm_and_si128(_mm_castps_si128(V), g_XMNegativeZero);
			comp = _mm_cmpeq_epi32(sign, g_XMNegativeZero);
			select0 = _mm_and_si128(comp, result4);
			select1 = _mm_andnot_si128(comp, result2);
			__m128i result5 = _mm_or_si128(select0, select1);

			__m128i t0 = _mm_and_si128(_mm_castps_si128(V), g_XMQNaNTest);
			__m128i t1 = _mm_and_si128(_mm_castps_si128(V), g_XMInfinity);
			t0 = _mm_cmpeq_epi32(t0, g_XMZero);
			t1 = _mm_cmpeq_epi32(t1, g_XMInfinity);
			__m128i isNaN = _mm_andnot_si128(t0, t1);

			select0 = _mm_and_si128(isNaN, g_XMQNaN);
			select1 = _mm_andnot_si128(isNaN, result5);
			__m128i vResult = _mm_or_si128(select0, select1);

			return _mm_castsi128_ps(vResult);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorExpE
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result = { { {
					expf(V.vector4_f32[0]),
					expf(V.vector4_f32[1]),
					expf(V.vector4_f32[2]),
					expf(V.vector4_f32[3])
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// expE(V) = exp2(vin*log2(e))
			float32x4_t Ve = vmulq_f32(g_XMLgE, V);

			int32x4_t itrunc = vcvtq_s32_f32(Ve);
			float32x4_t ftrunc = vcvtq_f32_s32(itrunc);
			float32x4_t y = vsubq_f32(Ve, ftrunc);


			float32x4_t poly = vmlaq_f32(g_XMExpEst6, g_XMExpEst7, y);
			poly = vmlaq_f32(g_XMExpEst5, poly, y);
			poly = vmlaq_f32(g_XMExpEst4, poly, y);
			poly = vmlaq_f32(g_XMExpEst3, poly, y);
			poly = vmlaq_f32(g_XMExpEst2, poly, y);
			poly = vmlaq_f32(g_XMExpEst1, poly, y);
			poly = vmlaq_f32(g_XMOne, poly, y);

			int32x4_t biased = vaddq_s32(itrunc, g_XMExponentBias);
			biased = vshlq_n_s32(biased, 23);
			float32x4_t result0 = VectorDivide(biased, poly);

			biased = vaddq_s32(itrunc, g_XM253);
			biased = vshlq_n_s32(biased, 23);
			float32x4_t result1 = VectorDivide(biased, poly);
			result1 = vmulq_f32(g_XMMinNormal.v, result1);

			// Use selection to handle the cases
			//  if (V is NaN) -> QNaN;
			//  else if (V sign bit set)
			//      if (V > -150)
			//         if (V.exponent < -126) -> result1
			//         else -> result0
			//      else -> +0
			//  else
			//      if (V < 128) -> result0
			//      else -> +inf

			int32x4_t comp = vcltq_s32(Ve, g_XMBin128);
			float32x4_t result2 = vbslq_f32(comp, result0, g_XMInfinity);

			comp = vcltq_s32(itrunc, g_XMSubnormalExponent);
			float32x4_t result3 = vbslq_f32(comp, result1, result0);

			comp = vcltq_s32(Ve, g_XMBinNeg150);
			float32x4_t result4 = vbslq_f32(comp, result3, g_XMZero);

			int32x4_t sign = vandq_s32(Ve, g_XMNegativeZero);
			comp = vceqq_s32(sign, g_XMNegativeZero);
			float32x4_t result5 = vbslq_f32(comp, result4, result2);

			int32x4_t t0 = vandq_s32(Ve, g_XMQNaNTest);
			int32x4_t t1 = vandq_s32(Ve, g_XMInfinity);
			t0 = vceqq_s32(t0, g_XMZero);
			t1 = vceqq_s32(t1, g_XMInfinity);
			int32x4_t isNaN = vbicq_s32(t1, t0);

			float32x4_t vResult = vbslq_f32(isNaN, g_XMQNaN, result5);
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			// expE(V) = exp2(vin*log2(e))
			__m128 Ve = _mm_mul_ps(g_XMLgE, V);

			__m128i itrunc = _mm_cvttps_epi32(Ve);
			__m128 ftrunc = _mm_cvtepi32_ps(itrunc);
			__m128 y = _mm_sub_ps(Ve, ftrunc);
			__m128 poly = _mm_mul_ps(g_XMExpEst7, y);
			poly = _mm_add_ps(g_XMExpEst6, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(g_XMExpEst5, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(g_XMExpEst4, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(g_XMExpEst3, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(g_XMExpEst2, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(g_XMExpEst1, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(g_XMOne, poly);

			__m128i biased = _mm_add_epi32(itrunc, g_XMExponentBias);
			biased = _mm_slli_epi32(biased, 23);
			__m128 result0 = _mm_div_ps(_mm_castsi128_ps(biased), poly);

			biased = _mm_add_epi32(itrunc, g_XM253);
			biased = _mm_slli_epi32(biased, 23);
			__m128 result1 = _mm_div_ps(_mm_castsi128_ps(biased), poly);
			result1 = _mm_mul_ps(g_XMMinNormal.v, result1);

			// Use selection to handle the cases
			//  if (V is NaN) -> QNaN;
			//  else if (V sign bit set)
			//      if (V > -150)
			//         if (V.exponent < -126) -> result1
			//         else -> result0
			//      else -> +0
			//  else
			//      if (V < 128) -> result0
			//      else -> +inf

			__m128i comp = _mm_cmplt_epi32(_mm_castps_si128(Ve), g_XMBin128);
			__m128i select0 = _mm_and_si128(comp, _mm_castps_si128(result0));
			__m128i select1 = _mm_andnot_si128(comp, g_XMInfinity);
			__m128i result2 = _mm_or_si128(select0, select1);

			comp = _mm_cmplt_epi32(itrunc, g_XMSubnormalExponent);
			select1 = _mm_and_si128(comp, _mm_castps_si128(result1));
			select0 = _mm_andnot_si128(comp, _mm_castps_si128(result0));
			__m128i result3 = _mm_or_si128(select0, select1);

			comp = _mm_cmplt_epi32(_mm_castps_si128(Ve), g_XMBinNeg150);
			select0 = _mm_and_si128(comp, result3);
			select1 = _mm_andnot_si128(comp, g_XMZero);
			__m128i result4 = _mm_or_si128(select0, select1);

			__m128i sign = _mm_and_si128(_mm_castps_si128(Ve), g_XMNegativeZero);
			comp = _mm_cmpeq_epi32(sign, g_XMNegativeZero);
			select0 = _mm_and_si128(comp, result4);
			select1 = _mm_andnot_si128(comp, result2);
			__m128i result5 = _mm_or_si128(select0, select1);

			__m128i t0 = _mm_and_si128(_mm_castps_si128(Ve), g_XMQNaNTest);
			__m128i t1 = _mm_and_si128(_mm_castps_si128(Ve), g_XMInfinity);
			t0 = _mm_cmpeq_epi32(t0, g_XMZero);
			t1 = _mm_cmpeq_epi32(t1, g_XMInfinity);
			__m128i isNaN = _mm_andnot_si128(t0, t1);

			select0 = _mm_and_si128(isNaN, g_XMQNaN);
			select1 = _mm_andnot_si128(isNaN, result5);
			__m128i vResult = _mm_or_si128(select0, select1);

			return _mm_castsi128_ps(vResult);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorExp
		(
			FVector V
		)
		{
			return VectorExp2(V);
		}

		//------------------------------------------------------------------------------

#if defined(_XM_SSE_INTRINSICS_)

		namespace Internal
		{
			inline __m128i multi_sll_epi32(__m128i value, __m128i count)
			{
				__m128i v = _mm_shuffle_epi32(value, _MM_SHUFFLE(0, 0, 0, 0));
				__m128i c = _mm_shuffle_epi32(count, _MM_SHUFFLE(0, 0, 0, 0));
				c = _mm_and_si128(c, g_XMMaskX);
				__m128i r0 = _mm_sll_epi32(v, c);

				v = _mm_shuffle_epi32(value, _MM_SHUFFLE(1, 1, 1, 1));
				c = _mm_shuffle_epi32(count, _MM_SHUFFLE(1, 1, 1, 1));
				c = _mm_and_si128(c, g_XMMaskX);
				__m128i r1 = _mm_sll_epi32(v, c);

				v = _mm_shuffle_epi32(value, _MM_SHUFFLE(2, 2, 2, 2));
				c = _mm_shuffle_epi32(count, _MM_SHUFFLE(2, 2, 2, 2));
				c = _mm_and_si128(c, g_XMMaskX);
				__m128i r2 = _mm_sll_epi32(v, c);

				v = _mm_shuffle_epi32(value, _MM_SHUFFLE(3, 3, 3, 3));
				c = _mm_shuffle_epi32(count, _MM_SHUFFLE(3, 3, 3, 3));
				c = _mm_and_si128(c, g_XMMaskX);
				__m128i r3 = _mm_sll_epi32(v, c);

				// (r0,r0,r1,r1)
				__m128 r01 = _mm_shuffle_ps(_mm_castsi128_ps(r0), _mm_castsi128_ps(r1), _MM_SHUFFLE(0, 0, 0, 0));
				// (r2,r2,r3,r3)
				__m128 r23 = _mm_shuffle_ps(_mm_castsi128_ps(r2), _mm_castsi128_ps(r3), _MM_SHUFFLE(0, 0, 0, 0));
				// (r0,r1,r2,r3)
				__m128 result = _mm_shuffle_ps(r01, r23, _MM_SHUFFLE(2, 0, 2, 0));
				return _mm_castps_si128(result);
			}

			inline __m128i multi_srl_epi32(__m128i value, __m128i count)
			{
				__m128i v = _mm_shuffle_epi32(value, _MM_SHUFFLE(0, 0, 0, 0));
				__m128i c = _mm_shuffle_epi32(count, _MM_SHUFFLE(0, 0, 0, 0));
				c = _mm_and_si128(c, g_XMMaskX);
				__m128i r0 = _mm_srl_epi32(v, c);

				v = _mm_shuffle_epi32(value, _MM_SHUFFLE(1, 1, 1, 1));
				c = _mm_shuffle_epi32(count, _MM_SHUFFLE(1, 1, 1, 1));
				c = _mm_and_si128(c, g_XMMaskX);
				__m128i r1 = _mm_srl_epi32(v, c);

				v = _mm_shuffle_epi32(value, _MM_SHUFFLE(2, 2, 2, 2));
				c = _mm_shuffle_epi32(count, _MM_SHUFFLE(2, 2, 2, 2));
				c = _mm_and_si128(c, g_XMMaskX);
				__m128i r2 = _mm_srl_epi32(v, c);

				v = _mm_shuffle_epi32(value, _MM_SHUFFLE(3, 3, 3, 3));
				c = _mm_shuffle_epi32(count, _MM_SHUFFLE(3, 3, 3, 3));
				c = _mm_and_si128(c, g_XMMaskX);
				__m128i r3 = _mm_srl_epi32(v, c);

				// (r0,r0,r1,r1)
				__m128 r01 = _mm_shuffle_ps(_mm_castsi128_ps(r0), _mm_castsi128_ps(r1), _MM_SHUFFLE(0, 0, 0, 0));
				// (r2,r2,r3,r3)
				__m128 r23 = _mm_shuffle_ps(_mm_castsi128_ps(r2), _mm_castsi128_ps(r3), _MM_SHUFFLE(0, 0, 0, 0));
				// (r0,r1,r2,r3)
				__m128 result = _mm_shuffle_ps(r01, r23, _MM_SHUFFLE(2, 0, 2, 0));
				return _mm_castps_si128(result);
			}

			inline __m128i GetLeadingBit(const __m128i value)
			{
				static const VectorI32 g_XM0000FFFF = { { { 0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF } } };
				static const VectorI32 g_XM000000FF = { { { 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF } } };
				static const VectorI32 g_XM0000000F = { { { 0x0000000F, 0x0000000F, 0x0000000F, 0x0000000F } } };
				static const VectorI32 g_XM00000003 = { { { 0x00000003, 0x00000003, 0x00000003, 0x00000003 } } };

				__m128i v = value, r, c, b, s;

				c = _mm_cmpgt_epi32(v, g_XM0000FFFF);   // c = (v > 0xFFFF)
				b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
				r = _mm_slli_epi32(b, 4);               // r = (b << 4)
				v = multi_srl_epi32(v, r);              // v = (v >> r)

				c = _mm_cmpgt_epi32(v, g_XM000000FF);   // c = (v > 0xFF)
				b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
				s = _mm_slli_epi32(b, 3);               // s = (b << 3)
				v = multi_srl_epi32(v, s);              // v = (v >> s)
				r = _mm_or_si128(r, s);                 // r = (r | s)

				c = _mm_cmpgt_epi32(v, g_XM0000000F);   // c = (v > 0xF)
				b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
				s = _mm_slli_epi32(b, 2);               // s = (b << 2)
				v = multi_srl_epi32(v, s);              // v = (v >> s)
				r = _mm_or_si128(r, s);                 // r = (r | s)

				c = _mm_cmpgt_epi32(v, g_XM00000003);   // c = (v > 0x3)
				b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
				s = _mm_slli_epi32(b, 1);               // s = (b << 1)
				v = multi_srl_epi32(v, s);              // v = (v >> s)
				r = _mm_or_si128(r, s);                 // r = (r | s)

				s = _mm_srli_epi32(v, 1);
				r = _mm_or_si128(r, s);
				return r;
			}
		} // namespace Internal

#endif // _XM_SSE_INTRINSICS_

#if defined(_XM_ARM_NEON_INTRINSICS_)

		namespace Internal
		{
			inline int32x4_t GetLeadingBit(const int32x4_t value)
			{
				static const VectorI32 g_XM0000FFFF = { { { 0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF } } };
				static const VectorI32 g_XM000000FF = { { { 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF } } };
				static const VectorI32 g_XM0000000F = { { { 0x0000000F, 0x0000000F, 0x0000000F, 0x0000000F } } };
				static const VectorI32 g_XM00000003 = { { { 0x00000003, 0x00000003, 0x00000003, 0x00000003 } } };

				int32x4_t v = value, r, c, b, s;

				c = vcgtq_s32(v, g_XM0000FFFF);     // c = (v > 0xFFFF)
				b = vshrq_n_u32(c, 31);             // b = (c ? 1 : 0)
				r = vshlq_n_s32(b, 4);              // r = (b << 4)
				r = vnegq_s32(r);
				v = vshlq_u32(v, r);              // v = (v >> r)

				c = vcgtq_s32(v, g_XM000000FF);     // c = (v > 0xFF)
				b = vshrq_n_u32(c, 31);             // b = (c ? 1 : 0)
				s = vshlq_n_s32(b, 3);              // s = (b << 3)
				s = vnegq_s32(s);
				v = vshlq_u32(v, s);                // v = (v >> s)
				r = vorrq_s32(r, s);                // r = (r | s)

				c = vcgtq_s32(v, g_XM0000000F);     // c = (v > 0xF)
				b = vshrq_n_u32(c, 31);             // b = (c ? 1 : 0)
				s = vshlq_n_s32(b, 2);              // s = (b << 2)
				s = vnegq_s32(s);
				v = vshlq_u32(v, s);                // v = (v >> s)
				r = vorrq_s32(r, s);                // r = (r | s)

				c = vcgtq_s32(v, g_XM00000003);     // c = (v > 0x3)
				b = vshrq_n_u32(c, 31);             // b = (c ? 1 : 0)
				s = vshlq_n_s32(b, 1);              // s = (b << 1)
				s = vnegq_s32(s);
				v = vshlq_u32(v, s);                // v = (v >> s)
				r = vorrq_s32(r, s);                // r = (r | s)

				s = vshrq_n_u32(v, 1);
				r = vorrq_s32(r, s);
				return r;
			}

		} // namespace Internal

#endif

//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorLog2
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			const float fScale = 1.4426950f; // (1.0f / logf(2.0f));

			VectorF32 Result = { { {
					logf(V.vector4_f32[0]) * fScale,
					logf(V.vector4_f32[1]) * fScale,
					logf(V.vector4_f32[2]) * fScale,
					logf(V.vector4_f32[3]) * fScale
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			int32x4_t rawBiased = vandq_s32(V, g_XMInfinity);
			int32x4_t trailing = vandq_s32(V, g_XMQNaNTest);
			int32x4_t isExponentZero = vceqq_s32(g_XMZero, rawBiased);

			// Compute exponent and significand for normals.
			int32x4_t biased = vshrq_n_u32(rawBiased, 23);
			int32x4_t exponentNor = vsubq_s32(biased, g_XMExponentBias);
			int32x4_t trailingNor = trailing;

			// Compute exponent and significand for subnormals.
			int32x4_t leading = Internal::GetLeadingBit(trailing);
			int32x4_t shift = vsubq_s32(g_XMNumTrailing, leading);
			int32x4_t exponentSub = vsubq_s32(g_XMSubnormalExponent, shift);
			int32x4_t trailingSub = vshlq_u32(trailing, shift);
			trailingSub = vandq_s32(trailingSub, g_XMQNaNTest);
			int32x4_t e = vbslq_f32(isExponentZero, exponentSub, exponentNor);
			int32x4_t t = vbslq_f32(isExponentZero, trailingSub, trailingNor);

			// Compute the approximation.
			int32x4_t tmp = vorrq_s32(g_XMOne, t);
			float32x4_t y = vsubq_f32(tmp, g_XMOne);

			float32x4_t log2 = vmlaq_f32(g_XMLogEst6, g_XMLogEst7, y);
			log2 = vmlaq_f32(g_XMLogEst5, log2, y);
			log2 = vmlaq_f32(g_XMLogEst4, log2, y);
			log2 = vmlaq_f32(g_XMLogEst3, log2, y);
			log2 = vmlaq_f32(g_XMLogEst2, log2, y);
			log2 = vmlaq_f32(g_XMLogEst1, log2, y);
			log2 = vmlaq_f32(g_XMLogEst0, log2, y);
			log2 = vmlaq_f32(vcvtq_f32_s32(e), log2, y);

			//  if (x is NaN) -> QNaN
			//  else if (V is positive)
			//      if (V is infinite) -> +inf
			//      else -> log2(V)
			//  else
			//      if (V is zero) -> -inf
			//      else -> -QNaN

			int32x4_t isInfinite = vandq_s32((V), g_XMAbsMask);
			isInfinite = vceqq_s32(isInfinite, g_XMInfinity);

			int32x4_t isGreaterZero = vcgtq_s32((V), g_XMZero);
			int32x4_t isNotFinite = vcgtq_s32((V), g_XMInfinity);
			int32x4_t isPositive = vbicq_s32(isGreaterZero, isNotFinite);

			int32x4_t isZero = vandq_s32((V), g_XMAbsMask);
			isZero = vceqq_s32(isZero, g_XMZero);

			int32x4_t t0 = vandq_s32((V), g_XMQNaNTest);
			int32x4_t t1 = vandq_s32((V), g_XMInfinity);
			t0 = vceqq_s32(t0, g_XMZero);
			t1 = vceqq_s32(t1, g_XMInfinity);
			int32x4_t isNaN = vbicq_s32(t1, t0);

			float32x4_t result = vbslq_f32(isInfinite, g_XMInfinity, log2);
			tmp = vbslq_f32(isZero, g_XMNegInfinity, g_XMNegQNaN);
			result = vbslq_f32(isPositive, result, tmp);
			result = vbslq_f32(isNaN, g_XMQNaN, result);
			return result;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i rawBiased = _mm_and_si128(_mm_castps_si128(V), g_XMInfinity);
			__m128i trailing = _mm_and_si128(_mm_castps_si128(V), g_XMQNaNTest);
			__m128i isExponentZero = _mm_cmpeq_epi32(g_XMZero, rawBiased);

			// Compute exponent and significand for normals.
			__m128i biased = _mm_srli_epi32(rawBiased, 23);
			__m128i exponentNor = _mm_sub_epi32(biased, g_XMExponentBias);
			__m128i trailingNor = trailing;

			// Compute exponent and significand for subnormals.
			__m128i leading = Internal::GetLeadingBit(trailing);
			__m128i shift = _mm_sub_epi32(g_XMNumTrailing, leading);
			__m128i exponentSub = _mm_sub_epi32(g_XMSubnormalExponent, shift);
			__m128i trailingSub = Internal::multi_sll_epi32(trailing, shift);
			trailingSub = _mm_and_si128(trailingSub, g_XMQNaNTest);

			__m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
			__m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
			__m128i e = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isExponentZero, trailingSub);
			select1 = _mm_andnot_si128(isExponentZero, trailingNor);
			__m128i t = _mm_or_si128(select0, select1);

			// Compute the approximation.
			__m128i tmp = _mm_or_si128(g_XMOne, t);
			__m128 y = _mm_sub_ps(_mm_castsi128_ps(tmp), g_XMOne);

			__m128 log2 = _mm_mul_ps(g_XMLogEst7, y);
			log2 = _mm_add_ps(g_XMLogEst6, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(g_XMLogEst5, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(g_XMLogEst4, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(g_XMLogEst3, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(g_XMLogEst2, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(g_XMLogEst1, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(g_XMLogEst0, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(log2, _mm_cvtepi32_ps(e));

			//  if (x is NaN) -> QNaN
			//  else if (V is positive)
			//      if (V is infinite) -> +inf
			//      else -> log2(V)
			//  else
			//      if (V is zero) -> -inf
			//      else -> -QNaN

			__m128i isInfinite = _mm_and_si128(_mm_castps_si128(V), g_XMAbsMask);
			isInfinite = _mm_cmpeq_epi32(isInfinite, g_XMInfinity);

			__m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(V), g_XMZero);
			__m128i isNotFinite = _mm_cmpgt_epi32(_mm_castps_si128(V), g_XMInfinity);
			__m128i isPositive = _mm_andnot_si128(isNotFinite, isGreaterZero);

			__m128i isZero = _mm_and_si128(_mm_castps_si128(V), g_XMAbsMask);
			isZero = _mm_cmpeq_epi32(isZero, g_XMZero);

			__m128i t0 = _mm_and_si128(_mm_castps_si128(V), g_XMQNaNTest);
			__m128i t1 = _mm_and_si128(_mm_castps_si128(V), g_XMInfinity);
			t0 = _mm_cmpeq_epi32(t0, g_XMZero);
			t1 = _mm_cmpeq_epi32(t1, g_XMInfinity);
			__m128i isNaN = _mm_andnot_si128(t0, t1);

			select0 = _mm_and_si128(isInfinite, g_XMInfinity);
			select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
			__m128i result = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isZero, g_XMNegInfinity);
			select1 = _mm_andnot_si128(isZero, g_XMNegQNaN);
			tmp = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isPositive, result);
			select1 = _mm_andnot_si128(isPositive, tmp);
			result = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isNaN, g_XMQNaN);
			select1 = _mm_andnot_si128(isNaN, result);
			result = _mm_or_si128(select0, select1);

			return _mm_castsi128_ps(result);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorLogE
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result = { { {
					logf(V.vector4_f32[0]),
					logf(V.vector4_f32[1]),
					logf(V.vector4_f32[2]),
					logf(V.vector4_f32[3])
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			int32x4_t rawBiased = vandq_s32(V, g_XMInfinity);
			int32x4_t trailing = vandq_s32(V, g_XMQNaNTest);
			int32x4_t isExponentZero = vceqq_s32(g_XMZero, rawBiased);

			// Compute exponent and significand for normals.
			int32x4_t biased = vshrq_n_u32(rawBiased, 23);
			int32x4_t exponentNor = vsubq_s32(biased, g_XMExponentBias);
			int32x4_t trailingNor = trailing;

			// Compute exponent and significand for subnormals.
			int32x4_t leading = Internal::GetLeadingBit(trailing);
			int32x4_t shift = vsubq_s32(g_XMNumTrailing, leading);
			int32x4_t exponentSub = vsubq_s32(g_XMSubnormalExponent, shift);
			int32x4_t trailingSub = vshlq_u32(trailing, shift);
			trailingSub = vandq_s32(trailingSub, g_XMQNaNTest);
			int32x4_t e = vbslq_f32(isExponentZero, exponentSub, exponentNor);
			int32x4_t t = vbslq_f32(isExponentZero, trailingSub, trailingNor);

			// Compute the approximation.
			int32x4_t tmp = vorrq_s32(g_XMOne, t);
			float32x4_t y = vsubq_f32(tmp, g_XMOne);

			float32x4_t log2 = vmlaq_f32(g_XMLogEst6, g_XMLogEst7, y);
			log2 = vmlaq_f32(g_XMLogEst5, log2, y);
			log2 = vmlaq_f32(g_XMLogEst4, log2, y);
			log2 = vmlaq_f32(g_XMLogEst3, log2, y);
			log2 = vmlaq_f32(g_XMLogEst2, log2, y);
			log2 = vmlaq_f32(g_XMLogEst1, log2, y);
			log2 = vmlaq_f32(g_XMLogEst0, log2, y);
			log2 = vmlaq_f32(vcvtq_f32_s32(e), log2, y);

			log2 = vmulq_f32(g_XMInvLgE, log2);

			//  if (x is NaN) -> QNaN
			//  else if (V is positive)
			//      if (V is infinite) -> +inf
			//      else -> log2(V)
			//  else
			//      if (V is zero) -> -inf
			//      else -> -QNaN

			int32x4_t isInfinite = vandq_s32((V), g_XMAbsMask);
			isInfinite = vceqq_s32(isInfinite, g_XMInfinity);

			int32x4_t isGreaterZero = vcgtq_s32((V), g_XMZero);
			int32x4_t isNotFinite = vcgtq_s32((V), g_XMInfinity);
			int32x4_t isPositive = vbicq_s32(isGreaterZero, isNotFinite);

			int32x4_t isZero = vandq_s32((V), g_XMAbsMask);
			isZero = vceqq_s32(isZero, g_XMZero);

			int32x4_t t0 = vandq_s32((V), g_XMQNaNTest);
			int32x4_t t1 = vandq_s32((V), g_XMInfinity);
			t0 = vceqq_s32(t0, g_XMZero);
			t1 = vceqq_s32(t1, g_XMInfinity);
			int32x4_t isNaN = vbicq_s32(t1, t0);

			float32x4_t result = vbslq_f32(isInfinite, g_XMInfinity, log2);
			tmp = vbslq_f32(isZero, g_XMNegInfinity, g_XMNegQNaN);
			result = vbslq_f32(isPositive, result, tmp);
			result = vbslq_f32(isNaN, g_XMQNaN, result);
			return result;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i rawBiased = _mm_and_si128(_mm_castps_si128(V), g_XMInfinity);
			__m128i trailing = _mm_and_si128(_mm_castps_si128(V), g_XMQNaNTest);
			__m128i isExponentZero = _mm_cmpeq_epi32(g_XMZero, rawBiased);

			// Compute exponent and significand for normals.
			__m128i biased = _mm_srli_epi32(rawBiased, 23);
			__m128i exponentNor = _mm_sub_epi32(biased, g_XMExponentBias);
			__m128i trailingNor = trailing;

			// Compute exponent and significand for subnormals.
			__m128i leading = Internal::GetLeadingBit(trailing);
			__m128i shift = _mm_sub_epi32(g_XMNumTrailing, leading);
			__m128i exponentSub = _mm_sub_epi32(g_XMSubnormalExponent, shift);
			__m128i trailingSub = Internal::multi_sll_epi32(trailing, shift);
			trailingSub = _mm_and_si128(trailingSub, g_XMQNaNTest);

			__m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
			__m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
			__m128i e = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isExponentZero, trailingSub);
			select1 = _mm_andnot_si128(isExponentZero, trailingNor);
			__m128i t = _mm_or_si128(select0, select1);

			// Compute the approximation.
			__m128i tmp = _mm_or_si128(g_XMOne, t);
			__m128 y = _mm_sub_ps(_mm_castsi128_ps(tmp), g_XMOne);

			__m128 log2 = _mm_mul_ps(g_XMLogEst7, y);
			log2 = _mm_add_ps(g_XMLogEst6, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(g_XMLogEst5, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(g_XMLogEst4, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(g_XMLogEst3, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(g_XMLogEst2, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(g_XMLogEst1, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(g_XMLogEst0, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(log2, _mm_cvtepi32_ps(e));

			log2 = _mm_mul_ps(g_XMInvLgE, log2);

			//  if (x is NaN) -> QNaN
			//  else if (V is positive)
			//      if (V is infinite) -> +inf
			//      else -> log2(V)
			//  else
			//      if (V is zero) -> -inf
			//      else -> -QNaN

			__m128i isInfinite = _mm_and_si128(_mm_castps_si128(V), g_XMAbsMask);
			isInfinite = _mm_cmpeq_epi32(isInfinite, g_XMInfinity);

			__m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(V), g_XMZero);
			__m128i isNotFinite = _mm_cmpgt_epi32(_mm_castps_si128(V), g_XMInfinity);
			__m128i isPositive = _mm_andnot_si128(isNotFinite, isGreaterZero);

			__m128i isZero = _mm_and_si128(_mm_castps_si128(V), g_XMAbsMask);
			isZero = _mm_cmpeq_epi32(isZero, g_XMZero);

			__m128i t0 = _mm_and_si128(_mm_castps_si128(V), g_XMQNaNTest);
			__m128i t1 = _mm_and_si128(_mm_castps_si128(V), g_XMInfinity);
			t0 = _mm_cmpeq_epi32(t0, g_XMZero);
			t1 = _mm_cmpeq_epi32(t1, g_XMInfinity);
			__m128i isNaN = _mm_andnot_si128(t0, t1);

			select0 = _mm_and_si128(isInfinite, g_XMInfinity);
			select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
			__m128i result = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isZero, g_XMNegInfinity);
			select1 = _mm_andnot_si128(isZero, g_XMNegQNaN);
			tmp = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isPositive, result);
			select1 = _mm_andnot_si128(isPositive, tmp);
			result = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isNaN, g_XMQNaN);
			select1 = _mm_andnot_si128(isNaN, result);
			result = _mm_or_si128(select0, select1);

			return _mm_castsi128_ps(result);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorLog
		(
			FVector V
		)
		{
			return VectorLog2(V);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorPow
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result = { { {
					powf(V1.vector4_f32[0], V2.vector4_f32[0]),
					powf(V1.vector4_f32[1], V2.vector4_f32[1]),
					powf(V1.vector4_f32[2], V2.vector4_f32[2]),
					powf(V1.vector4_f32[3], V2.vector4_f32[3])
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			VectorF32 vResult = { { {
					powf(vgetq_lane_f32(V1, 0), vgetq_lane_f32(V2, 0)),
					powf(vgetq_lane_f32(V1, 1), vgetq_lane_f32(V2, 1)),
					powf(vgetq_lane_f32(V1, 2), vgetq_lane_f32(V2, 2)),
					powf(vgetq_lane_f32(V1, 3), vgetq_lane_f32(V2, 3))
				} } };
			return vResult.v;
#elif defined(_XM_SSE_INTRINSICS_)
			XM_ALIGNED(16) float a[4];
			XM_ALIGNED(16) float b[4];
			_mm_store_ps(a, V1);
			_mm_store_ps(b, V2);
			Vector vResult = _mm_setr_ps(
				powf(a[0], b[0]),
				powf(a[1], b[1]),
				powf(a[2], b[2]),
				powf(a[3], b[3]));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorAbs
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 vResult = { { {
					fabsf(V.vector4_f32[0]),
					fabsf(V.vector4_f32[1]),
					fabsf(V.vector4_f32[2]),
					fabsf(V.vector4_f32[3])
				} } };
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			return vabsq_f32(V);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = _mm_setzero_ps();
			vResult = _mm_sub_ps(vResult, V);
			vResult = _mm_max_ps(vResult, V);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorMod
		(
			FVector V1,
			FVector V2
		)
		{
			// V1 % V2 = V1 - V2 * truncate(V1 / V2)

#if defined(_XM_NO_INTRINSICS_)

			Vector Quotient = VectorDivide(V1, V2);
			Quotient = VectorTruncate(Quotient);
			Vector Result = VectorNegativeMultiplySubtract(V2, Quotient, V1);
			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Vector vResult = VectorDivide(V1, V2);
			vResult = VectorTruncate(vResult);
			return vmlsq_f32(V1, vResult, V2);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = _mm_div_ps(V1, V2);
			vResult = VectorTruncate(vResult);
			vResult = _mm_mul_ps(vResult, V2);
			vResult = _mm_sub_ps(V1, vResult);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorModAngles
		(
			FVector Angles
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector V;
			Vector Result;

			// Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
			V = VectorMultiply(Angles, g_XMReciprocalTwoPi.v);
			V = VectorRound(V);
			Result = VectorNegativeMultiplySubtract(g_XMTwoPi.v, V, Angles);
			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
			Vector vResult = vmulq_f32(Angles, g_XMReciprocalTwoPi);
			// Use the inline function due to complexity for rounding
			vResult = VectorRound(vResult);
			return vmlsq_f32(Angles, vResult, g_XMTwoPi);
#elif defined(_XM_SSE_INTRINSICS_)
			// Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
			Vector vResult = _mm_mul_ps(Angles, g_XMReciprocalTwoPi);
			// Use the inline function due to complexity for rounding
			vResult = VectorRound(vResult);
			vResult = _mm_mul_ps(vResult, g_XMTwoPi);
			vResult = _mm_sub_ps(Angles, vResult);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorSin
		(
			FVector V
		)
		{
			// 11-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					sinf(V.vector4_f32[0]),
					sinf(V.vector4_f32[1]),
					sinf(V.vector4_f32[2]),
					sinf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Force the value within the bounds of pi
			Vector x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with sin(y) = sin(x).
			uint32x4_t sign = vandq_u32(x, g_XMNegativeZero);
			uint32x4_t c = vorrq_u32(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
			float32x4_t absx = vabsq_f32(x);
			float32x4_t rflx = vsubq_f32(c, x);
			uint32x4_t comp = vcleq_f32(absx, g_XMHalfPi);
			x = vbslq_f32(comp, x, rflx);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation
			const Vector SC1 = g_XMSinCoefficients1;
			const Vector SC0 = g_XMSinCoefficients0;
			Vector vConstants = vdupq_lane_f32(vget_high_f32(SC0), 1);
			Vector Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(SC1), 0);

			vConstants = vdupq_lane_f32(vget_high_f32(SC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(SC0), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(SC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(g_XMOne, Result, x2);
			Result = vmulq_f32(Result, x);
			return Result;
#elif defined(_XM_SSE_INTRINSICS_)
	// Force the value within the bounds of pi
			Vector x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with sin(y) = sin(x).
			__m128 sign = _mm_and_ps(x, g_XMNegativeZero);
			__m128 c = _mm_or_ps(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
			__m128 absx = _mm_andnot_ps(sign, x);  // |x|
			__m128 rflx = _mm_sub_ps(c, x);
			__m128 comp = _mm_cmple_ps(absx, g_XMHalfPi);
			__m128 select0 = _mm_and_ps(comp, x);
			__m128 select1 = _mm_andnot_ps(comp, rflx);
			x = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation
			const Vector SC1 = g_XMSinCoefficients1;
			Vector vConstants = XM_PERMUTE_PS(SC1, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			const Vector SC0 = g_XMSinCoefficients0;
			vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(3, 3, 3, 3));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);
			Result = _mm_add_ps(Result, g_XMOne);
			Result = _mm_mul_ps(Result, x);
			return Result;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorCos
		(
			FVector V
		)
		{
			// 10-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					cosf(V.vector4_f32[0]),
					cosf(V.vector4_f32[1]),
					cosf(V.vector4_f32[2]),
					cosf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Map V to x in [-pi,pi].
			Vector x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
			uint32x4_t sign = vandq_u32(x, g_XMNegativeZero);
			uint32x4_t c = vorrq_u32(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
			float32x4_t absx = vabsq_f32(x);
			float32x4_t rflx = vsubq_f32(c, x);
			uint32x4_t comp = vcleq_f32(absx, g_XMHalfPi);
			x = vbslq_f32(comp, x, rflx);
			sign = vbslq_f32(comp, g_XMOne, g_XMNegativeOne);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation
			const Vector CC1 = g_XMCosCoefficients1;
			const Vector CC0 = g_XMCosCoefficients0;
			Vector vConstants = vdupq_lane_f32(vget_high_f32(CC0), 1);
			Vector Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(CC1), 0);

			vConstants = vdupq_lane_f32(vget_high_f32(CC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(CC0), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(CC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(g_XMOne, Result, x2);
			Result = vmulq_f32(Result, sign);
			return Result;
#elif defined(_XM_SSE_INTRINSICS_)
	// Map V to x in [-pi,pi].
			Vector x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
			Vector sign = _mm_and_ps(x, g_XMNegativeZero);
			__m128 c = _mm_or_ps(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
			__m128 absx = _mm_andnot_ps(sign, x);  // |x|
			__m128 rflx = _mm_sub_ps(c, x);
			__m128 comp = _mm_cmple_ps(absx, g_XMHalfPi);
			__m128 select0 = _mm_and_ps(comp, x);
			__m128 select1 = _mm_andnot_ps(comp, rflx);
			x = _mm_or_ps(select0, select1);
			select0 = _mm_and_ps(comp, g_XMOne);
			select1 = _mm_andnot_ps(comp, g_XMNegativeOne);
			sign = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation
			const Vector CC1 = g_XMCosCoefficients1;
			Vector vConstants = XM_PERMUTE_PS(CC1, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			const Vector CC0 = g_XMCosCoefficients0;
			vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(3, 3, 3, 3));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);
			Result = _mm_add_ps(Result, g_XMOne);
			Result = _mm_mul_ps(Result, sign);
			return Result;
#endif
		}

		//------------------------------------------------------------------------------


		inline void XM_CALLCONV VectorSinCos
		(
			Vector* pSin,
			Vector* pCos,
			FVector V
		)
		{
			assert(pSin != nullptr);
			assert(pCos != nullptr);

			// 11/10-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Sin = { { {
					sinf(V.vector4_f32[0]),
					sinf(V.vector4_f32[1]),
					sinf(V.vector4_f32[2]),
					sinf(V.vector4_f32[3])
				} } };

			VectorF32 Cos = { { {
					cosf(V.vector4_f32[0]),
					cosf(V.vector4_f32[1]),
					cosf(V.vector4_f32[2]),
					cosf(V.vector4_f32[3])
				} } };

			*pSin = Sin.v;
			*pCos = Cos.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Force the value within the bounds of pi
			Vector x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
			uint32x4_t sign = vandq_u32(x, g_XMNegativeZero);
			uint32x4_t c = vorrq_u32(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
			float32x4_t absx = vabsq_f32(x);
			float32x4_t  rflx = vsubq_f32(c, x);
			uint32x4_t comp = vcleq_f32(absx, g_XMHalfPi);
			x = vbslq_f32(comp, x, rflx);
			sign = vbslq_f32(comp, g_XMOne, g_XMNegativeOne);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation for sine
			const Vector SC1 = g_XMSinCoefficients1;
			const Vector SC0 = g_XMSinCoefficients0;
			Vector vConstants = vdupq_lane_f32(vget_high_f32(SC0), 1);
			Vector Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(SC1), 0);

			vConstants = vdupq_lane_f32(vget_high_f32(SC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(SC0), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(SC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(g_XMOne, Result, x2);
			*pSin = vmulq_f32(Result, x);

			// Compute polynomial approximation for cosine
			const Vector CC1 = g_XMCosCoefficients1;
			const Vector CC0 = g_XMCosCoefficients0;
			vConstants = vdupq_lane_f32(vget_high_f32(CC0), 1);
			Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(CC1), 0);

			vConstants = vdupq_lane_f32(vget_high_f32(CC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(CC0), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(CC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(g_XMOne, Result, x2);
			*pCos = vmulq_f32(Result, sign);
#elif defined(_XM_SSE_INTRINSICS_)
	// Force the value within the bounds of pi
			Vector x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with sin(y) = sin(x), cos(y) = sign*cos(x).
			Vector sign = _mm_and_ps(x, g_XMNegativeZero);
			__m128 c = _mm_or_ps(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
			__m128 absx = _mm_andnot_ps(sign, x);  // |x|
			__m128 rflx = _mm_sub_ps(c, x);
			__m128 comp = _mm_cmple_ps(absx, g_XMHalfPi);
			__m128 select0 = _mm_and_ps(comp, x);
			__m128 select1 = _mm_andnot_ps(comp, rflx);
			x = _mm_or_ps(select0, select1);
			select0 = _mm_and_ps(comp, g_XMOne);
			select1 = _mm_andnot_ps(comp, g_XMNegativeOne);
			sign = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation of sine
			const Vector SC1 = g_XMSinCoefficients1;
			Vector vConstants = XM_PERMUTE_PS(SC1, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			const Vector SC0 = g_XMSinCoefficients0;
			vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(3, 3, 3, 3));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);
			Result = _mm_add_ps(Result, g_XMOne);
			Result = _mm_mul_ps(Result, x);
			*pSin = Result;

			// Compute polynomial approximation of cosine
			const Vector CC1 = g_XMCosCoefficients1;
			vConstants = XM_PERMUTE_PS(CC1, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_mul_ps(vConstants, x2);

			const Vector CC0 = g_XMCosCoefficients0;
			vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(3, 3, 3, 3));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);
			Result = _mm_add_ps(Result, g_XMOne);
			Result = _mm_mul_ps(Result, sign);
			*pCos = Result;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorTan
		(
			FVector V
		)
		{
			// Cody and Waite algorithm to compute tangent.

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					tanf(V.vector4_f32[0]),
					tanf(V.vector4_f32[1]),
					tanf(V.vector4_f32[2]),
					tanf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_) 

			static const VectorF32 TanCoefficients0 = { { { 1.0f, -4.667168334e-1f, 2.566383229e-2f, -3.118153191e-4f } } };
			static const VectorF32 TanCoefficients1 = { { { 4.981943399e-7f, -1.333835001e-1f, 3.424887824e-3f, -1.786170734e-5f } } };
			static const VectorF32 TanConstants = { { { 1.570796371f, 6.077100628e-11f, 0.000244140625f, 0.63661977228f /*2 / Pi*/ } } };
			static const VectorU32 Mask = { { { 0x1, 0x1, 0x1, 0x1 } } };

			Vector TwoDivPi = VectorSplatW(TanConstants.v);

			Vector Zero = VectorZero();

			Vector C0 = VectorSplatX(TanConstants.v);
			Vector C1 = VectorSplatY(TanConstants.v);
			Vector Epsilon = VectorSplatZ(TanConstants.v);

			Vector VA = VectorMultiply(V, TwoDivPi);

			VA = VectorRound(VA);

			Vector VC = VectorNegativeMultiplySubtract(VA, C0, V);

			Vector VB = VectorAbs(VA);

			VC = VectorNegativeMultiplySubtract(VA, C1, VC);

#if defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			VB = vcvtq_u32_f32(VB);
#elif defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			reinterpret_cast<__m128i*>(&VB)[0] = _mm_cvttps_epi32(VB);
#else
			for (size_t i = 0; i < 4; i++)
			{
				VB.vector4_u32[i] = static_cast<uint32_t>(VB.vector4_f32[i]);
			}
#endif

			Vector VC2 = VectorMultiply(VC, VC);

			Vector T7 = VectorSplatW(TanCoefficients1.v);
			Vector T6 = VectorSplatZ(TanCoefficients1.v);
			Vector T4 = VectorSplatX(TanCoefficients1.v);
			Vector T3 = VectorSplatW(TanCoefficients0.v);
			Vector T5 = VectorSplatY(TanCoefficients1.v);
			Vector T2 = VectorSplatZ(TanCoefficients0.v);
			Vector T1 = VectorSplatY(TanCoefficients0.v);
			Vector T0 = VectorSplatX(TanCoefficients0.v);

			Vector VBIsEven = VectorAndInt(VB, Mask.v);
			VBIsEven = VectorEqualInt(VBIsEven, Zero);

			Vector N = VectorMultiplyAdd(VC2, T7, T6);
			Vector D = VectorMultiplyAdd(VC2, T4, T3);
			N = VectorMultiplyAdd(VC2, N, T5);
			D = VectorMultiplyAdd(VC2, D, T2);
			N = VectorMultiply(VC2, N);
			D = VectorMultiplyAdd(VC2, D, T1);
			N = VectorMultiplyAdd(VC, N, VC);
			Vector VCNearZero = VectorInBounds(VC, Epsilon);
			D = VectorMultiplyAdd(VC2, D, T0);

			N = VectorSelect(N, VC, VCNearZero);
			D = VectorSelect(D, g_XMOne.v, VCNearZero);

			Vector R0 = VectorNegate(N);
			Vector R1 = VectorDivide(N, D);
			R0 = VectorDivide(D, R0);

			Vector VIsZero = VectorEqual(V, Zero);

			Vector Result = VectorSelect(R0, R1, VBIsEven);

			Result = VectorSelect(Result, Zero, VIsZero);

			return Result;

#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorSinH
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					sinhf(V.vector4_f32[0]),
					sinhf(V.vector4_f32[1]),
					sinhf(V.vector4_f32[2]),
					sinhf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			static const VectorF32 Scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

			Vector V1 = vmlaq_f32(g_XMNegativeOne.v, V, Scale.v);
			Vector V2 = vmlsq_f32(g_XMNegativeOne.v, V, Scale.v);
			Vector E1 = VectorExp(V1);
			Vector E2 = VectorExp(V2);

			return vsubq_f32(E1, E2);
#elif defined(_XM_SSE_INTRINSICS_)
			static const VectorF32 Scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

			Vector V1 = _mm_mul_ps(V, Scale);
			V1 = _mm_add_ps(V1, g_XMNegativeOne);
			Vector V2 = _mm_mul_ps(V, Scale);
			V2 = _mm_sub_ps(g_XMNegativeOne, V2);
			Vector E1 = VectorExp(V1);
			Vector E2 = VectorExp(V2);

			return _mm_sub_ps(E1, E2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorCosH
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					coshf(V.vector4_f32[0]),
					coshf(V.vector4_f32[1]),
					coshf(V.vector4_f32[2]),
					coshf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			static const VectorF32 Scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

			Vector V1 = vmlaq_f32(g_XMNegativeOne.v, V, Scale.v);
			Vector V2 = vmlsq_f32(g_XMNegativeOne.v, V, Scale.v);
			Vector E1 = VectorExp(V1);
			Vector E2 = VectorExp(V2);
			return vaddq_f32(E1, E2);
#elif defined(_XM_SSE_INTRINSICS_)
			static const VectorF32 Scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

			Vector V1 = _mm_mul_ps(V, Scale.v);
			V1 = _mm_add_ps(V1, g_XMNegativeOne.v);
			Vector V2 = _mm_mul_ps(V, Scale.v);
			V2 = _mm_sub_ps(g_XMNegativeOne.v, V2);
			Vector E1 = VectorExp(V1);
			Vector E2 = VectorExp(V2);
			return _mm_add_ps(E1, E2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorTanH
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					tanhf(V.vector4_f32[0]),
					tanhf(V.vector4_f32[1]),
					tanhf(V.vector4_f32[2]),
					tanhf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			static const VectorF32 Scale = { { { 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f } } }; // 2.0f / ln(2.0f)

			Vector E = vmulq_f32(V, Scale.v);
			E = VectorExp(E);
			E = vmlaq_f32(g_XMOneHalf.v, E, g_XMOneHalf.v);
			E = VectorReciprocal(E);
			return vsubq_f32(g_XMOne.v, E);
#elif defined(_XM_SSE_INTRINSICS_)
			static const VectorF32 Scale = { { { 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f } } }; // 2.0f / ln(2.0f)

			Vector E = _mm_mul_ps(V, Scale.v);
			E = VectorExp(E);
			E = _mm_mul_ps(E, g_XMOneHalf.v);
			E = _mm_add_ps(E, g_XMOneHalf.v);
			E = _mm_div_ps(g_XMOne.v, E);
			return _mm_sub_ps(g_XMOne.v, E);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorASin
		(
			FVector V
		)
		{
			// 7-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					asinf(V.vector4_f32[0]),
					asinf(V.vector4_f32[1]),
					asinf(V.vector4_f32[2]),
					asinf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t nonnegative = vcgeq_f32(V, g_XMZero);
			float32x4_t x = vabsq_f32(V);

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			float32x4_t oneMValue = vsubq_f32(g_XMOne, x);
			float32x4_t clampOneMValue = vmaxq_f32(g_XMZero, oneMValue);
			float32x4_t root = VectorSqrt(clampOneMValue);

			// Compute polynomial approximation
			const Vector AC1 = g_XMArcCoefficients1;
			Vector vConstants = vdupq_lane_f32(vget_high_f32(AC1), 0);
			Vector t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AC1), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(AC1), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AC1), 0);
			t0 = vmlaq_f32(vConstants, t0, x);

			const Vector AC0 = g_XMArcCoefficients0;
			vConstants = vdupq_lane_f32(vget_high_f32(AC0), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_high_f32(AC0), 0);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AC0), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AC0), 0);
			t0 = vmlaq_f32(vConstants, t0, x);
			t0 = vmulq_f32(t0, root);

			float32x4_t t1 = vsubq_f32(g_XMPi, t0);
			t0 = vbslq_f32(nonnegative, t0, t1);
			t0 = vsubq_f32(g_XMHalfPi, t0);
			return t0;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 nonnegative = _mm_cmpge_ps(V, g_XMZero);
			__m128 mvalue = _mm_sub_ps(g_XMZero, V);
			__m128 x = _mm_max_ps(V, mvalue);  // |V|

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			__m128 oneMValue = _mm_sub_ps(g_XMOne, x);
			__m128 clampOneMValue = _mm_max_ps(g_XMZero, oneMValue);
			__m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

			// Compute polynomial approximation
			const Vector AC1 = g_XMArcCoefficients1;
			Vector vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 t0 = _mm_mul_ps(vConstants, x);

			vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(2, 2, 2, 2));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(1, 1, 1, 1));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(0, 0, 0, 0));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			const Vector AC0 = g_XMArcCoefficients0;
			vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(3, 3, 3, 3));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(2, 2, 2, 2));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(1, 1, 1, 1));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(0, 0, 0, 0));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, root);

			__m128 t1 = _mm_sub_ps(g_XMPi, t0);
			t0 = _mm_and_ps(nonnegative, t0);
			t1 = _mm_andnot_ps(nonnegative, t1);
			t0 = _mm_or_ps(t0, t1);
			t0 = _mm_sub_ps(g_XMHalfPi, t0);
			return t0;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorACos
		(
			FVector V
		)
		{
			// 7-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					acosf(V.vector4_f32[0]),
					acosf(V.vector4_f32[1]),
					acosf(V.vector4_f32[2]),
					acosf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t nonnegative = vcgeq_f32(V, g_XMZero);
			float32x4_t x = vabsq_f32(V);

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			float32x4_t oneMValue = vsubq_f32(g_XMOne, x);
			float32x4_t clampOneMValue = vmaxq_f32(g_XMZero, oneMValue);
			float32x4_t root = VectorSqrt(clampOneMValue);

			// Compute polynomial approximation
			const Vector AC1 = g_XMArcCoefficients1;
			Vector vConstants = vdupq_lane_f32(vget_high_f32(AC1), 0);
			Vector t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AC1), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(AC1), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AC1), 0);
			t0 = vmlaq_f32(vConstants, t0, x);

			const Vector AC0 = g_XMArcCoefficients0;
			vConstants = vdupq_lane_f32(vget_high_f32(AC0), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_high_f32(AC0), 0);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AC0), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AC0), 0);
			t0 = vmlaq_f32(vConstants, t0, x);
			t0 = vmulq_f32(t0, root);

			float32x4_t t1 = vsubq_f32(g_XMPi, t0);
			t0 = vbslq_f32(nonnegative, t0, t1);
			return t0;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 nonnegative = _mm_cmpge_ps(V, g_XMZero);
			__m128 mvalue = _mm_sub_ps(g_XMZero, V);
			__m128 x = _mm_max_ps(V, mvalue);  // |V|

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			__m128 oneMValue = _mm_sub_ps(g_XMOne, x);
			__m128 clampOneMValue = _mm_max_ps(g_XMZero, oneMValue);
			__m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

			// Compute polynomial approximation
			const Vector AC1 = g_XMArcCoefficients1;
			Vector vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 t0 = _mm_mul_ps(vConstants, x);

			vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(2, 2, 2, 2));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(1, 1, 1, 1));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(0, 0, 0, 0));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			const Vector AC0 = g_XMArcCoefficients0;
			vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(3, 3, 3, 3));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(2, 2, 2, 2));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(1, 1, 1, 1));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(0, 0, 0, 0));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, root);

			__m128 t1 = _mm_sub_ps(g_XMPi, t0);
			t0 = _mm_and_ps(nonnegative, t0);
			t1 = _mm_andnot_ps(nonnegative, t1);
			t0 = _mm_or_ps(t0, t1);
			return t0;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorATan
		(
			FVector V
		)
		{
			// 17-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					atanf(V.vector4_f32[0]),
					atanf(V.vector4_f32[1]),
					atanf(V.vector4_f32[2]),
					atanf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4_t absV = vabsq_f32(V);
			float32x4_t invV = VectorReciprocal(V);
			uint32x4_t comp = vcgtq_f32(V, g_XMOne);
			uint32x4_t sign = vbslq_f32(comp, g_XMOne, g_XMNegativeOne);
			comp = vcleq_f32(absV, g_XMOne);
			sign = vbslq_f32(comp, g_XMZero, sign);
			uint32x4_t x = vbslq_f32(comp, V, invV);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation
			const Vector TC1 = g_XMATanCoefficients1;
			Vector vConstants = vdupq_lane_f32(vget_high_f32(TC1), 0);
			Vector Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(TC1), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(TC1), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(TC1), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			const Vector TC0 = g_XMATanCoefficients0;
			vConstants = vdupq_lane_f32(vget_high_f32(TC0), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_high_f32(TC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(TC0), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(TC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(g_XMOne, Result, x2);
			Result = vmulq_f32(Result, x);

			float32x4_t result1 = vmulq_f32(sign, g_XMHalfPi);
			result1 = vsubq_f32(result1, Result);

			comp = vceqq_f32(sign, g_XMZero);
			Result = vbslq_f32(comp, Result, result1);
			return Result;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 absV = VectorAbs(V);
			__m128 invV = _mm_div_ps(g_XMOne, V);
			__m128 comp = _mm_cmpgt_ps(V, g_XMOne);
			__m128 select0 = _mm_and_ps(comp, g_XMOne);
			__m128 select1 = _mm_andnot_ps(comp, g_XMNegativeOne);
			__m128 sign = _mm_or_ps(select0, select1);
			comp = _mm_cmple_ps(absV, g_XMOne);
			select0 = _mm_and_ps(comp, g_XMZero);
			select1 = _mm_andnot_ps(comp, sign);
			sign = _mm_or_ps(select0, select1);
			select0 = _mm_and_ps(comp, V);
			select1 = _mm_andnot_ps(comp, invV);
			__m128 x = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation
			const Vector TC1 = g_XMATanCoefficients1;
			Vector vConstants = XM_PERMUTE_PS(TC1, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			vConstants = XM_PERMUTE_PS(TC1, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(TC1, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(TC1, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			const Vector TC0 = g_XMATanCoefficients0;
			vConstants = XM_PERMUTE_PS(TC0, _MM_SHUFFLE(3, 3, 3, 3));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(TC0, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(TC0, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(TC0, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);
			Result = _mm_add_ps(Result, g_XMOne);
			Result = _mm_mul_ps(Result, x);
			__m128 result1 = _mm_mul_ps(sign, g_XMHalfPi);
			result1 = _mm_sub_ps(result1, Result);

			comp = _mm_cmpeq_ps(sign, g_XMZero);
			select0 = _mm_and_ps(comp, Result);
			select1 = _mm_andnot_ps(comp, result1);
			Result = _mm_or_ps(select0, select1);
			return Result;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorATan2
		(
			FVector Y,
			FVector X
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					atan2f(Y.vector4_f32[0], X.vector4_f32[0]),
					atan2f(Y.vector4_f32[1], X.vector4_f32[1]),
					atan2f(Y.vector4_f32[2], X.vector4_f32[2]),
					atan2f(Y.vector4_f32[3], X.vector4_f32[3])
				} } };
			return Result.v;
#else

			// Return the inverse tangent of Y / X in the range of -Pi to Pi with the following exceptions:

			//     Y == 0 and X is Negative         -> Pi with the sign of Y
			//     y == 0 and x is positive         -> 0 with the sign of y
			//     Y != 0 and X == 0                -> Pi / 2 with the sign of Y
			//     Y != 0 and X is Negative         -> atan(y/x) + (PI with the sign of Y)
			//     X == -Infinity and Finite Y      -> Pi with the sign of Y
			//     X == +Infinity and Finite Y      -> 0 with the sign of Y
			//     Y == Infinity and X is Finite    -> Pi / 2 with the sign of Y
			//     Y == Infinity and X == -Infinity -> 3Pi / 4 with the sign of Y
			//     Y == Infinity and X == +Infinity -> Pi / 4 with the sign of Y

			static const VectorF32 ATan2Constants = { { { XM_PI, XM_PIDIV2, XM_PIDIV4, XM_PI * 3.0f / 4.0f } } };

			Vector Zero = VectorZero();
			Vector ATanResultValid = VectorTrueInt();

			Vector Pi = VectorSplatX(ATan2Constants);
			Vector PiOverTwo = VectorSplatY(ATan2Constants);
			Vector PiOverFour = VectorSplatZ(ATan2Constants);
			Vector ThreePiOverFour = VectorSplatW(ATan2Constants);

			Vector YEqualsZero = VectorEqual(Y, Zero);
			Vector XEqualsZero = VectorEqual(X, Zero);
			Vector XIsPositive = VectorAndInt(X, g_XMNegativeZero.v);
			XIsPositive = VectorEqualInt(XIsPositive, Zero);
			Vector YEqualsInfinity = VectorIsInfinite(Y);
			Vector XEqualsInfinity = VectorIsInfinite(X);

			Vector YSign = VectorAndInt(Y, g_XMNegativeZero.v);
			Pi = VectorOrInt(Pi, YSign);
			PiOverTwo = VectorOrInt(PiOverTwo, YSign);
			PiOverFour = VectorOrInt(PiOverFour, YSign);
			ThreePiOverFour = VectorOrInt(ThreePiOverFour, YSign);

			Vector R1 = VectorSelect(Pi, YSign, XIsPositive);
			Vector R2 = VectorSelect(ATanResultValid, PiOverTwo, XEqualsZero);
			Vector R3 = VectorSelect(R2, R1, YEqualsZero);
			Vector R4 = VectorSelect(ThreePiOverFour, PiOverFour, XIsPositive);
			Vector R5 = VectorSelect(PiOverTwo, R4, XEqualsInfinity);
			Vector Result = VectorSelect(R3, R5, YEqualsInfinity);
			ATanResultValid = VectorEqualInt(Result, ATanResultValid);

			Vector V = VectorDivide(Y, X);

			Vector R0 = VectorATan(V);

			R1 = VectorSelect(Pi, g_XMNegativeZero, XIsPositive);
			R2 = VectorAdd(R0, R1);

			return VectorSelect(Result, R2, ATanResultValid);

#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorSinEst
		(
			FVector V
		)
		{
			// 7-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					sinf(V.vector4_f32[0]),
					sinf(V.vector4_f32[1]),
					sinf(V.vector4_f32[2]),
					sinf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Force the value within the bounds of pi
			Vector x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with sin(y) = sin(x).
			uint32x4_t sign = vandq_u32(x, g_XMNegativeZero);
			uint32x4_t c = vorrq_u32(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
			float32x4_t absx = vabsq_f32(x);
			float32x4_t rflx = vsubq_f32(c, x);
			uint32x4_t comp = vcleq_f32(absx, g_XMHalfPi);
			x = vbslq_f32(comp, x, rflx);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation
			const Vector SEC = g_XMSinCoefficients1;
			Vector vConstants = vdupq_lane_f32(vget_high_f32(SEC), 0);
			Vector Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(SEC), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(SEC), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(g_XMOne, Result, x2);
			Result = vmulq_f32(Result, x);
			return Result;
#elif defined(_XM_SSE_INTRINSICS_)
	// Force the value within the bounds of pi
			Vector x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with sin(y) = sin(x).
			__m128 sign = _mm_and_ps(x, g_XMNegativeZero);
			__m128 c = _mm_or_ps(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
			__m128 absx = _mm_andnot_ps(sign, x);  // |x|
			__m128 rflx = _mm_sub_ps(c, x);
			__m128 comp = _mm_cmple_ps(absx, g_XMHalfPi);
			__m128 select0 = _mm_and_ps(comp, x);
			__m128 select1 = _mm_andnot_ps(comp, rflx);
			x = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation
			const Vector SEC = g_XMSinCoefficients1;
			Vector vConstants = XM_PERMUTE_PS(SEC, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			vConstants = XM_PERMUTE_PS(SEC, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(SEC, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			Result = _mm_add_ps(Result, g_XMOne);
			Result = _mm_mul_ps(Result, x);
			return Result;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorCosEst
		(
			FVector V
		)
		{
			// 6-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					cosf(V.vector4_f32[0]),
					cosf(V.vector4_f32[1]),
					cosf(V.vector4_f32[2]),
					cosf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Map V to x in [-pi,pi].
			Vector x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
			uint32x4_t sign = vandq_u32(x, g_XMNegativeZero);
			uint32x4_t c = vorrq_u32(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
			float32x4_t absx = vabsq_f32(x);
			float32x4_t rflx = vsubq_f32(c, x);
			uint32x4_t comp = vcleq_f32(absx, g_XMHalfPi);
			x = vbslq_f32(comp, x, rflx);
			sign = vbslq_f32(comp, g_XMOne, g_XMNegativeOne);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation
			const Vector CEC = g_XMCosCoefficients1;
			Vector vConstants = vdupq_lane_f32(vget_high_f32(CEC), 0);
			Vector Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(CEC), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(CEC), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(g_XMOne, Result, x2);
			Result = vmulq_f32(Result, sign);
			return Result;
#elif defined(_XM_SSE_INTRINSICS_)
	// Map V to x in [-pi,pi].
			Vector x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
			Vector sign = _mm_and_ps(x, g_XMNegativeZero);
			__m128 c = _mm_or_ps(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
			__m128 absx = _mm_andnot_ps(sign, x);  // |x|
			__m128 rflx = _mm_sub_ps(c, x);
			__m128 comp = _mm_cmple_ps(absx, g_XMHalfPi);
			__m128 select0 = _mm_and_ps(comp, x);
			__m128 select1 = _mm_andnot_ps(comp, rflx);
			x = _mm_or_ps(select0, select1);
			select0 = _mm_and_ps(comp, g_XMOne);
			select1 = _mm_andnot_ps(comp, g_XMNegativeOne);
			sign = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation
			const Vector CEC = g_XMCosCoefficients1;
			Vector vConstants = XM_PERMUTE_PS(CEC, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			vConstants = XM_PERMUTE_PS(CEC, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(CEC, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			Result = _mm_add_ps(Result, g_XMOne);
			Result = _mm_mul_ps(Result, sign);
			return Result;
#endif
		}

		//------------------------------------------------------------------------------


		inline void XM_CALLCONV VectorSinCosEst
		(
			Vector* pSin,
			Vector* pCos,
			FVector  V
		)
		{
			assert(pSin != nullptr);
			assert(pCos != nullptr);

			// 7/6-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Sin = { { {
					sinf(V.vector4_f32[0]),
					sinf(V.vector4_f32[1]),
					sinf(V.vector4_f32[2]),
					sinf(V.vector4_f32[3])
				} } };

			VectorF32 Cos = { { {
					cosf(V.vector4_f32[0]),
					cosf(V.vector4_f32[1]),
					cosf(V.vector4_f32[2]),
					cosf(V.vector4_f32[3])
				} } };

			*pSin = Sin.v;
			*pCos = Cos.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Force the value within the bounds of pi
			Vector x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
			uint32x4_t sign = vandq_u32(x, g_XMNegativeZero);
			uint32x4_t c = vorrq_u32(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
			float32x4_t absx = vabsq_f32(x);
			float32x4_t rflx = vsubq_f32(c, x);
			uint32x4_t comp = vcleq_f32(absx, g_XMHalfPi);
			x = vbslq_f32(comp, x, rflx);
			sign = vbslq_f32(comp, g_XMOne, g_XMNegativeOne);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation for sine
			const Vector SEC = g_XMSinCoefficients1;
			Vector vConstants = vdupq_lane_f32(vget_high_f32(SEC), 0);
			Vector Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(SEC), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(SEC), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(g_XMOne, Result, x2);
			*pSin = vmulq_f32(Result, x);

			// Compute polynomial approximation
			const Vector CEC = g_XMCosCoefficients1;
			vConstants = vdupq_lane_f32(vget_high_f32(CEC), 0);
			Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(CEC), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(CEC), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(g_XMOne, Result, x2);
			*pCos = vmulq_f32(Result, sign);
#elif defined(_XM_SSE_INTRINSICS_)
	// Force the value within the bounds of pi
			Vector x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with sin(y) = sin(x), cos(y) = sign*cos(x).
			Vector sign = _mm_and_ps(x, g_XMNegativeZero);
			__m128 c = _mm_or_ps(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
			__m128 absx = _mm_andnot_ps(sign, x);  // |x|
			__m128 rflx = _mm_sub_ps(c, x);
			__m128 comp = _mm_cmple_ps(absx, g_XMHalfPi);
			__m128 select0 = _mm_and_ps(comp, x);
			__m128 select1 = _mm_andnot_ps(comp, rflx);
			x = _mm_or_ps(select0, select1);
			select0 = _mm_and_ps(comp, g_XMOne);
			select1 = _mm_andnot_ps(comp, g_XMNegativeOne);
			sign = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation for sine
			const Vector SEC = g_XMSinCoefficients1;
			Vector vConstants = XM_PERMUTE_PS(SEC, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			vConstants = XM_PERMUTE_PS(SEC, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(SEC, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			Result = _mm_add_ps(Result, g_XMOne);
			Result = _mm_mul_ps(Result, x);
			*pSin = Result;

			// Compute polynomial approximation for cosine
			const Vector CEC = g_XMCosCoefficients1;
			vConstants = XM_PERMUTE_PS(CEC, _MM_SHUFFLE(3, 3, 3, 3));
			Result = _mm_mul_ps(vConstants, x2);

			vConstants = XM_PERMUTE_PS(CEC, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(CEC, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			Result = _mm_add_ps(Result, g_XMOne);
			Result = _mm_mul_ps(Result, sign);
			*pCos = Result;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorTanEst
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					tanf(V.vector4_f32[0]),
					tanf(V.vector4_f32[1]),
					tanf(V.vector4_f32[2]),
					tanf(V.vector4_f32[3])
				} } };
			return Result.v;
#else

			Vector OneOverPi = VectorSplatW(g_XMTanEstCoefficients.v);

			Vector V1 = VectorMultiply(V, OneOverPi);
			V1 = VectorRound(V1);

			V1 = VectorNegativeMultiplySubtract(g_XMPi.v, V1, V);

			Vector T0 = VectorSplatX(g_XMTanEstCoefficients.v);
			Vector T1 = VectorSplatY(g_XMTanEstCoefficients.v);
			Vector T2 = VectorSplatZ(g_XMTanEstCoefficients.v);

			Vector V2T2 = VectorNegativeMultiplySubtract(V1, V1, T2);
			Vector V2 = VectorMultiply(V1, V1);
			Vector V1T0 = VectorMultiply(V1, T0);
			Vector V1T1 = VectorMultiply(V1, T1);

			Vector D = VectorReciprocalEst(V2T2);
			Vector N = VectorMultiplyAdd(V2, V1T1, V1T0);

			return VectorMultiply(N, D);

#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorASinEst
		(
			FVector V
		)
		{
			// 3-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result;
			Result.f[0] = asinf(V.vector4_f32[0]);
			Result.f[1] = asinf(V.vector4_f32[1]);
			Result.f[2] = asinf(V.vector4_f32[2]);
			Result.f[3] = asinf(V.vector4_f32[3]);
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t nonnegative = vcgeq_f32(V, g_XMZero);
			float32x4_t x = vabsq_f32(V);

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			float32x4_t oneMValue = vsubq_f32(g_XMOne, x);
			float32x4_t clampOneMValue = vmaxq_f32(g_XMZero, oneMValue);
			float32x4_t root = VectorSqrt(clampOneMValue);

			// Compute polynomial approximation
			const Vector AEC = g_XMArcEstCoefficients;
			Vector vConstants = vdupq_lane_f32(vget_high_f32(AEC), 0);
			Vector t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AEC), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(AEC), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AEC), 0);
			t0 = vmlaq_f32(vConstants, t0, x);
			t0 = vmulq_f32(t0, root);

			float32x4_t t1 = vsubq_f32(g_XMPi, t0);
			t0 = vbslq_f32(nonnegative, t0, t1);
			t0 = vsubq_f32(g_XMHalfPi, t0);
			return t0;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 nonnegative = _mm_cmpge_ps(V, g_XMZero);
			__m128 mvalue = _mm_sub_ps(g_XMZero, V);
			__m128 x = _mm_max_ps(V, mvalue);  // |V|

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			__m128 oneMValue = _mm_sub_ps(g_XMOne, x);
			__m128 clampOneMValue = _mm_max_ps(g_XMZero, oneMValue);
			__m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

			// Compute polynomial approximation
			const Vector AEC = g_XMArcEstCoefficients;
			Vector vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 t0 = _mm_mul_ps(vConstants, x);

			vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(2, 2, 2, 2));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(1, 1, 1, 1));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(0, 0, 0, 0));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, root);

			__m128 t1 = _mm_sub_ps(g_XMPi, t0);
			t0 = _mm_and_ps(nonnegative, t0);
			t1 = _mm_andnot_ps(nonnegative, t1);
			t0 = _mm_or_ps(t0, t1);
			t0 = _mm_sub_ps(g_XMHalfPi, t0);
			return t0;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorACosEst
		(
			FVector V
		)
		{
			// 3-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					acosf(V.vector4_f32[0]),
					acosf(V.vector4_f32[1]),
					acosf(V.vector4_f32[2]),
					acosf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t nonnegative = vcgeq_f32(V, g_XMZero);
			float32x4_t x = vabsq_f32(V);

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			float32x4_t oneMValue = vsubq_f32(g_XMOne, x);
			float32x4_t clampOneMValue = vmaxq_f32(g_XMZero, oneMValue);
			float32x4_t root = VectorSqrt(clampOneMValue);

			// Compute polynomial approximation
			const Vector AEC = g_XMArcEstCoefficients;
			Vector vConstants = vdupq_lane_f32(vget_high_f32(AEC), 0);
			Vector t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AEC), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(AEC), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AEC), 0);
			t0 = vmlaq_f32(vConstants, t0, x);
			t0 = vmulq_f32(t0, root);

			float32x4_t t1 = vsubq_f32(g_XMPi, t0);
			t0 = vbslq_f32(nonnegative, t0, t1);
			return t0;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 nonnegative = _mm_cmpge_ps(V, g_XMZero);
			__m128 mvalue = _mm_sub_ps(g_XMZero, V);
			__m128 x = _mm_max_ps(V, mvalue);  // |V|

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			__m128 oneMValue = _mm_sub_ps(g_XMOne, x);
			__m128 clampOneMValue = _mm_max_ps(g_XMZero, oneMValue);
			__m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

			// Compute polynomial approximation
			const Vector AEC = g_XMArcEstCoefficients;
			Vector vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 t0 = _mm_mul_ps(vConstants, x);

			vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(2, 2, 2, 2));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(1, 1, 1, 1));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(0, 0, 0, 0));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, root);

			__m128 t1 = _mm_sub_ps(g_XMPi, t0);
			t0 = _mm_and_ps(nonnegative, t0);
			t1 = _mm_andnot_ps(nonnegative, t1);
			t0 = _mm_or_ps(t0, t1);
			return t0;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorATanEst
		(
			FVector V
		)
		{
			// 9-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					atanf(V.vector4_f32[0]),
					atanf(V.vector4_f32[1]),
					atanf(V.vector4_f32[2]),
					atanf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4_t absV = vabsq_f32(V);
			float32x4_t invV = VectorReciprocalEst(V);
			uint32x4_t comp = vcgtq_f32(V, g_XMOne);
			uint32x4_t sign = vbslq_f32(comp, g_XMOne, g_XMNegativeOne);
			comp = vcleq_f32(absV, g_XMOne);
			sign = vbslq_f32(comp, g_XMZero, sign);
			uint32x4_t x = vbslq_f32(comp, V, invV);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation
			const Vector AEC = g_XMATanEstCoefficients1;
			Vector vConstants = vdupq_lane_f32(vget_high_f32(AEC), 0);
			Vector Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(AEC), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(AEC), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(AEC), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			// ATanEstCoefficients0 is already splatted
			Result = vmlaq_f32(g_XMATanEstCoefficients0, Result, x2);
			Result = vmulq_f32(Result, x);

			float32x4_t result1 = vmulq_f32(sign, g_XMHalfPi);
			result1 = vsubq_f32(result1, Result);

			comp = vceqq_f32(sign, g_XMZero);
			Result = vbslq_f32(comp, Result, result1);
			return Result;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 absV = VectorAbs(V);
			__m128 invV = _mm_div_ps(g_XMOne, V);
			__m128 comp = _mm_cmpgt_ps(V, g_XMOne);
			__m128 select0 = _mm_and_ps(comp, g_XMOne);
			__m128 select1 = _mm_andnot_ps(comp, g_XMNegativeOne);
			__m128 sign = _mm_or_ps(select0, select1);
			comp = _mm_cmple_ps(absV, g_XMOne);
			select0 = _mm_and_ps(comp, g_XMZero);
			select1 = _mm_andnot_ps(comp, sign);
			sign = _mm_or_ps(select0, select1);
			select0 = _mm_and_ps(comp, V);
			select1 = _mm_andnot_ps(comp, invV);
			__m128 x = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation
			const Vector AEC = g_XMATanEstCoefficients1;
			Vector vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			// ATanEstCoefficients0 is already splatted
			Result = _mm_add_ps(Result, g_XMATanEstCoefficients0);
			Result = _mm_mul_ps(Result, x);
			__m128 result1 = _mm_mul_ps(sign, g_XMHalfPi);
			result1 = _mm_sub_ps(result1, Result);

			comp = _mm_cmpeq_ps(sign, g_XMZero);
			select0 = _mm_and_ps(comp, Result);
			select1 = _mm_andnot_ps(comp, result1);
			Result = _mm_or_ps(select0, select1);
			return Result;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorATan2Est
		(
			FVector Y,
			FVector X
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					atan2f(Y.vector4_f32[0], X.vector4_f32[0]),
					atan2f(Y.vector4_f32[1], X.vector4_f32[1]),
					atan2f(Y.vector4_f32[2], X.vector4_f32[2]),
					atan2f(Y.vector4_f32[3], X.vector4_f32[3]),
				} } };
			return Result.v;
#else

			static const VectorF32 ATan2Constants = { { { XM_PI, XM_PIDIV2, XM_PIDIV4, 2.3561944905f /* Pi*3/4 */ } } };

			const Vector Zero = VectorZero();
			Vector ATanResultValid = VectorTrueInt();

			Vector Pi = VectorSplatX(ATan2Constants);
			Vector PiOverTwo = VectorSplatY(ATan2Constants);
			Vector PiOverFour = VectorSplatZ(ATan2Constants);
			Vector ThreePiOverFour = VectorSplatW(ATan2Constants);

			Vector YEqualsZero = VectorEqual(Y, Zero);
			Vector XEqualsZero = VectorEqual(X, Zero);
			Vector XIsPositive = VectorAndInt(X, g_XMNegativeZero.v);
			XIsPositive = VectorEqualInt(XIsPositive, Zero);
			Vector YEqualsInfinity = VectorIsInfinite(Y);
			Vector XEqualsInfinity = VectorIsInfinite(X);

			Vector YSign = VectorAndInt(Y, g_XMNegativeZero.v);
			Pi = VectorOrInt(Pi, YSign);
			PiOverTwo = VectorOrInt(PiOverTwo, YSign);
			PiOverFour = VectorOrInt(PiOverFour, YSign);
			ThreePiOverFour = VectorOrInt(ThreePiOverFour, YSign);

			Vector R1 = VectorSelect(Pi, YSign, XIsPositive);
			Vector R2 = VectorSelect(ATanResultValid, PiOverTwo, XEqualsZero);
			Vector R3 = VectorSelect(R2, R1, YEqualsZero);
			Vector R4 = VectorSelect(ThreePiOverFour, PiOverFour, XIsPositive);
			Vector R5 = VectorSelect(PiOverTwo, R4, XEqualsInfinity);
			Vector Result = VectorSelect(R3, R5, YEqualsInfinity);
			ATanResultValid = VectorEqualInt(Result, ATanResultValid);

			Vector Reciprocal = VectorReciprocalEst(X);
			Vector V = VectorMultiply(Y, Reciprocal);
			Vector R0 = VectorATanEst(V);

			R1 = VectorSelect(Pi, g_XMNegativeZero, XIsPositive);
			R2 = VectorAdd(R0, R1);

			Result = VectorSelect(Result, R2, ATanResultValid);

			return Result;

#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorLerp
		(
			FVector V0,
			FVector V1,
			float    t
		)
		{
			// V0 + t * (V1 - V0)

#if defined(_XM_NO_INTRINSICS_)

			Vector Scale = VectorReplicate(t);
			Vector Length = VectorSubtract(V1, V0);
			return VectorMultiplyAdd(Length, Scale, V0);

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Vector L = vsubq_f32(V1, V0);
			return vmlaq_n_f32(V0, L, t);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector L = _mm_sub_ps(V1, V0);
			Vector S = _mm_set_ps1(t);
			Vector Result = _mm_mul_ps(L, S);
			return _mm_add_ps(Result, V0);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorLerpV
		(
			FVector V0,
			FVector V1,
			FVector T
		)
		{
			// V0 + T * (V1 - V0)

#if defined(_XM_NO_INTRINSICS_)

			Vector Length = VectorSubtract(V1, V0);
			return VectorMultiplyAdd(Length, T, V0);

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Vector L = vsubq_f32(V1, V0);
			return vmlaq_f32(V0, L, T);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector Length = _mm_sub_ps(V1, V0);
			Vector Result = _mm_mul_ps(Length, T);
			return _mm_add_ps(Result, V0);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorHermite
		(
			FVector Position0,
			FVector Tangent0,
			FVector Position1,
			GVector Tangent1,
			float    t
		)
		{
			// Result = (2 * t^3 - 3 * t^2 + 1) * Position0 +
			//          (t^3 - 2 * t^2 + t) * Tangent0 +
			//          (-2 * t^3 + 3 * t^2) * Position1 +
			//          (t^3 - t^2) * Tangent1

#if defined(_XM_NO_INTRINSICS_)

			float t2 = t * t;
			float t3 = t * t2;

			Vector P0 = VectorReplicate(2.0f * t3 - 3.0f * t2 + 1.0f);
			Vector T0 = VectorReplicate(t3 - 2.0f * t2 + t);
			Vector P1 = VectorReplicate(-2.0f * t3 + 3.0f * t2);
			Vector T1 = VectorReplicate(t3 - t2);

			Vector Result = VectorMultiply(P0, Position0);
			Result = VectorMultiplyAdd(T0, Tangent0, Result);
			Result = VectorMultiplyAdd(P1, Position1, Result);
			Result = VectorMultiplyAdd(T1, Tangent1, Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float t2 = t * t;
			float t3 = t * t2;

			float p0 = 2.0f * t3 - 3.0f * t2 + 1.0f;
			float t0 = t3 - 2.0f * t2 + t;
			float p1 = -2.0f * t3 + 3.0f * t2;
			float t1 = t3 - t2;

			Vector vResult = vmulq_n_f32(Position0, p0);
			vResult = vmlaq_n_f32(vResult, Tangent0, t0);
			vResult = vmlaq_n_f32(vResult, Position1, p1);
			vResult = vmlaq_n_f32(vResult, Tangent1, t1);
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			float t2 = t * t;
			float t3 = t * t2;

			Vector P0 = _mm_set_ps1(2.0f * t3 - 3.0f * t2 + 1.0f);
			Vector T0 = _mm_set_ps1(t3 - 2.0f * t2 + t);
			Vector P1 = _mm_set_ps1(-2.0f * t3 + 3.0f * t2);
			Vector T1 = _mm_set_ps1(t3 - t2);

			Vector vResult = _mm_mul_ps(P0, Position0);
			Vector vTemp = _mm_mul_ps(T0, Tangent0);
			vResult = _mm_add_ps(vResult, vTemp);
			vTemp = _mm_mul_ps(P1, Position1);
			vResult = _mm_add_ps(vResult, vTemp);
			vTemp = _mm_mul_ps(T1, Tangent1);
			vResult = _mm_add_ps(vResult, vTemp);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorHermiteV
		(
			FVector Position0,
			FVector Tangent0,
			FVector Position1,
			GVector Tangent1,
			HVector T
		)
		{
			// Result = (2 * t^3 - 3 * t^2 + 1) * Position0 +
			//          (t^3 - 2 * t^2 + t) * Tangent0 +
			//          (-2 * t^3 + 3 * t^2) * Position1 +
			//          (t^3 - t^2) * Tangent1

#if defined(_XM_NO_INTRINSICS_)

			Vector T2 = VectorMultiply(T, T);
			Vector T3 = VectorMultiply(T, T2);

			Vector P0 = VectorReplicate(2.0f * T3.vector4_f32[0] - 3.0f * T2.vector4_f32[0] + 1.0f);
			Vector T0 = VectorReplicate(T3.vector4_f32[1] - 2.0f * T2.vector4_f32[1] + T.vector4_f32[1]);
			Vector P1 = VectorReplicate(-2.0f * T3.vector4_f32[2] + 3.0f * T2.vector4_f32[2]);
			Vector T1 = VectorReplicate(T3.vector4_f32[3] - T2.vector4_f32[3]);

			Vector Result = VectorMultiply(P0, Position0);
			Result = VectorMultiplyAdd(T0, Tangent0, Result);
			Result = VectorMultiplyAdd(P1, Position1, Result);
			Result = VectorMultiplyAdd(T1, Tangent1, Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			static const VectorF32 CatMulT2 = { { { -3.0f, -2.0f, 3.0f, -1.0f } } };
			static const VectorF32 CatMulT3 = { { { 2.0f, 1.0f, -2.0f, 1.0f } } };

			Vector T2 = vmulq_f32(T, T);
			Vector T3 = vmulq_f32(T, T2);
			// Mul by the constants against t^2
			T2 = vmulq_f32(T2, CatMulT2);
			// Mul by the constants against t^3
			T3 = vmlaq_f32(T2, T3, CatMulT3);
			// T3 now has the pre-result.
			// I need to add t.y only
			T2 = vandq_u32(T, g_XMMaskY);
			T3 = vaddq_f32(T3, T2);
			// Add 1.0f to x
			T3 = vaddq_f32(T3, g_XMIdentityR0);
			// Now, I have the constants created
			// Mul the x constant to Position0
			Vector vResult = vmulq_lane_f32(Position0, vget_low_f32(T3), 0); // T3[0]
			// Mul the y constant to Tangent0
			vResult = vmlaq_lane_f32(vResult, Tangent0, vget_low_f32(T3), 1); // T3[1]
			// Mul the z constant to Position1
			vResult = vmlaq_lane_f32(vResult, Position1, vget_high_f32(T3), 0); // T3[2]
			// Mul the w constant to Tangent1
			vResult = vmlaq_lane_f32(vResult, Tangent1, vget_high_f32(T3), 1); // T3[3]
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			static const VectorF32 CatMulT2 = { { { -3.0f, -2.0f, 3.0f, -1.0f } } };
			static const VectorF32 CatMulT3 = { { { 2.0f, 1.0f, -2.0f, 1.0f } } };

			Vector T2 = _mm_mul_ps(T, T);
			Vector T3 = _mm_mul_ps(T, T2);
			// Mul by the constants against t^2
			T2 = _mm_mul_ps(T2, CatMulT2);
			// Mul by the constants against t^3
			T3 = _mm_mul_ps(T3, CatMulT3);
			// T3 now has the pre-result.
			T3 = _mm_add_ps(T3, T2);
			// I need to add t.y only
			T2 = _mm_and_ps(T, g_XMMaskY);
			T3 = _mm_add_ps(T3, T2);
			// Add 1.0f to x
			T3 = _mm_add_ps(T3, g_XMIdentityR0);
			// Now, I have the constants created
			// Mul the x constant to Position0
			Vector vResult = XM_PERMUTE_PS(T3, _MM_SHUFFLE(0, 0, 0, 0));
			vResult = _mm_mul_ps(vResult, Position0);
			// Mul the y constant to Tangent0
			T2 = XM_PERMUTE_PS(T3, _MM_SHUFFLE(1, 1, 1, 1));
			T2 = _mm_mul_ps(T2, Tangent0);
			vResult = _mm_add_ps(vResult, T2);
			// Mul the z constant to Position1
			T2 = XM_PERMUTE_PS(T3, _MM_SHUFFLE(2, 2, 2, 2));
			T2 = _mm_mul_ps(T2, Position1);
			vResult = _mm_add_ps(vResult, T2);
			// Mul the w constant to Tangent1
			T3 = XM_PERMUTE_PS(T3, _MM_SHUFFLE(3, 3, 3, 3));
			T3 = _mm_mul_ps(T3, Tangent1);
			vResult = _mm_add_ps(vResult, T3);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorCatmullRom
		(
			FVector Position0,
			FVector Position1,
			FVector Position2,
			GVector Position3,
			float    t
		)
		{
			// Result = ((-t^3 + 2 * t^2 - t) * Position0 +
			//           (3 * t^3 - 5 * t^2 + 2) * Position1 +
			//           (-3 * t^3 + 4 * t^2 + t) * Position2 +
			//           (t^3 - t^2) * Position3) * 0.5

#if defined(_XM_NO_INTRINSICS_)

			float t2 = t * t;
			float t3 = t * t2;

			Vector P0 = VectorReplicate((-t3 + 2.0f * t2 - t) * 0.5f);
			Vector P1 = VectorReplicate((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
			Vector P2 = VectorReplicate((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
			Vector P3 = VectorReplicate((t3 - t2) * 0.5f);

			Vector Result = VectorMultiply(P0, Position0);
			Result = VectorMultiplyAdd(P1, Position1, Result);
			Result = VectorMultiplyAdd(P2, Position2, Result);
			Result = VectorMultiplyAdd(P3, Position3, Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float t2 = t * t;
			float t3 = t * t2;

			float p0 = (-t3 + 2.0f * t2 - t) * 0.5f;
			float p1 = (3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f;
			float p2 = (-3.0f * t3 + 4.0f * t2 + t) * 0.5f;
			float p3 = (t3 - t2) * 0.5f;

			Vector P1 = vmulq_n_f32(Position1, p1);
			Vector P0 = vmlaq_n_f32(P1, Position0, p0);
			Vector P3 = vmulq_n_f32(Position3, p3);
			Vector P2 = vmlaq_n_f32(P3, Position2, p2);
			P0 = vaddq_f32(P0, P2);
			return P0;
#elif defined(_XM_SSE_INTRINSICS_)
			float t2 = t * t;
			float t3 = t * t2;

			Vector P0 = _mm_set_ps1((-t3 + 2.0f * t2 - t) * 0.5f);
			Vector P1 = _mm_set_ps1((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
			Vector P2 = _mm_set_ps1((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
			Vector P3 = _mm_set_ps1((t3 - t2) * 0.5f);

			P0 = _mm_mul_ps(P0, Position0);
			P1 = _mm_mul_ps(P1, Position1);
			P2 = _mm_mul_ps(P2, Position2);
			P3 = _mm_mul_ps(P3, Position3);
			P0 = _mm_add_ps(P0, P1);
			P2 = _mm_add_ps(P2, P3);
			P0 = _mm_add_ps(P0, P2);
			return P0;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorCatmullRomV
		(
			FVector Position0,
			FVector Position1,
			FVector Position2,
			GVector Position3,
			HVector T
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			float fx = T.vector4_f32[0];
			float fy = T.vector4_f32[1];
			float fz = T.vector4_f32[2];
			float fw = T.vector4_f32[3];
			VectorF32 vResult = { { {
					0.5f * ((-fx * fx * fx + 2 * fx * fx - fx) * Position0.vector4_f32[0]
					+ (3 * fx * fx * fx - 5 * fx * fx + 2) * Position1.vector4_f32[0]
					+ (-3 * fx * fx * fx + 4 * fx * fx + fx) * Position2.vector4_f32[0]
					+ (fx * fx * fx - fx * fx) * Position3.vector4_f32[0]),

					0.5f * ((-fy * fy * fy + 2 * fy * fy - fy) * Position0.vector4_f32[1]
					+ (3 * fy * fy * fy - 5 * fy * fy + 2) * Position1.vector4_f32[1]
					+ (-3 * fy * fy * fy + 4 * fy * fy + fy) * Position2.vector4_f32[1]
					+ (fy * fy * fy - fy * fy) * Position3.vector4_f32[1]),

					0.5f * ((-fz * fz * fz + 2 * fz * fz - fz) * Position0.vector4_f32[2]
					+ (3 * fz * fz * fz - 5 * fz * fz + 2) * Position1.vector4_f32[2]
					+ (-3 * fz * fz * fz + 4 * fz * fz + fz) * Position2.vector4_f32[2]
					+ (fz * fz * fz - fz * fz) * Position3.vector4_f32[2]),

					0.5f * ((-fw * fw * fw + 2 * fw * fw - fw) * Position0.vector4_f32[3]
					+ (3 * fw * fw * fw - 5 * fw * fw + 2) * Position1.vector4_f32[3]
					+ (-3 * fw * fw * fw + 4 * fw * fw + fw) * Position2.vector4_f32[3]
					+ (fw * fw * fw - fw * fw) * Position3.vector4_f32[3])
				} } };
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			static const VectorF32 Catmul2 = { { { 2.0f, 2.0f, 2.0f, 2.0f } } };
			static const VectorF32 Catmul3 = { { { 3.0f, 3.0f, 3.0f, 3.0f } } };
			static const VectorF32 Catmul4 = { { { 4.0f, 4.0f, 4.0f, 4.0f } } };
			static const VectorF32 Catmul5 = { { { 5.0f, 5.0f, 5.0f, 5.0f } } };
			// Cache T^2 and T^3
			Vector T2 = vmulq_f32(T, T);
			Vector T3 = vmulq_f32(T, T2);
			// Perform the Position0 term
			Vector vResult = vaddq_f32(T2, T2);
			vResult = vsubq_f32(vResult, T);
			vResult = vsubq_f32(vResult, T3);
			vResult = vmulq_f32(vResult, Position0);
			// Perform the Position1 term and add
			Vector vTemp = vmulq_f32(T3, Catmul3);
			vTemp = vmlsq_f32(vTemp, T2, Catmul5);
			vTemp = vaddq_f32(vTemp, Catmul2);
			vResult = vmlaq_f32(vResult, vTemp, Position1);
			// Perform the Position2 term and add
			vTemp = vmulq_f32(T2, Catmul4);
			vTemp = vmlsq_f32(vTemp, T3, Catmul3);
			vTemp = vaddq_f32(vTemp, T);
			vResult = vmlaq_f32(vResult, vTemp, Position2);
			// Position3 is the last term
			T3 = vsubq_f32(T3, T2);
			vResult = vmlaq_f32(vResult, T3, Position3);
			// Multiply by 0.5f and exit
			vResult = vmulq_f32(vResult, g_XMOneHalf);
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			static const VectorF32 Catmul2 = { { { 2.0f, 2.0f, 2.0f, 2.0f } } };
			static const VectorF32 Catmul3 = { { { 3.0f, 3.0f, 3.0f, 3.0f } } };
			static const VectorF32 Catmul4 = { { { 4.0f, 4.0f, 4.0f, 4.0f } } };
			static const VectorF32 Catmul5 = { { { 5.0f, 5.0f, 5.0f, 5.0f } } };
			// Cache T^2 and T^3
			Vector T2 = _mm_mul_ps(T, T);
			Vector T3 = _mm_mul_ps(T, T2);
			// Perform the Position0 term
			Vector vResult = _mm_add_ps(T2, T2);
			vResult = _mm_sub_ps(vResult, T);
			vResult = _mm_sub_ps(vResult, T3);
			vResult = _mm_mul_ps(vResult, Position0);
			// Perform the Position1 term and add
			Vector vTemp = _mm_mul_ps(T3, Catmul3);
			Vector vTemp2 = _mm_mul_ps(T2, Catmul5);
			vTemp = _mm_sub_ps(vTemp, vTemp2);
			vTemp = _mm_add_ps(vTemp, Catmul2);
			vTemp = _mm_mul_ps(vTemp, Position1);
			vResult = _mm_add_ps(vResult, vTemp);
			// Perform the Position2 term and add
			vTemp = _mm_mul_ps(T2, Catmul4);
			vTemp2 = _mm_mul_ps(T3, Catmul3);
			vTemp = _mm_sub_ps(vTemp, vTemp2);
			vTemp = _mm_add_ps(vTemp, T);
			vTemp = _mm_mul_ps(vTemp, Position2);
			vResult = _mm_add_ps(vResult, vTemp);
			// Position3 is the last term
			T3 = _mm_sub_ps(T3, T2);
			T3 = _mm_mul_ps(T3, Position3);
			vResult = _mm_add_ps(vResult, T3);
			// Multiply by 0.5f and exit
			vResult = _mm_mul_ps(vResult, g_XMOneHalf);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorBaryCentric
		(
			FVector Position0,
			FVector Position1,
			FVector Position2,
			float    f,
			float    g
		)
		{
			// Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)

#if defined(_XM_NO_INTRINSICS_)

			Vector P10 = VectorSubtract(Position1, Position0);
			Vector ScaleF = VectorReplicate(f);

			Vector P20 = VectorSubtract(Position2, Position0);
			Vector ScaleG = VectorReplicate(g);

			Vector Result = VectorMultiplyAdd(P10, ScaleF, Position0);
			Result = VectorMultiplyAdd(P20, ScaleG, Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Vector R1 = vsubq_f32(Position1, Position0);
			Vector R2 = vsubq_f32(Position2, Position0);
			R1 = vmlaq_n_f32(Position0, R1, f);
			return vmlaq_n_f32(R1, R2, g);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector R1 = _mm_sub_ps(Position1, Position0);
			Vector SF = _mm_set_ps1(f);
			Vector R2 = _mm_sub_ps(Position2, Position0);
			Vector SG = _mm_set_ps1(g);
			R1 = _mm_mul_ps(R1, SF);
			R2 = _mm_mul_ps(R2, SG);
			R1 = _mm_add_ps(R1, Position0);
			R1 = _mm_add_ps(R1, R2);
			return R1;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV VectorBaryCentricV
		(
			FVector Position0,
			FVector Position1,
			FVector Position2,
			GVector F,
			HVector G
		)
		{
			// Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)

#if defined(_XM_NO_INTRINSICS_)

			Vector P10 = VectorSubtract(Position1, Position0);
			Vector P20 = VectorSubtract(Position2, Position0);

			Vector Result = VectorMultiplyAdd(P10, F, Position0);
			Result = VectorMultiplyAdd(P20, G, Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Vector R1 = vsubq_f32(Position1, Position0);
			Vector R2 = vsubq_f32(Position2, Position0);
			R1 = vmlaq_f32(Position0, R1, F);
			return vmlaq_f32(R1, R2, G);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector R1 = _mm_sub_ps(Position1, Position0);
			Vector R2 = _mm_sub_ps(Position2, Position0);
			R1 = _mm_mul_ps(R1, F);
			R2 = _mm_mul_ps(R2, G);
			R1 = _mm_add_ps(R1, Position0);
			R1 = _mm_add_ps(R1, R2);
			return R1;
#endif
		}

		/****************************************************************************
		 *
		 * 2D Vector
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------
		 // Comparison operations
		 //------------------------------------------------------------------------------

		 //------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector2Equal
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] == V2.vector4_f32[0]) && (V1.vector4_f32[1] == V2.vector4_f32[1])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t vTemp = vceq_f32(vget_low_f32(V1), vget_low_f32(V2));
			return (vget_lane_u64(vTemp, 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpeq_ps(V1, V2);
			// z and w are don't care
			return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#endif
		}


		//------------------------------------------------------------------------------

		inline uint32_t XM_CALLCONV Vector2EqualR
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			uint32_t CR = 0;
			if ((V1.vector4_f32[0] == V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] == V2.vector4_f32[1]))
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] != V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] != V2.vector4_f32[1]))
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t vTemp = vceq_f32(vget_low_f32(V1), vget_low_f32(V2));
			uint64_t r = vget_lane_u64(vTemp, 0);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFFFFFFFFFU)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpeq_ps(V1, V2);
			// z and w are don't care
			int iTest = _mm_movemask_ps(vTemp) & 3;
			uint32_t CR = 0;
			if (iTest == 3)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector2EqualInt
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_u32[0] == V2.vector4_u32[0]) && (V1.vector4_u32[1] == V2.vector4_u32[1])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t vTemp = vceq_u32(vget_low_u32(V1), vget_low_u32(V2));
			return (vget_lane_u64(vTemp, 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 3) == 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t XM_CALLCONV Vector2EqualIntR
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			uint32_t CR = 0;
			if ((V1.vector4_u32[0] == V2.vector4_u32[0]) &&
				(V1.vector4_u32[1] == V2.vector4_u32[1]))
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_u32[0] != V2.vector4_u32[0]) &&
				(V1.vector4_u32[1] != V2.vector4_u32[1]))
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t vTemp = vceq_u32(vget_low_u32(V1), vget_low_u32(V2));
			uint64_t r = vget_lane_u64(vTemp, 0);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFFFFFFFFFU)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			int iTest = _mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 3;
			uint32_t CR = 0;
			if (iTest == 3)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector2NearEqual
		(
			FVector V1,
			FVector V2,
			FVector Epsilon
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			float dx = fabsf(V1.vector4_f32[0] - V2.vector4_f32[0]);
			float dy = fabsf(V1.vector4_f32[1] - V2.vector4_f32[1]);
			return ((dx <= Epsilon.vector4_f32[0]) &&
				(dy <= Epsilon.vector4_f32[1]));
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t vDelta = vsub_f32(vget_low_u32(V1), vget_low_u32(V2));
			uint32x2_t vTemp = vacle_f32(vDelta, vget_low_u32(Epsilon));
			uint64_t r = vget_lane_u64(vTemp, 0);
			return (r == 0xFFFFFFFFFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			// Get the difference
			Vector vDelta = _mm_sub_ps(V1, V2);
			// Get the absolute value of the difference
			Vector vTemp = _mm_setzero_ps();
			vTemp = _mm_sub_ps(vTemp, vDelta);
			vTemp = _mm_max_ps(vTemp, vDelta);
			vTemp = _mm_cmple_ps(vTemp, Epsilon);
			// z and w are don't care
			return (((_mm_movemask_ps(vTemp) & 3) == 0x3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector2NotEqual
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] != V2.vector4_f32[0]) || (V1.vector4_f32[1] != V2.vector4_f32[1])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t vTemp = vceq_f32(vget_low_f32(V1), vget_low_f32(V2));
			return (vget_lane_u64(vTemp, 0) != 0xFFFFFFFFFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpeq_ps(V1, V2);
			// z and w are don't care
			return (((_mm_movemask_ps(vTemp) & 3) != 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector2NotEqualInt
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_u32[0] != V2.vector4_u32[0]) || (V1.vector4_u32[1] != V2.vector4_u32[1])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t vTemp = vceq_u32(vget_low_u32(V1), vget_low_u32(V2));
			return (vget_lane_u64(vTemp, 0) != 0xFFFFFFFFFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 3) != 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector2Greater
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] > V2.vector4_f32[0]) && (V1.vector4_f32[1] > V2.vector4_f32[1])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t vTemp = vcgt_f32(vget_low_f32(V1), vget_low_f32(V2));
			return (vget_lane_u64(vTemp, 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpgt_ps(V1, V2);
			// z and w are don't care
			return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t XM_CALLCONV Vector2GreaterR
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			uint32_t CR = 0;
			if ((V1.vector4_f32[0] > V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] > V2.vector4_f32[1]))
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] <= V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] <= V2.vector4_f32[1]))
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t vTemp = vcgt_f32(vget_low_f32(V1), vget_low_f32(V2));
			uint64_t r = vget_lane_u64(vTemp, 0);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFFFFFFFFFU)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpgt_ps(V1, V2);
			int iTest = _mm_movemask_ps(vTemp) & 3;
			uint32_t CR = 0;
			if (iTest == 3)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector2GreaterOrEqual
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] >= V2.vector4_f32[0]) && (V1.vector4_f32[1] >= V2.vector4_f32[1])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t vTemp = vcge_f32(vget_low_f32(V1), vget_low_f32(V2));
			return (vget_lane_u64(vTemp, 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpge_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t XM_CALLCONV Vector2GreaterOrEqualR
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			uint32_t CR = 0;
			if ((V1.vector4_f32[0] >= V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] >= V2.vector4_f32[1]))
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] < V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] < V2.vector4_f32[1]))
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t vTemp = vcge_f32(vget_low_f32(V1), vget_low_f32(V2));
			uint64_t r = vget_lane_u64(vTemp, 0);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFFFFFFFFFU)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpge_ps(V1, V2);
			int iTest = _mm_movemask_ps(vTemp) & 3;
			uint32_t CR = 0;
			if (iTest == 3)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector2Less
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] < V2.vector4_f32[0]) && (V1.vector4_f32[1] < V2.vector4_f32[1])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t vTemp = vclt_f32(vget_low_f32(V1), vget_low_f32(V2));
			return (vget_lane_u64(vTemp, 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmplt_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector2LessOrEqual
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] <= V2.vector4_f32[0]) && (V1.vector4_f32[1] <= V2.vector4_f32[1])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x2_t vTemp = vcle_f32(vget_low_f32(V1), vget_low_f32(V2));
			return (vget_lane_u64(vTemp, 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmple_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector2InBounds
		(
			FVector V,
			FVector Bounds
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V.vector4_f32[0] <= Bounds.vector4_f32[0] && V.vector4_f32[0] >= -Bounds.vector4_f32[0]) &&
				(V.vector4_f32[1] <= Bounds.vector4_f32[1] && V.vector4_f32[1] >= -Bounds.vector4_f32[1])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			float32x2_t B = vget_low_f32(Bounds);
			// Test if less than or equal
			uint32x2_t ivTemp1 = vcle_f32(VL, B);
			// Negate the bounds
			float32x2_t vTemp2 = vneg_f32(B);
			// Test if greater or equal (Reversed)
			uint32x2_t ivTemp2 = vcle_f32(vTemp2, VL);
			// Blend answers
			ivTemp1 = vand_u32(ivTemp1, ivTemp2);
			// x and y in bounds?
			return (vget_lane_u64(ivTemp1, 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			// Test if less than or equal
			Vector vTemp1 = _mm_cmple_ps(V, Bounds);
			// Negate the bounds
			Vector vTemp2 = _mm_mul_ps(Bounds, g_XMNegativeOne);
			// Test if greater or equal (Reversed)
			vTemp2 = _mm_cmple_ps(vTemp2, V);
			// Blend answers
			vTemp1 = _mm_and_ps(vTemp1, vTemp2);
			// x and y in bounds? (z and w are don't care)
			return (((_mm_movemask_ps(vTemp1) & 0x3) == 0x3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector2IsNaN
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (XMISNAN(V.vector4_f32[0]) ||
				XMISNAN(V.vector4_f32[1]));
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			// Test against itself. NaN is always not equal
			uint32x2_t vTempNan = vceq_f32(VL, VL);
			// If x or y are NaN, the mask is zero
			return (vget_lane_u64(vTempNan, 0) != 0xFFFFFFFFFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			// Test against itself. NaN is always not equal
			Vector vTempNan = _mm_cmpneq_ps(V, V);
			// If x or y are NaN, the mask is non-zero
			return ((_mm_movemask_ps(vTempNan) & 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector2IsInfinite
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			return (XMISINF(V.vector4_f32[0]) ||
				XMISINF(V.vector4_f32[1]));
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Mask off the sign bit
			uint32x2_t vTemp = vand_u32(vget_low_f32(V), vget_low_f32(g_XMAbsMask));
			// Compare to infinity
			vTemp = vceq_f32(vTemp, vget_low_f32(g_XMInfinity));
			// If any are infinity, the signs are true.
			return vget_lane_u64(vTemp, 0) != 0;
#elif defined(_XM_SSE_INTRINSICS_)
			// Mask off the sign bit
			__m128 vTemp = _mm_and_ps(V, g_XMAbsMask);
			// Compare to infinity
			vTemp = _mm_cmpeq_ps(vTemp, g_XMInfinity);
			// If x or z are infinity, the signs are true.
			return ((_mm_movemask_ps(vTemp) & 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2Dot
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result;
			Result.f[0] =
				Result.f[1] =
				Result.f[2] =
				Result.f[3] = V1.vector4_f32[0] * V2.vector4_f32[0] + V1.vector4_f32[1] * V2.vector4_f32[1];
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Perform the dot product on x and y
			float32x2_t vTemp = vmul_f32(vget_low_f32(V1), vget_low_f32(V2));
			vTemp = vpadd_f32(vTemp, vTemp);
			return vcombine_f32(vTemp, vTemp);
#elif defined(_XM_SSE4_INTRINSICS_)
			return _mm_dp_ps(V1, V2, 0x3f);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vDot = _mm_mul_ps(V1, V2);
			vDot = _mm_hadd_ps(vDot, vDot);
			vDot = _mm_moveldup_ps(vDot);
			return vDot;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x and y
			Vector vLengthSq = _mm_mul_ps(V1, V2);
			// vTemp has y splatted
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			// x+y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2Cross
		(
			FVector V1,
			FVector V2
		)
		{
			// [ V1.x*V2.y - V1.y*V2.x, V1.x*V2.y - V1.y*V2.x ]

#if defined(_XM_NO_INTRINSICS_)
			float fCross = (V1.vector4_f32[0] * V2.vector4_f32[1]) - (V1.vector4_f32[1] * V2.vector4_f32[0]);
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = fCross;
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			static const VectorF32 Negate = { { { 1.f, -1.f, 0, 0 } } };

			float32x2_t vTemp = vmul_f32(vget_low_f32(V1), vrev64_f32(vget_low_f32(V2)));
			vTemp = vmul_f32(vTemp, vget_low_f32(Negate));
			vTemp = vpadd_f32(vTemp, vTemp);
			return vcombine_f32(vTemp, vTemp);
#elif defined(_XM_SSE_INTRINSICS_)
	// Swap x and y
			Vector vResult = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 1, 0, 1));
			// Perform the muls
			vResult = _mm_mul_ps(vResult, V1);
			// Splat y
			Vector vTemp = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(1, 1, 1, 1));
			// Sub the values
			vResult = _mm_sub_ss(vResult, vTemp);
			// Splat the cross product
			vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 0, 0, 0));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2LengthSq
		(
			FVector V
		)
		{
			return Vector2Dot(V, V);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2ReciprocalLengthEst
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Result;
			Result = Vector2LengthSq(V);
			Result = VectorReciprocalSqrtEst(Result);
			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			// Dot2
			float32x2_t vTemp = vmul_f32(VL, VL);
			vTemp = vpadd_f32(vTemp, vTemp);
			// Reciprocal sqrt (estimate)
			vTemp = vrsqrte_f32(vTemp);
			return vcombine_f32(vTemp, vTemp);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(V, V, 0x3f);
			return _mm_rsqrt_ps(vTemp);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vLengthSq = _mm_mul_ps(V, V);
			Vector vTemp = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_rsqrt_ss(vTemp);
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x and y
			Vector vLengthSq = _mm_mul_ps(V, V);
			// vTemp has y splatted
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			// x+y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_rsqrt_ss(vLengthSq);
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2ReciprocalLength
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Result;
			Result = Vector2LengthSq(V);
			Result = VectorReciprocalSqrt(Result);
			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			// Dot2
			float32x2_t vTemp = vmul_f32(VL, VL);
			vTemp = vpadd_f32(vTemp, vTemp);
			// Reciprocal sqrt
			float32x2_t  S0 = vrsqrte_f32(vTemp);
			float32x2_t  P0 = vmul_f32(vTemp, S0);
			float32x2_t  R0 = vrsqrts_f32(P0, S0);
			float32x2_t  S1 = vmul_f32(S0, R0);
			float32x2_t  P1 = vmul_f32(vTemp, S1);
			float32x2_t  R1 = vrsqrts_f32(P1, S1);
			float32x2_t Result = vmul_f32(S1, R1);
			return vcombine_f32(Result, Result);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(V, V, 0x3f);
			Vector vLengthSq = _mm_sqrt_ps(vTemp);
			return _mm_div_ps(g_XMOne, vLengthSq);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vLengthSq = _mm_mul_ps(V, V);
			Vector vTemp = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ss(vTemp);
			vLengthSq = _mm_div_ss(g_XMOne, vLengthSq);
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x and y
			Vector vLengthSq = _mm_mul_ps(V, V);
			// vTemp has y splatted
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			// x+y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_sqrt_ss(vLengthSq);
			vLengthSq = _mm_div_ss(g_XMOne, vLengthSq);
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2LengthEst
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Result;
			Result = Vector2LengthSq(V);
			Result = VectorSqrtEst(Result);
			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			// Dot2
			float32x2_t vTemp = vmul_f32(VL, VL);
			vTemp = vpadd_f32(vTemp, vTemp);
			const float32x2_t zero = vdup_n_f32(0);
			uint32x2_t VEqualsZero = vceq_f32(vTemp, zero);
			// Sqrt (estimate)
			float32x2_t Result = vrsqrte_f32(vTemp);
			Result = vmul_f32(vTemp, Result);
			Result = vbsl_f32(VEqualsZero, zero, Result);
			return vcombine_f32(Result, Result);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(V, V, 0x3f);
			return _mm_sqrt_ps(vTemp);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vLengthSq = _mm_mul_ps(V, V);
			Vector vTemp = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ss(vTemp);
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x and y
			Vector vLengthSq = _mm_mul_ps(V, V);
			// vTemp has y splatted
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			// x+y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_sqrt_ss(vLengthSq);
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2Length
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Result;
			Result = Vector2LengthSq(V);
			Result = VectorSqrt(Result);
			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			// Dot2
			float32x2_t vTemp = vmul_f32(VL, VL);
			vTemp = vpadd_f32(vTemp, vTemp);
			const float32x2_t zero = vdup_n_f32(0);
			uint32x2_t VEqualsZero = vceq_f32(vTemp, zero);
			// Sqrt
			float32x2_t S0 = vrsqrte_f32(vTemp);
			float32x2_t P0 = vmul_f32(vTemp, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(vTemp, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			float32x2_t Result = vmul_f32(S1, R1);
			Result = vmul_f32(vTemp, Result);
			Result = vbsl_f32(VEqualsZero, zero, Result);
			return vcombine_f32(Result, Result);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(V, V, 0x3f);
			return _mm_sqrt_ps(vTemp);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vLengthSq = _mm_mul_ps(V, V);
			Vector vTemp = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ss(vTemp);
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x and y
			Vector vLengthSq = _mm_mul_ps(V, V);
			// vTemp has y splatted
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			// x+y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------
		// Vector2NormalizeEst uses a reciprocal estimate and
		// returns QNaN on zero and infinite vectors.

		inline Vector XM_CALLCONV Vector2NormalizeEst
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Result;
			Result = Vector2ReciprocalLength(V);
			Result = VectorMultiply(V, Result);
			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			// Dot2
			float32x2_t vTemp = vmul_f32(VL, VL);
			vTemp = vpadd_f32(vTemp, vTemp);
			// Reciprocal sqrt (estimate)
			vTemp = vrsqrte_f32(vTemp);
			// Normalize
			float32x2_t Result = vmul_f32(VL, vTemp);
			return vcombine_f32(Result, Result);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(V, V, 0x3f);
			Vector vResult = _mm_rsqrt_ps(vTemp);
			return _mm_mul_ps(vResult, V);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_rsqrt_ss(vLengthSq);
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			vLengthSq = _mm_mul_ps(vLengthSq, V);
			return vLengthSq;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x and y
			Vector vLengthSq = _mm_mul_ps(V, V);
			// vTemp has y splatted
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			// x+y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_rsqrt_ss(vLengthSq);
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			vLengthSq = _mm_mul_ps(vLengthSq, V);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2Normalize
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector vResult = Vector2Length(V);
			float fLength = vResult.vector4_f32[0];

			// Prevent divide by zero
			if (fLength > 0) {
				fLength = 1.0f / fLength;
			}

			vResult.vector4_f32[0] = V.vector4_f32[0] * fLength;
			vResult.vector4_f32[1] = V.vector4_f32[1] * fLength;
			vResult.vector4_f32[2] = V.vector4_f32[2] * fLength;
			vResult.vector4_f32[3] = V.vector4_f32[3] * fLength;
			return vResult;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			// Dot2
			float32x2_t vTemp = vmul_f32(VL, VL);
			vTemp = vpadd_f32(vTemp, vTemp);
			uint32x2_t VEqualsZero = vceq_f32(vTemp, vdup_n_f32(0));
			uint32x2_t VEqualsInf = vceq_f32(vTemp, vget_low_f32(g_XMInfinity));
			// Reciprocal sqrt (2 iterations of Newton-Raphson)
			float32x2_t S0 = vrsqrte_f32(vTemp);
			float32x2_t P0 = vmul_f32(vTemp, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(vTemp, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			vTemp = vmul_f32(S1, R1);
			// Normalize
			float32x2_t Result = vmul_f32(VL, vTemp);
			Result = vbsl_f32(VEqualsZero, vdup_n_f32(0), Result);
			Result = vbsl_f32(VEqualsInf, vget_low_f32(g_XMQNaN), Result);
			return vcombine_f32(Result, Result);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vLengthSq = _mm_dp_ps(V, V, 0x3f);
			// Prepare for the division
			Vector vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			Vector vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_XMInfinity);
			// Reciprocal mul to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			Vector vTemp1 = _mm_andnot_ps(vLengthSq, g_XMQNaN);
			Vector vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#elif defined(_XM_SSE3_INTRINSICS_)
			// Perform the dot product on x and y only
			Vector vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_moveldup_ps(vLengthSq);
			// Prepare for the division
			Vector vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			Vector vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_XMInfinity);
			// Reciprocal mul to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			Vector vTemp1 = _mm_andnot_ps(vLengthSq, g_XMQNaN);
			Vector vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x and y only
			Vector vLengthSq = _mm_mul_ps(V, V);
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			// Prepare for the division
			Vector vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			Vector vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_XMInfinity);
			// Reciprocal mul to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			Vector vTemp1 = _mm_andnot_ps(vLengthSq, g_XMQNaN);
			Vector vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2ClampLength
		(
			FVector V,
			float    LengthMin,
			float    LengthMax
		)
		{
			Vector ClampMax = VectorReplicate(LengthMax);
			Vector ClampMin = VectorReplicate(LengthMin);
			return Vector2ClampLengthV(V, ClampMin, ClampMax);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2ClampLengthV
		(
			FVector V,
			FVector LengthMin,
			FVector LengthMax
		)
		{
			assert((VectorGetY(LengthMin) == VectorGetX(LengthMin)));
			assert((VectorGetY(LengthMax) == VectorGetX(LengthMax)));
			assert(Vector2GreaterOrEqual(LengthMin, g_XMZero));
			assert(Vector2GreaterOrEqual(LengthMax, g_XMZero));
			assert(Vector2GreaterOrEqual(LengthMax, LengthMin));

			Vector LengthSq = Vector2LengthSq(V);

			const Vector Zero = VectorZero();

			Vector RcpLength = VectorReciprocalSqrt(LengthSq);

			Vector InfiniteLength = VectorEqualInt(LengthSq, g_XMInfinity.v);
			Vector ZeroLength = VectorEqual(LengthSq, Zero);

			Vector Length = VectorMultiply(LengthSq, RcpLength);

			Vector Normal = VectorMultiply(V, RcpLength);

			Vector Select = VectorEqualInt(InfiniteLength, ZeroLength);
			Length = VectorSelect(LengthSq, Length, Select);
			Normal = VectorSelect(LengthSq, Normal, Select);

			Vector ControlMax = VectorGreater(Length, LengthMax);
			Vector ControlMin = VectorLess(Length, LengthMin);

			Vector ClampLength = VectorSelect(Length, LengthMax, ControlMax);
			ClampLength = VectorSelect(ClampLength, LengthMin, ControlMin);

			Vector Result = VectorMultiply(Normal, ClampLength);

			// Preserve the original vector (with no precision loss) if the length falls within the given range
			Vector Control = VectorEqualInt(ControlMax, ControlMin);
			Result = VectorSelect(Result, V, Control);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2Reflect
		(
			FVector Incident,
			FVector Normal
		)
		{
			// Result = Incident - (2 * dot(Incident, Normal)) * Normal

			Vector Result;
			Result = Vector2Dot(Incident, Normal);
			Result = VectorAdd(Result, Result);
			Result = VectorNegativeMultiplySubtract(Result, Normal, Incident);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2Refract
		(
			FVector Incident,
			FVector Normal,
			float    RefractionIndex
		)
		{
			Vector Index = VectorReplicate(RefractionIndex);
			return Vector2RefractV(Incident, Normal, Index);
		}

		//------------------------------------------------------------------------------

		// Return the refraction of a 2D vector
		inline Vector XM_CALLCONV Vector2RefractV
		(
			FVector Incident,
			FVector Normal,
			FVector RefractionIndex
		)
		{
			// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
			// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))

#if defined(_XM_NO_INTRINSICS_)

			float IDotN = (Incident.vector4_f32[0] * Normal.vector4_f32[0]) + (Incident.vector4_f32[1] * Normal.vector4_f32[1]);
			// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			float RY = 1.0f - (IDotN * IDotN);
			float RX = 1.0f - (RY * RefractionIndex.vector4_f32[0] * RefractionIndex.vector4_f32[0]);
			RY = 1.0f - (RY * RefractionIndex.vector4_f32[1] * RefractionIndex.vector4_f32[1]);
			if (RX >= 0.0f) {
				RX = (RefractionIndex.vector4_f32[0] * Incident.vector4_f32[0]) - (Normal.vector4_f32[0] * ((RefractionIndex.vector4_f32[0] * IDotN) + sqrtf(RX)));
			}
			else {
				RX = 0.0f;
			}
			if (RY >= 0.0f) {
				RY = (RefractionIndex.vector4_f32[1] * Incident.vector4_f32[1]) - (Normal.vector4_f32[1] * ((RefractionIndex.vector4_f32[1] * IDotN) + sqrtf(RY)));
			}
			else {
				RY = 0.0f;
			}

			Vector vResult;
			vResult.vector4_f32[0] = RX;
			vResult.vector4_f32[1] = RY;
			vResult.vector4_f32[2] = 0.0f;
			vResult.vector4_f32[3] = 0.0f;
			return vResult;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t IL = vget_low_f32(Incident);
			float32x2_t NL = vget_low_f32(Normal);
			float32x2_t RIL = vget_low_f32(RefractionIndex);
			// Get the 2D Dot product of Incident-Normal
			float32x2_t vTemp = vmul_f32(IL, NL);
			float32x2_t IDotN = vpadd_f32(vTemp, vTemp);
			// vTemp = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			vTemp = vmls_f32(vget_low_f32(g_XMOne), IDotN, IDotN);
			vTemp = vmul_f32(vTemp, RIL);
			vTemp = vmls_f32(vget_low_f32(g_XMOne), vTemp, RIL);
			// If any terms are <=0, sqrt() will fail, punt to zero
			uint32x2_t vMask = vcgt_f32(vTemp, vget_low_f32(g_XMZero));
			// Sqrt(vTemp)
			float32x2_t S0 = vrsqrte_f32(vTemp);
			float32x2_t P0 = vmul_f32(vTemp, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(vTemp, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			float32x2_t S2 = vmul_f32(S1, R1);
			vTemp = vmul_f32(vTemp, S2);
			// R = RefractionIndex * IDotN + sqrt(R)
			vTemp = vmla_f32(vTemp, RIL, IDotN);
			// Result = RefractionIndex * Incident - Normal * R
			float32x2_t vResult = vmul_f32(RIL, IL);
			vResult = vmls_f32(vResult, vTemp, NL);
			vResult = vand_u32(vResult, vMask);
			return vcombine_f32(vResult, vResult);
#elif defined(_XM_SSE_INTRINSICS_)
	// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
	// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
	// Get the 2D Dot product of Incident-Normal
			Vector IDotN = Vector2Dot(Incident, Normal);
			// vTemp = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			Vector vTemp = _mm_mul_ps(IDotN, IDotN);
			vTemp = _mm_sub_ps(g_XMOne, vTemp);
			vTemp = _mm_mul_ps(vTemp, RefractionIndex);
			vTemp = _mm_mul_ps(vTemp, RefractionIndex);
			vTemp = _mm_sub_ps(g_XMOne, vTemp);
			// If any terms are <=0, sqrt() will fail, punt to zero
			Vector vMask = _mm_cmpgt_ps(vTemp, g_XMZero);
			// R = RefractionIndex * IDotN + sqrt(R)
			vTemp = _mm_sqrt_ps(vTemp);
			Vector vResult = _mm_mul_ps(RefractionIndex, IDotN);
			vTemp = _mm_add_ps(vTemp, vResult);
			// Result = RefractionIndex * Incident - Normal * R
			vResult = _mm_mul_ps(RefractionIndex, Incident);
			vTemp = _mm_mul_ps(vTemp, Normal);
			vResult = _mm_sub_ps(vResult, vTemp);
			vResult = _mm_and_ps(vResult, vMask);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2Orthogonal
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result = { { {
					-V.vector4_f32[1],
					V.vector4_f32[0],
					0.f,
					0.f
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			static const VectorF32 Negate = { { { -1.f, 1.f, 0, 0 } } };
			const float32x2_t zero = vdup_n_f32(0);

			float32x2_t VL = vget_low_f32(V);
			float32x2_t Result = vmul_f32(vrev64_f32(VL), vget_low_f32(Negate));
			return vcombine_f32(Result, zero);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
			vResult = _mm_mul_ps(vResult, g_XMNegateX);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2AngleBetweenNormalsEst
		(
			FVector N1,
			FVector N2
		)
		{
			Vector Result = Vector2Dot(N1, N2);
			Result = VectorClamp(Result, g_XMNegativeOne.v, g_XMOne.v);
			Result = VectorACosEst(Result);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2AngleBetweenNormals
		(
			FVector N1,
			FVector N2
		)
		{
			Vector Result = Vector2Dot(N1, N2);
			Result = VectorClamp(Result, g_XMNegativeOne, g_XMOne);
			Result = VectorACos(Result);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2AngleBetweenVectors
		(
			FVector V1,
			FVector V2
		)
		{
			Vector L1 = Vector2ReciprocalLength(V1);
			Vector L2 = Vector2ReciprocalLength(V2);

			Vector Dot = Vector2Dot(V1, V2);

			L1 = VectorMultiply(L1, L2);

			Vector CosAngle = VectorMultiply(Dot, L1);
			CosAngle = VectorClamp(CosAngle, g_XMNegativeOne.v, g_XMOne.v);

			return VectorACos(CosAngle);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2LinePointDistance
		(
			FVector LinePoint1,
			FVector LinePoint2,
			FVector Point
		)
		{
			// Given a vector PointVector from LinePoint1 to Point and a vector
			// LineVector from LinePoint1 to LinePoint2, the scaled distance 
			// PointProjectionScale from LinePoint1 to the perpendicular projection
			// of PointVector onto the line is defined as:
			//
			//     PointProjectionScale = dot(PointVector, LineVector) / LengthSq(LineVector)

			Vector PointVector = VectorSubtract(Point, LinePoint1);
			Vector LineVector = VectorSubtract(LinePoint2, LinePoint1);

			Vector LengthSq = Vector2LengthSq(LineVector);

			Vector PointProjectionScale = Vector2Dot(PointVector, LineVector);
			PointProjectionScale = VectorDivide(PointProjectionScale, LengthSq);

			Vector DistanceVector = VectorMultiply(LineVector, PointProjectionScale);
			DistanceVector = VectorSubtract(PointVector, DistanceVector);

			return Vector2Length(DistanceVector);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2IntersectLine
		(
			FVector Line1Point1,
			FVector Line1Point2,
			FVector Line2Point1,
			GVector Line2Point2
		)
		{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)

			Vector V1 = VectorSubtract(Line1Point2, Line1Point1);
			Vector V2 = VectorSubtract(Line2Point2, Line2Point1);
			Vector V3 = VectorSubtract(Line1Point1, Line2Point1);

			Vector C1 = Vector2Cross(V1, V2);
			Vector C2 = Vector2Cross(V2, V3);

			Vector Result;
			const Vector Zero = VectorZero();
			if (Vector2NearEqual(C1, Zero, g_XMEpsilon.v))
			{
				if (Vector2NearEqual(C2, Zero, g_XMEpsilon.v))
				{
					// Coincident
					Result = g_XMInfinity.v;
				}
				else
				{
					// Parallel
					Result = g_XMQNaN.v;
				}
			}
			else
			{
				// Intersection point = Line1Point1 + V1 * (C2 / C1)
				Vector Scale = VectorReciprocal(C1);
				Scale = VectorMultiply(C2, Scale);
				Result = VectorMultiplyAdd(V1, Scale, Line1Point1);
			}

			return Result;

#elif defined(_XM_SSE_INTRINSICS_)
			Vector V1 = _mm_sub_ps(Line1Point2, Line1Point1);
			Vector V2 = _mm_sub_ps(Line2Point2, Line2Point1);
			Vector V3 = _mm_sub_ps(Line1Point1, Line2Point1);
			// Generate the cross products
			Vector C1 = Vector2Cross(V1, V2);
			Vector C2 = Vector2Cross(V2, V3);
			// If C1 is not close to epsilon, use the calculated value
			Vector vResultMask = _mm_setzero_ps();
			vResultMask = _mm_sub_ps(vResultMask, C1);
			vResultMask = _mm_max_ps(vResultMask, C1);
			// 0xFFFFFFFF if the calculated value is to be used
			vResultMask = _mm_cmpgt_ps(vResultMask, g_XMEpsilon);
			// If C1 is close to epsilon, which fail type is it? INFINITY or NAN?
			Vector vFailMask = _mm_setzero_ps();
			vFailMask = _mm_sub_ps(vFailMask, C2);
			vFailMask = _mm_max_ps(vFailMask, C2);
			vFailMask = _mm_cmple_ps(vFailMask, g_XMEpsilon);
			Vector vFail = _mm_and_ps(vFailMask, g_XMInfinity);
			vFailMask = _mm_andnot_ps(vFailMask, g_XMQNaN);
			// vFail is NAN or INF
			vFail = _mm_or_ps(vFail, vFailMask);
			// Intersection point = Line1Point1 + V1 * (C2 / C1)
			Vector vResult = _mm_div_ps(C2, C1);
			vResult = _mm_mul_ps(vResult, V1);
			vResult = _mm_add_ps(vResult, Line1Point1);
			// Use result, or failure value
			vResult = _mm_and_ps(vResult, vResultMask);
			vResultMask = _mm_andnot_ps(vResultMask, vFail);
			vResult = _mm_or_ps(vResult, vResultMask);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2Transform
		(
			FVector V,
			FMatrix M
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Y = VectorSplatY(V);
			Vector X = VectorSplatX(V);

			Vector Result = VectorMultiplyAdd(Y, M.r[1], M.r[3]);
			Result = VectorMultiplyAdd(X, M.r[0], Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			float32x4_t Result = vmlaq_lane_f32(M.r[3], M.r[1], VL, 1); // Y
			return vmlaq_lane_f32(Result, M.r[0], VL, 0); // X
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
			vResult = _mm_mul_ps(vResult, M.r[0]);
			Vector vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			vTemp = _mm_mul_ps(vTemp, M.r[1]);
			vResult = _mm_add_ps(vResult, vTemp);
			vResult = _mm_add_ps(vResult, M.r[3]);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------


		inline Float4* XM_CALLCONV Vector2TransformStream
		(
			Float4* pOutputStream,
			size_t          OutputStride,
			const Float2* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			FMatrix       M
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float2));
			_Analysis_assume_(InputStride >= sizeof(Float2));

			assert(OutputStride >= sizeof(Float4));
			_Analysis_assume_(OutputStride >= sizeof(Float4));

#if defined(_XM_NO_INTRINSICS_)

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row3 = M.r[3];

			for (size_t i = 0; i < VectorCount; i++)
			{
				Vector V = LoadFloat2(reinterpret_cast<const Float2*>(pInputVector));
				Vector Y = VectorSplatY(V);
				Vector X = VectorSplatX(V);

				Vector Result = VectorMultiplyAdd(Y, row1, row3);
				Result = VectorMultiplyAdd(X, row0, Result);

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015, "PREfast noise: Esp:1307" )
#endif

				StoreFloat4(reinterpret_cast<Float4*>(pOutputVector), Result);

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row3 = M.r[3];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float2)) && (OutputStride == sizeof(Float4)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x2_t V = vld2q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float2) * 4;

						float32x2_t r3 = vget_low_f32(row3);
						float32x2_t r = vget_low_f32(row0);
						Vector vResult0 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Ax+M
						Vector vResult1 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Bx+N

						__prefetch(pInputVector);

						r3 = vget_high_f32(row3);
						r = vget_high_f32(row0);
						Vector vResult2 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Cx+O
						Vector vResult3 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Dx+P

						__prefetch(pInputVector + XM_CACHE_LINE_SIZE);

						r = vget_low_f32(row1);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy+N

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(row1);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[1], r, 0); // Cx+Gy+O
						vResult3 = vmlaq_lane_f32(vResult3, V.val[1], r, 1); // Dx+Hy+P

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 3));

						float32x4x4_t R;
						R.val[0] = vResult0;
						R.val[1] = vResult1;
						R.val[2] = vResult2;
						R.val[3] = vResult3;

						vst4q_f32(reinterpret_cast<float*>(pOutputVector), R);
						pOutputVector += sizeof(Float4) * 4;

						i += 4;
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				float32x2_t V = vld1_f32(reinterpret_cast<const float*>(pInputVector));
				pInputVector += InputStride;

				Vector vResult = vmlaq_lane_f32(row3, row0, V, 0); // X
				vResult = vmlaq_lane_f32(vResult, row1, V, 1); // Y

				vst1q_f32(reinterpret_cast<float*>(pOutputVector), vResult);
				pOutputVector += OutputStride;
			}

			return pOutputStream;
#elif defined(_XM_SSE_INTRINSICS_)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row3 = M.r[3];

			size_t i = 0;
			size_t two = VectorCount >> 1;
			if (two > 0)
			{
				if (InputStride == sizeof(Float2))
				{
					if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF) && !(OutputStride & 0xF))
					{
						// Packed input, aligned output
						for (size_t j = 0; j < two; ++j)
						{
							Vector V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							pInputVector += sizeof(Float2) * 2;

							Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
							Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

							Vector vTemp = _mm_mul_ps(Y, row1);
							Vector vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);

							XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
							X = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

							vTemp = _mm_mul_ps(Y, row1);
							vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);

							XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 2;
						}
					}
					else
					{
						// Packed input, unaligned output
						for (size_t j = 0; j < two; ++j)
						{
							Vector V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							pInputVector += sizeof(Float2) * 2;

							Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
							Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

							Vector vTemp = _mm_mul_ps(Y, row1);
							Vector vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);

							_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
							X = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

							vTemp = _mm_mul_ps(Y, row1);
							vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);

							_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 2;
						}
					}
				}
			}

			if (!(reinterpret_cast<uintptr_t>(pInputVector) & 0xF) && !(InputStride & 0xF))
			{
				if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF) && !(OutputStride & 0xF))
				{
					// Aligned input, aligned output
					for (; i < VectorCount; i++)
					{
						Vector V = _mm_castsi128_ps(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(pInputVector)));
						pInputVector += InputStride;

						Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

						Vector vTemp = _mm_mul_ps(Y, row1);
						Vector vTemp2 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);

						XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
						pOutputVector += OutputStride;
					}
				}
				else
				{
					// Aligned input, unaligned output
					for (; i < VectorCount; i++)
					{
						Vector V = _mm_castsi128_ps(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(pInputVector)));
						pInputVector += InputStride;

						Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

						Vector vTemp = _mm_mul_ps(Y, row1);
						Vector vTemp2 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);

						_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
						pOutputVector += OutputStride;
					}
				}
			}
			else
			{
				// Unaligned input
				for (; i < VectorCount; i++)
				{
					__m128 x = _mm_load_ss(reinterpret_cast<const float*>(pInputVector));
					__m128 y = _mm_load_ss(reinterpret_cast<const float*>(pInputVector + 4));
					pInputVector += InputStride;

					Vector Y = XM_PERMUTE_PS(y, _MM_SHUFFLE(0, 0, 0, 0));
					Vector X = XM_PERMUTE_PS(x, _MM_SHUFFLE(0, 0, 0, 0));

					Vector vTemp = _mm_mul_ps(Y, row1);
					Vector vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);

					_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
				}
			}

			XM_SFENCE();

			return pOutputStream;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2TransformCoord
		(
			FVector V,
			FMatrix M
		)
		{
			Vector Y = VectorSplatY(V);
			Vector X = VectorSplatX(V);

			Vector Result = VectorMultiplyAdd(Y, M.r[1], M.r[3]);
			Result = VectorMultiplyAdd(X, M.r[0], Result);

			Vector W = VectorSplatW(Result);
			return VectorDivide(Result, W);
		}

		//------------------------------------------------------------------------------


		inline Float2* XM_CALLCONV Vector2TransformCoordStream
		(
			Float2* pOutputStream,
			size_t          OutputStride,
			const Float2* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			FMatrix       M
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float2));
			_Analysis_assume_(InputStride >= sizeof(Float2));

			assert(OutputStride >= sizeof(Float2));
			_Analysis_assume_(OutputStride >= sizeof(Float2));

#if defined(_XM_NO_INTRINSICS_)

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row3 = M.r[3];

			for (size_t i = 0; i < VectorCount; i++)
			{
				Vector V = LoadFloat2(reinterpret_cast<const Float2*>(pInputVector));
				Vector Y = VectorSplatY(V);
				Vector X = VectorSplatX(V);

				Vector Result = VectorMultiplyAdd(Y, row1, row3);
				Result = VectorMultiplyAdd(X, row0, Result);

				Vector W = VectorSplatW(Result);

				Result = VectorDivide(Result, W);

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015, "PREfast noise: Esp:1307" )
#endif

				StoreFloat2(reinterpret_cast<Float2*>(pOutputVector), Result);

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row3 = M.r[3];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float2)) && (OutputStride == sizeof(Float2)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x2_t V = vld2q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float2) * 4;

						float32x2_t r3 = vget_low_f32(row3);
						float32x2_t r = vget_low_f32(row0);
						Vector vResult0 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Ax+M
						Vector vResult1 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Bx+N

						__prefetch(pInputVector);

						r3 = vget_high_f32(row3);
						r = vget_high_f32(row0);
						Vector W = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Dx+P

						__prefetch(pInputVector + XM_CACHE_LINE_SIZE);

						r = vget_low_f32(row1);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy+N

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(row1);
						W = vmlaq_lane_f32(W, V.val[1], r, 1); // Dx+Hy+P

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 3));

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
						V.val[0] = vdivq_f32(vResult0, W);
						V.val[1] = vdivq_f32(vResult1, W);
#else
						// 2 iterations of Newton-Raphson refinement of reciprocal
						float32x4_t Reciprocal = vrecpeq_f32(W);
						float32x4_t S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);
						S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);

						V.val[0] = vmulq_f32(vResult0, Reciprocal);
						V.val[1] = vmulq_f32(vResult1, Reciprocal);
#endif

						vst2q_f32(reinterpret_cast<float*>(pOutputVector), V);
						pOutputVector += sizeof(Float2) * 4;

						i += 4;
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				float32x2_t V = vld1_f32(reinterpret_cast<const float*>(pInputVector));
				pInputVector += InputStride;

				Vector vResult = vmlaq_lane_f32(row3, row0, V, 0); // X
				vResult = vmlaq_lane_f32(vResult, row1, V, 1); // Y

				V = vget_high_f32(vResult);
				float32x2_t W = vdup_lane_f32(V, 1);

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
				V = vget_low_f32(vResult);
				V = vdiv_f32(V, W);
#else
				// 2 iterations of Newton-Raphson refinement of reciprocal for W
				float32x2_t Reciprocal = vrecpe_f32(W);
				float32x2_t S = vrecps_f32(Reciprocal, W);
				Reciprocal = vmul_f32(S, Reciprocal);
				S = vrecps_f32(Reciprocal, W);
				Reciprocal = vmul_f32(S, Reciprocal);

				V = vget_low_f32(vResult);
				V = vmul_f32(V, Reciprocal);
#endif

				vst1_f32(reinterpret_cast<float*>(pOutputVector), V);
				pOutputVector += OutputStride;
			}

			return pOutputStream;
#elif defined(_XM_SSE_INTRINSICS_)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row3 = M.r[3];

			size_t i = 0;
			size_t two = VectorCount >> 1;
			if (two > 0)
			{
				if (InputStride == sizeof(Float2))
				{
					if (OutputStride == sizeof(Float2))
					{
						if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF))
						{
							// Packed input, aligned & packed output
							for (size_t j = 0; j < two; ++j)
							{
								Vector V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								pInputVector += sizeof(Float2) * 2;

								// Result 1
								Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
								Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

								Vector vTemp = _mm_mul_ps(Y, row1);
								Vector vTemp2 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);

								Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								Vector V1 = _mm_div_ps(vTemp, W);

								// Result 2
								Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
								X = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

								vTemp = _mm_mul_ps(Y, row1);
								vTemp2 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								Vector V2 = _mm_div_ps(vTemp, W);

								vTemp = _mm_movelh_ps(V1, V2);

								XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
								pOutputVector += sizeof(Float2) * 2;

								i += 2;
							}
						}
						else
						{
							// Packed input, unaligned & packed output
							for (size_t j = 0; j < two; ++j)
							{
								Vector V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								pInputVector += sizeof(Float2) * 2;

								// Result 1
								Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
								Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

								Vector vTemp = _mm_mul_ps(Y, row1);
								Vector vTemp2 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);

								Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								Vector V1 = _mm_div_ps(vTemp, W);

								// Result 2
								Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
								X = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

								vTemp = _mm_mul_ps(Y, row1);
								vTemp2 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								Vector V2 = _mm_div_ps(vTemp, W);

								vTemp = _mm_movelh_ps(V1, V2);

								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
								pOutputVector += sizeof(Float2) * 2;

								i += 2;
							}
						}
					}
					else
					{
						// Packed input, unpacked output
						for (size_t j = 0; j < two; ++j)
						{
							Vector V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							pInputVector += sizeof(Float2) * 2;

							// Result 1
							Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
							Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

							Vector vTemp = _mm_mul_ps(Y, row1);
							Vector vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);

							Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

							vTemp = _mm_div_ps(vTemp, W);
							vTemp2 = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

							_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
							_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
							pOutputVector += OutputStride;

							// Result 2
							Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
							X = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

							vTemp = _mm_mul_ps(Y, row1);
							vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);

							W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

							vTemp = _mm_div_ps(vTemp, W);
							vTemp2 = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

							_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
							_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
							pOutputVector += OutputStride;

							i += 2;
						}
					}
				}
			}

			if (!(reinterpret_cast<uintptr_t>(pInputVector) & 0xF) && !(InputStride & 0xF))
			{
				// Aligned input
				for (; i < VectorCount; i++)
				{
					Vector V = _mm_castsi128_ps(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(pInputVector)));
					pInputVector += InputStride;

					Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
					Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

					Vector vTemp = _mm_mul_ps(Y, row1);
					Vector vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);

					Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

					vTemp = _mm_div_ps(vTemp, W);
					vTemp2 = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

					_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
					_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
					pOutputVector += OutputStride;
				}
			}
			else
			{
				// Unaligned input
				for (; i < VectorCount; i++)
				{
					__m128 x = _mm_load_ss(reinterpret_cast<const float*>(pInputVector));
					__m128 y = _mm_load_ss(reinterpret_cast<const float*>(pInputVector + 4));
					pInputVector += InputStride;

					Vector Y = XM_PERMUTE_PS(y, _MM_SHUFFLE(0, 0, 0, 0));
					Vector X = XM_PERMUTE_PS(x, _MM_SHUFFLE(0, 0, 0, 0));

					Vector vTemp = _mm_mul_ps(Y, row1);
					Vector vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);

					Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

					vTemp = _mm_div_ps(vTemp, W);
					vTemp2 = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

					_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
					_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
					pOutputVector += OutputStride;
				}
			}

			XM_SFENCE();

			return pOutputStream;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector2TransformNormal
		(
			FVector V,
			FMatrix M
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Y = VectorSplatY(V);
			Vector X = VectorSplatX(V);

			Vector Result = VectorMultiply(Y, M.r[1]);
			Result = VectorMultiplyAdd(X, M.r[0], Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			float32x4_t Result = vmulq_lane_f32(M.r[1], VL, 1); // Y
			return vmlaq_lane_f32(Result, M.r[0], VL, 0); // X
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
			vResult = _mm_mul_ps(vResult, M.r[0]);
			Vector vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			vTemp = _mm_mul_ps(vTemp, M.r[1]);
			vResult = _mm_add_ps(vResult, vTemp);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------


		inline Float2* XM_CALLCONV Vector2TransformNormalStream
		(
			Float2* pOutputStream,
			size_t          OutputStride,
			const Float2* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			FMatrix       M
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float2));
			_Analysis_assume_(InputStride >= sizeof(Float2));

			assert(OutputStride >= sizeof(Float2));
			_Analysis_assume_(OutputStride >= sizeof(Float2));

#if defined(_XM_NO_INTRINSICS_)

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];

			for (size_t i = 0; i < VectorCount; i++)
			{
				Vector V = LoadFloat2(reinterpret_cast<const Float2*>(pInputVector));
				Vector Y = VectorSplatY(V);
				Vector X = VectorSplatX(V);

				Vector Result = VectorMultiply(Y, row1);
				Result = VectorMultiplyAdd(X, row0, Result);

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015, "PREfast noise: Esp:1307" )
#endif

				StoreFloat2(reinterpret_cast<Float2*>(pOutputVector), Result);

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float2)) && (OutputStride == sizeof(Float2)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x2_t V = vld2q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float2) * 4;

						float32x2_t r = vget_low_f32(row0);
						Vector vResult0 = vmulq_lane_f32(V.val[0], r, 0); // Ax
						Vector vResult1 = vmulq_lane_f32(V.val[0], r, 1); // Bx

						__prefetch(pInputVector);
						__prefetch(pInputVector + XM_CACHE_LINE_SIZE);

						r = vget_low_f32(row1);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 2));
						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 3));

						V.val[0] = vResult0;
						V.val[1] = vResult1;

						vst2q_f32(reinterpret_cast<float*>(pOutputVector), V);
						pOutputVector += sizeof(Float2) * 4;

						i += 4;
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				float32x2_t V = vld1_f32(reinterpret_cast<const float*>(pInputVector));
				pInputVector += InputStride;

				Vector vResult = vmulq_lane_f32(row0, V, 0); // X
				vResult = vmlaq_lane_f32(vResult, row1, V, 1); // Y

				V = vget_low_f32(vResult);
				vst1_f32(reinterpret_cast<float*>(pOutputVector), V);
				pOutputVector += OutputStride;
			}

			return pOutputStream;
#elif defined(_XM_SSE_INTRINSICS_)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];

			size_t i = 0;
			size_t two = VectorCount >> 1;
			if (two > 0)
			{
				if (InputStride == sizeof(Float2))
				{
					if (OutputStride == sizeof(Float2))
					{
						if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF))
						{
							// Packed input, aligned & packed output
							for (size_t j = 0; j < two; ++j)
							{
								Vector V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								pInputVector += sizeof(Float2) * 2;

								// Result 1
								Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
								Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

								Vector vTemp = _mm_mul_ps(Y, row1);
								Vector vTemp2 = _mm_mul_ps(X, row0);
								Vector V1 = _mm_add_ps(vTemp, vTemp2);

								// Result 2
								Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
								X = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

								vTemp = _mm_mul_ps(Y, row1);
								vTemp2 = _mm_mul_ps(X, row0);
								Vector V2 = _mm_add_ps(vTemp, vTemp2);

								vTemp = _mm_movelh_ps(V1, V2);

								XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
								pOutputVector += sizeof(Float2) * 2;

								i += 2;
							}
						}
						else
						{
							// Packed input, unaligned & packed output
							for (size_t j = 0; j < two; ++j)
							{
								Vector V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								pInputVector += sizeof(Float2) * 2;

								// Result 1
								Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
								Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

								Vector vTemp = _mm_mul_ps(Y, row1);
								Vector vTemp2 = _mm_mul_ps(X, row0);
								Vector V1 = _mm_add_ps(vTemp, vTemp2);

								// Result 2
								Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
								X = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

								vTemp = _mm_mul_ps(Y, row1);
								vTemp2 = _mm_mul_ps(X, row0);
								Vector V2 = _mm_add_ps(vTemp, vTemp2);

								vTemp = _mm_movelh_ps(V1, V2);

								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
								pOutputVector += sizeof(Float2) * 2;

								i += 2;
							}
						}
					}
					else
					{
						// Packed input, unpacked output
						for (size_t j = 0; j < two; ++j)
						{
							Vector V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							pInputVector += sizeof(Float2) * 2;

							// Result 1
							Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
							Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

							Vector vTemp = _mm_mul_ps(Y, row1);
							Vector vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp2 = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

							_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
							_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
							pOutputVector += OutputStride;

							// Result 2
							Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
							X = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

							vTemp = _mm_mul_ps(Y, row1);
							vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp2 = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

							_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
							_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
							pOutputVector += OutputStride;

							i += 2;
						}
					}
				}
			}

			if (!(reinterpret_cast<uintptr_t>(pInputVector) & 0xF) && !(InputStride & 0xF))
			{
				// Aligned input
				for (; i < VectorCount; i++)
				{
					Vector V = _mm_castsi128_ps(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(pInputVector)));
					pInputVector += InputStride;

					Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
					Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

					Vector vTemp = _mm_mul_ps(Y, row1);
					Vector vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp2 = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

					_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
					_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
					pOutputVector += OutputStride;
				}
			}
			else
			{
				// Unaligned input
				for (; i < VectorCount; i++)
				{
					__m128 x = _mm_load_ss(reinterpret_cast<const float*>(pInputVector));
					__m128 y = _mm_load_ss(reinterpret_cast<const float*>(pInputVector + 4));
					pInputVector += InputStride;

					Vector Y = XM_PERMUTE_PS(y, _MM_SHUFFLE(0, 0, 0, 0));
					Vector X = XM_PERMUTE_PS(x, _MM_SHUFFLE(0, 0, 0, 0));

					Vector vTemp = _mm_mul_ps(Y, row1);
					Vector vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp2 = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

					_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
					_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
					pOutputVector += OutputStride;
				}
			}

			XM_SFENCE();

			return pOutputStream;
#endif
		}

		/****************************************************************************
		 *
		 * 3D Vector
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------
		 // Comparison operations
		 //------------------------------------------------------------------------------

		 //------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector3Equal
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] == V2.vector4_f32[0]) && (V1.vector4_f32[1] == V2.vector4_f32[1]) && (V1.vector4_f32[2] == V2.vector4_f32[2])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vceqq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpeq_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t XM_CALLCONV Vector3EqualR
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			uint32_t CR = 0;
			if ((V1.vector4_f32[0] == V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] == V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] == V2.vector4_f32[2]))
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] != V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] != V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] != V2.vector4_f32[2]))
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vceqq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU;

			uint32_t CR = 0;
			if (r == 0xFFFFFFU)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpeq_ps(V1, V2);
			int iTest = _mm_movemask_ps(vTemp) & 7;
			uint32_t CR = 0;
			if (iTest == 7)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector3EqualInt
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_u32[0] == V2.vector4_u32[0]) && (V1.vector4_u32[1] == V2.vector4_u32[1]) && (V1.vector4_u32[2] == V2.vector4_u32[2])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vceqq_u32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 7) == 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t XM_CALLCONV Vector3EqualIntR
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			uint32_t CR = 0;
			if ((V1.vector4_u32[0] == V2.vector4_u32[0]) &&
				(V1.vector4_u32[1] == V2.vector4_u32[1]) &&
				(V1.vector4_u32[2] == V2.vector4_u32[2]))
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_u32[0] != V2.vector4_u32[0]) &&
				(V1.vector4_u32[1] != V2.vector4_u32[1]) &&
				(V1.vector4_u32[2] != V2.vector4_u32[2]))
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vceqq_u32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU;

			uint32_t CR = 0;
			if (r == 0xFFFFFFU)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			int iTemp = _mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 7;
			uint32_t CR = 0;
			if (iTemp == 7)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!iTemp)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector3NearEqual
		(
			FVector V1,
			FVector V2,
			FVector Epsilon
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			float dx, dy, dz;

			dx = fabsf(V1.vector4_f32[0] - V2.vector4_f32[0]);
			dy = fabsf(V1.vector4_f32[1] - V2.vector4_f32[1]);
			dz = fabsf(V1.vector4_f32[2] - V2.vector4_f32[2]);
			return (((dx <= Epsilon.vector4_f32[0]) &&
				(dy <= Epsilon.vector4_f32[1]) &&
				(dz <= Epsilon.vector4_f32[2])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4_t vDelta = vsubq_f32(V1, V2);
			uint32x4_t vResult = vacleq_f32(vDelta, Epsilon);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			// Get the difference
			Vector vDelta = _mm_sub_ps(V1, V2);
			// Get the absolute value of the difference
			Vector vTemp = _mm_setzero_ps();
			vTemp = _mm_sub_ps(vTemp, vDelta);
			vTemp = _mm_max_ps(vTemp, vDelta);
			vTemp = _mm_cmple_ps(vTemp, Epsilon);
			// w is don't care
			return (((_mm_movemask_ps(vTemp) & 7) == 0x7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector3NotEqual
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] != V2.vector4_f32[0]) || (V1.vector4_f32[1] != V2.vector4_f32[1]) || (V1.vector4_f32[2] != V2.vector4_f32[2])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vceqq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) != 0xFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpeq_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 7) != 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector3NotEqualInt
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_u32[0] != V2.vector4_u32[0]) || (V1.vector4_u32[1] != V2.vector4_u32[1]) || (V1.vector4_u32[2] != V2.vector4_u32[2])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vceqq_u32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) != 0xFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 7) != 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector3Greater
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] > V2.vector4_f32[0]) && (V1.vector4_f32[1] > V2.vector4_f32[1]) && (V1.vector4_f32[2] > V2.vector4_f32[2])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vcgtq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpgt_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t XM_CALLCONV Vector3GreaterR
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			uint32_t CR = 0;
			if ((V1.vector4_f32[0] > V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] > V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] > V2.vector4_f32[2]))
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] <= V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] <= V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] <= V2.vector4_f32[2]))
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vcgtq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU;

			uint32_t CR = 0;
			if (r == 0xFFFFFFU)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpgt_ps(V1, V2);
			uint32_t CR = 0;
			int iTest = _mm_movemask_ps(vTemp) & 7;
			if (iTest == 7)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector3GreaterOrEqual
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] >= V2.vector4_f32[0]) && (V1.vector4_f32[1] >= V2.vector4_f32[1]) && (V1.vector4_f32[2] >= V2.vector4_f32[2])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vcgeq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpge_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t XM_CALLCONV Vector3GreaterOrEqualR
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			uint32_t CR = 0;
			if ((V1.vector4_f32[0] >= V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] >= V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] >= V2.vector4_f32[2]))
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] < V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] < V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] < V2.vector4_f32[2]))
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vcgeq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU;

			uint32_t CR = 0;
			if (r == 0xFFFFFFU)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpge_ps(V1, V2);
			uint32_t CR = 0;
			int iTest = _mm_movemask_ps(vTemp) & 7;
			if (iTest == 7)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector3Less
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] < V2.vector4_f32[0]) && (V1.vector4_f32[1] < V2.vector4_f32[1]) && (V1.vector4_f32[2] < V2.vector4_f32[2])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vcltq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmplt_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector3LessOrEqual
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] <= V2.vector4_f32[0]) && (V1.vector4_f32[1] <= V2.vector4_f32[1]) && (V1.vector4_f32[2] <= V2.vector4_f32[2])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vcleq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmple_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector3InBounds
		(
			FVector V,
			FVector Bounds
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V.vector4_f32[0] <= Bounds.vector4_f32[0] && V.vector4_f32[0] >= -Bounds.vector4_f32[0]) &&
				(V.vector4_f32[1] <= Bounds.vector4_f32[1] && V.vector4_f32[1] >= -Bounds.vector4_f32[1]) &&
				(V.vector4_f32[2] <= Bounds.vector4_f32[2] && V.vector4_f32[2] >= -Bounds.vector4_f32[2])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Test if less than or equal
			uint32x4_t ivTemp1 = vcleq_f32(V, Bounds);
			// Negate the bounds
			float32x4_t vTemp2 = vnegq_f32(Bounds);
			// Test if greater or equal (Reversed)
			uint32x4_t ivTemp2 = vcleq_f32(vTemp2, V);
			// Blend answers
			ivTemp1 = vandq_u32(ivTemp1, ivTemp2);
			// in bounds?
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(ivTemp1), vget_high_u8(ivTemp1));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			// Test if less than or equal
			Vector vTemp1 = _mm_cmple_ps(V, Bounds);
			// Negate the bounds
			Vector vTemp2 = _mm_mul_ps(Bounds, g_XMNegativeOne);
			// Test if greater or equal (Reversed)
			vTemp2 = _mm_cmple_ps(vTemp2, V);
			// Blend answers
			vTemp1 = _mm_and_ps(vTemp1, vTemp2);
			// x,y and z in bounds? (w is don't care)
			return (((_mm_movemask_ps(vTemp1) & 0x7) == 0x7) != 0);
#else
			return ComparisonAllInBounds(Vector3InBoundsR(V, Bounds));
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector3IsNaN
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			return (XMISNAN(V.vector4_f32[0]) ||
				XMISNAN(V.vector4_f32[1]) ||
				XMISNAN(V.vector4_f32[2]));

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Test against itself. NaN is always not equal
			uint32x4_t vTempNan = vceqq_f32(V, V);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTempNan), vget_high_u8(vTempNan));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			// If x or y or z are NaN, the mask is zero
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) != 0xFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			// Test against itself. NaN is always not equal
			Vector vTempNan = _mm_cmpneq_ps(V, V);
			// If x or y or z are NaN, the mask is non-zero
			return ((_mm_movemask_ps(vTempNan) & 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector3IsInfinite
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (XMISINF(V.vector4_f32[0]) ||
				XMISINF(V.vector4_f32[1]) ||
				XMISINF(V.vector4_f32[2]));
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Mask off the sign bit
			uint32x4_t vTempInf = vandq_u32(V, g_XMAbsMask);
			// Compare to infinity
			vTempInf = vceqq_f32(vTempInf, g_XMInfinity);
			// If any are infinity, the signs are true.
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTempInf), vget_high_u8(vTempInf));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) != 0);
#elif defined(_XM_SSE_INTRINSICS_)
			// Mask off the sign bit
			__m128 vTemp = _mm_and_ps(V, g_XMAbsMask);
			// Compare to infinity
			vTemp = _mm_cmpeq_ps(vTemp, g_XMInfinity);
			// If x,y or z are infinity, the signs are true.
			return ((_mm_movemask_ps(vTemp) & 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3Dot
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			float fValue = V1.vector4_f32[0] * V2.vector4_f32[0] + V1.vector4_f32[1] * V2.vector4_f32[1] + V1.vector4_f32[2] * V2.vector4_f32[2];
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = fValue;
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4_t vTemp = vmulq_f32(V1, V2);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			return vcombine_f32(v1, v1);
#elif defined(_XM_SSE4_INTRINSICS_)
			return _mm_dp_ps(V1, V2, 0x7f);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vTemp = _mm_mul_ps(V1, V2);
			vTemp = _mm_and_ps(vTemp, g_XMMask3);
			vTemp = _mm_hadd_ps(vTemp, vTemp);
			return _mm_hadd_ps(vTemp, vTemp);
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product
			Vector vDot = _mm_mul_ps(V1, V2);
			// x=Dot.vector4_f32[1], y=Dot.vector4_f32[2]
			Vector vTemp = XM_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
			// Result.vector4_f32[0] = x+y
			vDot = _mm_add_ss(vDot, vTemp);
			// x=Dot.vector4_f32[2]
			vTemp = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			// Result.vector4_f32[0] = (x+y)+z
			vDot = _mm_add_ss(vDot, vTemp);
			// Splat x
			return XM_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3Cross
		(
			FVector V1,
			FVector V2
		)
		{
			// [ V1.y*V2.z - V1.z*V2.y, V1.z*V2.x - V1.x*V2.z, V1.x*V2.y - V1.y*V2.x ]

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 vResult = { { {
					(V1.vector4_f32[1] * V2.vector4_f32[2]) - (V1.vector4_f32[2] * V2.vector4_f32[1]),
					(V1.vector4_f32[2] * V2.vector4_f32[0]) - (V1.vector4_f32[0] * V2.vector4_f32[2]),
					(V1.vector4_f32[0] * V2.vector4_f32[1]) - (V1.vector4_f32[1] * V2.vector4_f32[0]),
					0.0f
				} } };
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t v1xy = vget_low_f32(V1);
			float32x2_t v2xy = vget_low_f32(V2);

			float32x2_t v1yx = vrev64_f32(v1xy);
			float32x2_t v2yx = vrev64_f32(v2xy);

			float32x2_t v1zz = vdup_lane_f32(vget_high_f32(V1), 0);
			float32x2_t v2zz = vdup_lane_f32(vget_high_f32(V2), 0);

			Vector vResult = vmulq_f32(vcombine_f32(v1yx, v1xy), vcombine_f32(v2zz, v2yx));
			vResult = vmlsq_f32(vResult, vcombine_f32(v1zz, v1yx), vcombine_f32(v2yx, v2xy));
			vResult = veorq_u32(vResult, g_XMFlipY);
			return vandq_u32(vResult, g_XMMask3);
#elif defined(_XM_SSE_INTRINSICS_)
	// y1,z1,x1,w1
			Vector vTemp1 = XM_PERMUTE_PS(V1, _MM_SHUFFLE(3, 0, 2, 1));
			// z2,x2,y2,w2
			Vector vTemp2 = XM_PERMUTE_PS(V2, _MM_SHUFFLE(3, 1, 0, 2));
			// Perform the left operation
			Vector vResult = _mm_mul_ps(vTemp1, vTemp2);
			// z1,x1,y1,w1
			vTemp1 = XM_PERMUTE_PS(vTemp1, _MM_SHUFFLE(3, 0, 2, 1));
			// y2,z2,x2,w2
			vTemp2 = XM_PERMUTE_PS(vTemp2, _MM_SHUFFLE(3, 1, 0, 2));
			// Perform the right operation
			vTemp1 = _mm_mul_ps(vTemp1, vTemp2);
			// Subract the right from left, and return answer
			vResult = _mm_sub_ps(vResult, vTemp1);
			// Set w to zero
			return _mm_and_ps(vResult, g_XMMask3);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3LengthSq
		(
			FVector V
		)
		{
			return Vector3Dot(V, V);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3ReciprocalLengthEst
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Result;

			Result = Vector3LengthSq(V);
			Result = VectorReciprocalSqrtEst(Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Dot3
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			// Reciprocal sqrt (estimate)
			v2 = vrsqrte_f32(v1);
			return vcombine_f32(v2, v2);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(V, V, 0x7f);
			return _mm_rsqrt_ps(vTemp);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_and_ps(vLengthSq, g_XMMask3);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_rsqrt_ps(vLengthSq);
			return vLengthSq;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x,y and z
			Vector vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and y
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
			// x+z, y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			// y,y,y,y
			vTemp = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			// x+z+y,??,??,??
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			// Splat the length squared
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			// Get the reciprocal
			vLengthSq = _mm_rsqrt_ps(vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3ReciprocalLength
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Result;

			Result = Vector3LengthSq(V);
			Result = VectorReciprocalSqrt(Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Dot3
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			// Reciprocal sqrt
			float32x2_t  S0 = vrsqrte_f32(v1);
			float32x2_t  P0 = vmul_f32(v1, S0);
			float32x2_t  R0 = vrsqrts_f32(P0, S0);
			float32x2_t  S1 = vmul_f32(S0, R0);
			float32x2_t  P1 = vmul_f32(v1, S1);
			float32x2_t  R1 = vrsqrts_f32(P1, S1);
			float32x2_t Result = vmul_f32(S1, R1);
			return vcombine_f32(Result, Result);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(V, V, 0x7f);
			Vector vLengthSq = _mm_sqrt_ps(vTemp);
			return _mm_div_ps(g_XMOne, vLengthSq);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vDot = _mm_mul_ps(V, V);
			vDot = _mm_and_ps(vDot, g_XMMask3);
			vDot = _mm_hadd_ps(vDot, vDot);
			vDot = _mm_hadd_ps(vDot, vDot);
			vDot = _mm_sqrt_ps(vDot);
			vDot = _mm_div_ps(g_XMOne, vDot);
			return vDot;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product
			Vector vDot = _mm_mul_ps(V, V);
			// x=Dot.y, y=Dot.z
			Vector vTemp = XM_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
			// Result.x = x+y
			vDot = _mm_add_ss(vDot, vTemp);
			// x=Dot.z
			vTemp = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			// Result.x = (x+y)+z
			vDot = _mm_add_ss(vDot, vTemp);
			// Splat x
			vDot = XM_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
			// Get the reciprocal
			vDot = _mm_sqrt_ps(vDot);
			// Get the reciprocal
			vDot = _mm_div_ps(g_XMOne, vDot);
			return vDot;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3LengthEst
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Result;

			Result = Vector3LengthSq(V);
			Result = VectorSqrtEst(Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Dot3
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			const float32x2_t zero = vdup_n_f32(0);
			uint32x2_t VEqualsZero = vceq_f32(v1, zero);
			// Sqrt (estimate)
			float32x2_t Result = vrsqrte_f32(v1);
			Result = vmul_f32(v1, Result);
			Result = vbsl_f32(VEqualsZero, zero, Result);
			return vcombine_f32(Result, Result);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(V, V, 0x7f);
			return _mm_sqrt_ps(vTemp);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_and_ps(vLengthSq, g_XMMask3);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x,y and z
			Vector vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and y
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
			// x+z, y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			// y,y,y,y
			vTemp = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			// x+z+y,??,??,??
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			// Splat the length squared
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			// Get the length
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3Length
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Result;

			Result = Vector3LengthSq(V);
			Result = VectorSqrt(Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Dot3
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			const float32x2_t zero = vdup_n_f32(0);
			uint32x2_t VEqualsZero = vceq_f32(v1, zero);
			// Sqrt
			float32x2_t S0 = vrsqrte_f32(v1);
			float32x2_t P0 = vmul_f32(v1, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(v1, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			float32x2_t Result = vmul_f32(S1, R1);
			Result = vmul_f32(v1, Result);
			Result = vbsl_f32(VEqualsZero, zero, Result);
			return vcombine_f32(Result, Result);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(V, V, 0x7f);
			return _mm_sqrt_ps(vTemp);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_and_ps(vLengthSq, g_XMMask3);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x,y and z
			Vector vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and y
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
			// x+z, y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			// y,y,y,y
			vTemp = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			// x+z+y,??,??,??
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			// Splat the length squared
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			// Get the length
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------
		// Vector3NormalizeEst uses a reciprocal estimate and
		// returns QNaN on zero and infinite vectors.

		inline Vector XM_CALLCONV Vector3NormalizeEst
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Result;
			Result = Vector3ReciprocalLength(V);
			Result = VectorMultiply(V, Result);
			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Dot3
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			// Reciprocal sqrt (estimate)
			v2 = vrsqrte_f32(v1);
			// Normalize
			return vmulq_f32(V, vcombine_f32(v2, v2));
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(V, V, 0x7f);
			Vector vResult = _mm_rsqrt_ps(vTemp);
			return _mm_mul_ps(vResult, V);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vDot = _mm_mul_ps(V, V);
			vDot = _mm_and_ps(vDot, g_XMMask3);
			vDot = _mm_hadd_ps(vDot, vDot);
			vDot = _mm_hadd_ps(vDot, vDot);
			vDot = _mm_rsqrt_ps(vDot);
			vDot = _mm_mul_ps(vDot, V);
			return vDot;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product
			Vector vDot = _mm_mul_ps(V, V);
			// x=Dot.y, y=Dot.z
			Vector vTemp = XM_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
			// Result.x = x+y
			vDot = _mm_add_ss(vDot, vTemp);
			// x=Dot.z
			vTemp = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			// Result.x = (x+y)+z
			vDot = _mm_add_ss(vDot, vTemp);
			// Splat x
			vDot = XM_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
			// Get the reciprocal
			vDot = _mm_rsqrt_ps(vDot);
			// Perform the normalization
			vDot = _mm_mul_ps(vDot, V);
			return vDot;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3Normalize
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			float fLength;
			Vector vResult;

			vResult = Vector3Length(V);
			fLength = vResult.vector4_f32[0];

			// Prevent divide by zero
			if (fLength > 0) {
				fLength = 1.0f / fLength;
			}

			vResult.vector4_f32[0] = V.vector4_f32[0] * fLength;
			vResult.vector4_f32[1] = V.vector4_f32[1] * fLength;
			vResult.vector4_f32[2] = V.vector4_f32[2] * fLength;
			vResult.vector4_f32[3] = V.vector4_f32[3] * fLength;
			return vResult;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Dot3
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			uint32x2_t VEqualsZero = vceq_f32(v1, vdup_n_f32(0));
			uint32x2_t VEqualsInf = vceq_f32(v1, vget_low_f32(g_XMInfinity));
			// Reciprocal sqrt (2 iterations of Newton-Raphson)
			float32x2_t S0 = vrsqrte_f32(v1);
			float32x2_t P0 = vmul_f32(v1, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(v1, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			v2 = vmul_f32(S1, R1);
			// Normalize
			Vector vResult = vmulq_f32(V, vcombine_f32(v2, v2));
			vResult = vbslq_f32(vcombine_f32(VEqualsZero, VEqualsZero), vdupq_n_f32(0), vResult);
			return vbslq_f32(vcombine_f32(VEqualsInf, VEqualsInf), g_XMQNaN, vResult);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vLengthSq = _mm_dp_ps(V, V, 0x7f);
			// Prepare for the division
			Vector vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			Vector vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_XMInfinity);
			// Divide to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			Vector vTemp1 = _mm_andnot_ps(vLengthSq, g_XMQNaN);
			Vector vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#elif defined(_XM_SSE3_INTRINSICS_)
			// Perform the dot product on x,y and z only
			Vector vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_and_ps(vLengthSq, g_XMMask3);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			// Prepare for the division
			Vector vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			Vector vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_XMInfinity);
			// Divide to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			Vector vTemp1 = _mm_andnot_ps(vLengthSq, g_XMQNaN);
			Vector vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x,y and z only
			Vector vLengthSq = _mm_mul_ps(V, V);
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 1, 2, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vTemp = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			// Prepare for the division
			Vector vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			Vector vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_XMInfinity);
			// Divide to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			Vector vTemp1 = _mm_andnot_ps(vLengthSq, g_XMQNaN);
			Vector vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3ClampLength
		(
			FVector V,
			float    LengthMin,
			float    LengthMax
		)
		{
			Vector ClampMax = VectorReplicate(LengthMax);
			Vector ClampMin = VectorReplicate(LengthMin);

			return Vector3ClampLengthV(V, ClampMin, ClampMax);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3ClampLengthV
		(
			FVector V,
			FVector LengthMin,
			FVector LengthMax
		)
		{
			assert((VectorGetY(LengthMin) == VectorGetX(LengthMin)) && (VectorGetZ(LengthMin) == VectorGetX(LengthMin)));
			assert((VectorGetY(LengthMax) == VectorGetX(LengthMax)) && (VectorGetZ(LengthMax) == VectorGetX(LengthMax)));
			assert(Vector3GreaterOrEqual(LengthMin, VectorZero()));
			assert(Vector3GreaterOrEqual(LengthMax, VectorZero()));
			assert(Vector3GreaterOrEqual(LengthMax, LengthMin));

			Vector LengthSq = Vector3LengthSq(V);

			const Vector Zero = VectorZero();

			Vector RcpLength = VectorReciprocalSqrt(LengthSq);

			Vector InfiniteLength = VectorEqualInt(LengthSq, g_XMInfinity.v);
			Vector ZeroLength = VectorEqual(LengthSq, Zero);

			Vector Normal = VectorMultiply(V, RcpLength);

			Vector Length = VectorMultiply(LengthSq, RcpLength);

			Vector Select = VectorEqualInt(InfiniteLength, ZeroLength);
			Length = VectorSelect(LengthSq, Length, Select);
			Normal = VectorSelect(LengthSq, Normal, Select);

			Vector ControlMax = VectorGreater(Length, LengthMax);
			Vector ControlMin = VectorLess(Length, LengthMin);

			Vector ClampLength = VectorSelect(Length, LengthMax, ControlMax);
			ClampLength = VectorSelect(ClampLength, LengthMin, ControlMin);

			Vector Result = VectorMultiply(Normal, ClampLength);

			// Preserve the original vector (with no precision loss) if the length falls within the given range
			Vector Control = VectorEqualInt(ControlMax, ControlMin);
			Result = VectorSelect(Result, V, Control);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3Reflect
		(
			FVector Incident,
			FVector Normal
		)
		{
			// Result = Incident - (2 * dot(Incident, Normal)) * Normal

			Vector Result = Vector3Dot(Incident, Normal);
			Result = VectorAdd(Result, Result);
			Result = VectorNegativeMultiplySubtract(Result, Normal, Incident);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3Refract
		(
			FVector Incident,
			FVector Normal,
			float    RefractionIndex
		)
		{
			Vector Index = VectorReplicate(RefractionIndex);
			return Vector3RefractV(Incident, Normal, Index);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3RefractV
		(
			FVector Incident,
			FVector Normal,
			FVector RefractionIndex
		)
		{
			// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
			// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))

#if defined(_XM_NO_INTRINSICS_)

			const Vector  Zero = VectorZero();

			Vector IDotN = Vector3Dot(Incident, Normal);

			// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			Vector R = VectorNegativeMultiplySubtract(IDotN, IDotN, g_XMOne.v);
			R = VectorMultiply(R, RefractionIndex);
			R = VectorNegativeMultiplySubtract(R, RefractionIndex, g_XMOne.v);

			if (Vector4LessOrEqual(R, Zero))
			{
				// Total internal reflection
				return Zero;
			}
			else
			{
				// R = RefractionIndex * IDotN + sqrt(R)
				R = VectorSqrt(R);
				R = VectorMultiplyAdd(RefractionIndex, IDotN, R);

				// Result = RefractionIndex * Incident - Normal * R
				Vector Result = VectorMultiply(RefractionIndex, Incident);
				Result = VectorNegativeMultiplySubtract(Normal, R, Result);

				return Result;
			}

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Vector IDotN = Vector3Dot(Incident, Normal);

			// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			float32x4_t R = vmlsq_f32(g_XMOne, IDotN, IDotN);
			R = vmulq_f32(R, RefractionIndex);
			R = vmlsq_f32(g_XMOne, R, RefractionIndex);

			uint32x4_t vResult = vcleq_f32(R, g_XMZero);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			if (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU)
			{
				// Total internal reflection
				vResult = g_XMZero;
			}
			else
			{
				// Sqrt(R)
				float32x4_t S0 = vrsqrteq_f32(R);
				float32x4_t P0 = vmulq_f32(R, S0);
				float32x4_t R0 = vrsqrtsq_f32(P0, S0);
				float32x4_t S1 = vmulq_f32(S0, R0);
				float32x4_t P1 = vmulq_f32(R, S1);
				float32x4_t R1 = vrsqrtsq_f32(P1, S1);
				float32x4_t S2 = vmulq_f32(S1, R1);
				R = vmulq_f32(R, S2);
				// R = RefractionIndex * IDotN + sqrt(R)
				R = vmlaq_f32(R, RefractionIndex, IDotN);
				// Result = RefractionIndex * Incident - Normal * R
				vResult = vmulq_f32(RefractionIndex, Incident);
				vResult = vmlsq_f32(vResult, R, Normal);
			}
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
	// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
			Vector IDotN = Vector3Dot(Incident, Normal);
			// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			Vector R = _mm_mul_ps(IDotN, IDotN);
			R = _mm_sub_ps(g_XMOne, R);
			R = _mm_mul_ps(R, RefractionIndex);
			R = _mm_mul_ps(R, RefractionIndex);
			R = _mm_sub_ps(g_XMOne, R);

			Vector vResult = _mm_cmple_ps(R, g_XMZero);
			if (_mm_movemask_ps(vResult) == 0x0f)
			{
				// Total internal reflection
				vResult = g_XMZero;
			}
			else
			{
				// R = RefractionIndex * IDotN + sqrt(R)
				R = _mm_sqrt_ps(R);
				vResult = _mm_mul_ps(RefractionIndex, IDotN);
				R = _mm_add_ps(R, vResult);
				// Result = RefractionIndex * Incident - Normal * R
				vResult = _mm_mul_ps(RefractionIndex, Incident);
				R = _mm_mul_ps(R, Normal);
				vResult = _mm_sub_ps(vResult, R);
			}
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3Orthogonal
		(
			FVector V
		)
		{
			Vector Zero = VectorZero();
			Vector Z = VectorSplatZ(V);
			Vector YZYY = VectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(V);

			Vector NegativeV = VectorSubtract(Zero, V);

			Vector ZIsNegative = VectorLess(Z, Zero);
			Vector YZYYIsNegative = VectorLess(YZYY, Zero);

			Vector S = VectorAdd(YZYY, Z);
			Vector D = VectorSubtract(YZYY, Z);

			Vector Select = VectorEqualInt(ZIsNegative, YZYYIsNegative);

			Vector R0 = VectorPermute<XM_PERMUTE_1X, XM_PERMUTE_0X, XM_PERMUTE_0X, XM_PERMUTE_0X>(NegativeV, S);
			Vector R1 = VectorPermute<XM_PERMUTE_1X, XM_PERMUTE_0X, XM_PERMUTE_0X, XM_PERMUTE_0X>(V, D);

			return VectorSelect(R1, R0, Select);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3AngleBetweenNormalsEst
		(
			FVector N1,
			FVector N2
		)
		{
			Vector Result = Vector3Dot(N1, N2);
			Result = VectorClamp(Result, g_XMNegativeOne.v, g_XMOne.v);
			Result = VectorACosEst(Result);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3AngleBetweenNormals
		(
			FVector N1,
			FVector N2
		)
		{
			Vector Result = Vector3Dot(N1, N2);
			Result = VectorClamp(Result, g_XMNegativeOne.v, g_XMOne.v);
			Result = VectorACos(Result);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3AngleBetweenVectors
		(
			FVector V1,
			FVector V2
		)
		{
			Vector L1 = Vector3ReciprocalLength(V1);
			Vector L2 = Vector3ReciprocalLength(V2);

			Vector Dot = Vector3Dot(V1, V2);

			L1 = VectorMultiply(L1, L2);

			Vector CosAngle = VectorMultiply(Dot, L1);
			CosAngle = VectorClamp(CosAngle, g_XMNegativeOne.v, g_XMOne.v);

			return VectorACos(CosAngle);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3LinePointDistance
		(
			FVector LinePoint1,
			FVector LinePoint2,
			FVector Point
		)
		{
			// Given a vector PointVector from LinePoint1 to Point and a vector
			// LineVector from LinePoint1 to LinePoint2, the scaled distance 
			// PointProjectionScale from LinePoint1 to the perpendicular projection
			// of PointVector onto the line is defined as:
			//
			//     PointProjectionScale = dot(PointVector, LineVector) / LengthSq(LineVector)

			Vector PointVector = VectorSubtract(Point, LinePoint1);
			Vector LineVector = VectorSubtract(LinePoint2, LinePoint1);

			Vector LengthSq = Vector3LengthSq(LineVector);

			Vector PointProjectionScale = Vector3Dot(PointVector, LineVector);
			PointProjectionScale = VectorDivide(PointProjectionScale, LengthSq);

			Vector DistanceVector = VectorMultiply(LineVector, PointProjectionScale);
			DistanceVector = VectorSubtract(PointVector, DistanceVector);

			return Vector3Length(DistanceVector);
		}

		//------------------------------------------------------------------------------


		inline void XM_CALLCONV Vector3ComponentsFromNormal
		(
			Vector* pParallel,
			Vector* pPerpendicular,
			FVector  V,
			FVector  Normal
		)
		{
			assert(pParallel != nullptr);
			assert(pPerpendicular != nullptr);

			Vector Scale = Vector3Dot(V, Normal);

			Vector Parallel = VectorMultiply(Normal, Scale);

			*pParallel = Parallel;
			*pPerpendicular = VectorSubtract(V, Parallel);
		}

		//------------------------------------------------------------------------------
		// Transform a vector using a rotation expressed as a unit quaternion

		inline Vector XM_CALLCONV Vector3Rotate
		(
			FVector V,
			FVector RotationQuaternion
		)
		{
			Vector A = VectorSelect(g_XMSelect1110.v, V, g_XMSelect1110.v);
			Vector Q = QuaternionConjugate(RotationQuaternion);
			Vector Result = QuaternionMultiply(Q, A);
			return QuaternionMultiply(Result, RotationQuaternion);
		}

		//------------------------------------------------------------------------------
		// Transform a vector using the inverse of a rotation expressed as a unit quaternion

		inline Vector XM_CALLCONV Vector3InverseRotate
		(
			FVector V,
			FVector RotationQuaternion
		)
		{
			Vector A = VectorSelect(g_XMSelect1110.v, V, g_XMSelect1110.v);
			Vector Result = QuaternionMultiply(RotationQuaternion, A);
			Vector Q = QuaternionConjugate(RotationQuaternion);
			return QuaternionMultiply(Result, Q);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3Transform
		(
			FVector V,
			FMatrix M
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Z = VectorSplatZ(V);
			Vector Y = VectorSplatY(V);
			Vector X = VectorSplatX(V);

			Vector Result = VectorMultiplyAdd(Z, M.r[2], M.r[3]);
			Result = VectorMultiplyAdd(Y, M.r[1], Result);
			Result = VectorMultiplyAdd(X, M.r[0], Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			Vector vResult = vmlaq_lane_f32(M.r[3], M.r[0], VL, 0); // X
			vResult = vmlaq_lane_f32(vResult, M.r[1], VL, 1); // Y
			return vmlaq_lane_f32(vResult, M.r[2], vget_high_f32(V), 0); // Z
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
			vResult = _mm_mul_ps(vResult, M.r[0]);
			Vector vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			vTemp = _mm_mul_ps(vTemp, M.r[1]);
			vResult = _mm_add_ps(vResult, vTemp);
			vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			vTemp = _mm_mul_ps(vTemp, M.r[2]);
			vResult = _mm_add_ps(vResult, vTemp);
			vResult = _mm_add_ps(vResult, M.r[3]);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015 26019, "PREfast noise: Esp:1307" )
#endif


		inline Float4* XM_CALLCONV Vector3TransformStream
		(
			Float4* pOutputStream,
			size_t          OutputStride,
			const Float3* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			FMatrix       M
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float3));
			_Analysis_assume_(InputStride >= sizeof(Float3));

			assert(OutputStride >= sizeof(Float4));
			_Analysis_assume_(OutputStride >= sizeof(Float4));

#if defined(_XM_NO_INTRINSICS_)

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row2 = M.r[2];
			const Vector row3 = M.r[3];

			for (size_t i = 0; i < VectorCount; i++)
			{
				Vector V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
				Vector Z = VectorSplatZ(V);
				Vector Y = VectorSplatY(V);
				Vector X = VectorSplatX(V);

				Vector Result = VectorMultiplyAdd(Z, row2, row3);
				Result = VectorMultiplyAdd(Y, row1, Result);
				Result = VectorMultiplyAdd(X, row0, Result);

				StoreFloat4(reinterpret_cast<Float4*>(pOutputVector), Result);

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row2 = M.r[2];
			const Vector row3 = M.r[3];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float3)) && (OutputStride == sizeof(Float4)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x3_t V = vld3q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float3) * 4;

						float32x2_t r3 = vget_low_f32(row3);
						float32x2_t r = vget_low_f32(row0);
						Vector vResult0 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Ax+M
						Vector vResult1 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Bx+N

						__prefetch(pInputVector);

						r3 = vget_high_f32(row3);
						r = vget_high_f32(row0);
						Vector vResult2 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Cx+O
						Vector vResult3 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Dx+P

						__prefetch(pInputVector + XM_CACHE_LINE_SIZE);

						r = vget_low_f32(row1);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy+N

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(row1);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[1], r, 0); // Cx+Gy+O
						vResult3 = vmlaq_lane_f32(vResult3, V.val[1], r, 1); // Dx+Hy+P

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 3));

						r = vget_low_f32(row2);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[2], r, 0); // Ax+Ey+Iz+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[2], r, 1); // Bx+Fy+Jz+N

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 4));

						r = vget_high_f32(row2);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[2], r, 0); // Cx+Gy+Kz+O
						vResult3 = vmlaq_lane_f32(vResult3, V.val[2], r, 1); // Dx+Hy+Lz+P

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 5));

						float32x4x4_t R;
						R.val[0] = vResult0;
						R.val[1] = vResult1;
						R.val[2] = vResult2;
						R.val[3] = vResult3;

						vst4q_f32(reinterpret_cast<float*>(pOutputVector), R);
						pOutputVector += sizeof(Float4) * 4;

						i += 4;
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				float32x2_t VL = vld1_f32(reinterpret_cast<const float*>(pInputVector));
				float32x2_t zero = vdup_n_f32(0);
				float32x2_t VH = vld1_lane_f32(reinterpret_cast<const float*>(pInputVector) + 2, zero, 0);
				pInputVector += InputStride;

				Vector vResult = vmlaq_lane_f32(row3, row0, VL, 0); // X
				vResult = vmlaq_lane_f32(vResult, row1, VL, 1); // Y
				vResult = vmlaq_lane_f32(vResult, row2, VH, 0); // Z

				vst1q_f32(reinterpret_cast<float*>(pOutputVector), vResult);
				pOutputVector += OutputStride;
			}

			return pOutputStream;
#elif defined(_XM_SSE_INTRINSICS_)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row2 = M.r[2];
			const Vector row3 = M.r[3];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if (InputStride == sizeof(Float3))
				{
					if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF) && !(OutputStride & 0xF))
					{
						// Packed input, aligned output
						for (size_t j = 0; j < four; ++j)
						{
							__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
							__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
							pInputVector += sizeof(Float3) * 4;

							// Unpack the 4 vectors (.w components are junk)
							XM3UNPACK3INTO4(V1, L2, L3);

							// Result 1
							Vector Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
							Vector Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
							Vector X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

							Vector vTemp = _mm_mul_ps(Z, row2);
							Vector vTemp2 = _mm_mul_ps(Y, row1);
							Vector vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 2
							Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 3
							Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 4
							Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 4;
						}
					}
					else
					{
						// Packed input, unaligned output
						for (size_t j = 0; j < four; ++j)
						{
							__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
							__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
							pInputVector += sizeof(Float3) * 4;

							// Unpack the 4 vectors (.w components are junk)
							XM3UNPACK3INTO4(V1, L2, L3);

							// Result 1
							Vector Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
							Vector Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
							Vector X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

							Vector vTemp = _mm_mul_ps(Z, row2);
							Vector vTemp2 = _mm_mul_ps(Y, row1);
							Vector vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 2
							Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 3
							Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 4
							Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 4;
						}
					}
				}
			}

			if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF) && !(OutputStride & 0xF))
			{
				// Aligned output
				for (; i < VectorCount; ++i)
				{
					Vector V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
					pInputVector += InputStride;

					Vector Z = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
					Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
					Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

					Vector vTemp = _mm_mul_ps(Z, row2);
					Vector vTemp2 = _mm_mul_ps(Y, row1);
					Vector vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);

					XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
				}
			}
			else
			{
				// Unaligned output
				for (; i < VectorCount; ++i)
				{
					Vector V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
					pInputVector += InputStride;

					Vector Z = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
					Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
					Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

					Vector vTemp = _mm_mul_ps(Z, row2);
					Vector vTemp2 = _mm_mul_ps(Y, row1);
					Vector vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);

					_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
				}
			}

			XM_SFENCE();

			return pOutputStream;
#endif
		}

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3TransformCoord
		(
			FVector V,
			FMatrix M
		)
		{
			Vector Z = VectorSplatZ(V);
			Vector Y = VectorSplatY(V);
			Vector X = VectorSplatX(V);

			Vector Result = VectorMultiplyAdd(Z, M.r[2], M.r[3]);
			Result = VectorMultiplyAdd(Y, M.r[1], Result);
			Result = VectorMultiplyAdd(X, M.r[0], Result);

			Vector W = VectorSplatW(Result);
			return VectorDivide(Result, W);
		}

		//------------------------------------------------------------------------------

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015 26019, "PREfast noise: Esp:1307" )
#endif


		inline Float3* XM_CALLCONV Vector3TransformCoordStream
		(
			Float3* pOutputStream,
			size_t          OutputStride,
			const Float3* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			FMatrix       M
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float3));
			_Analysis_assume_(InputStride >= sizeof(Float3));

			assert(OutputStride >= sizeof(Float3));
			_Analysis_assume_(OutputStride >= sizeof(Float3));

#if defined(_XM_NO_INTRINSICS_)

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row2 = M.r[2];
			const Vector row3 = M.r[3];

			for (size_t i = 0; i < VectorCount; i++)
			{
				Vector V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
				Vector Z = VectorSplatZ(V);
				Vector Y = VectorSplatY(V);
				Vector X = VectorSplatX(V);

				Vector Result = VectorMultiplyAdd(Z, row2, row3);
				Result = VectorMultiplyAdd(Y, row1, Result);
				Result = VectorMultiplyAdd(X, row0, Result);

				Vector W = VectorSplatW(Result);

				Result = VectorDivide(Result, W);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), Result);

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row2 = M.r[2];
			const Vector row3 = M.r[3];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float3)) && (OutputStride == sizeof(Float3)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x3_t V = vld3q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float3) * 4;

						float32x2_t r3 = vget_low_f32(row3);
						float32x2_t r = vget_low_f32(row0);
						Vector vResult0 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Ax+M
						Vector vResult1 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Bx+N

						__prefetch(pInputVector);

						r3 = vget_high_f32(row3);
						r = vget_high_f32(row0);
						Vector vResult2 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Cx+O
						Vector W = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Dx+P

						__prefetch(pInputVector + XM_CACHE_LINE_SIZE);

						r = vget_low_f32(row1);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy+N

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(row1);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[1], r, 0); // Cx+Gy+O
						W = vmlaq_lane_f32(W, V.val[1], r, 1); // Dx+Hy+P

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 3));

						r = vget_low_f32(row2);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[2], r, 0); // Ax+Ey+Iz+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[2], r, 1); // Bx+Fy+Jz+N

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 4));

						r = vget_high_f32(row2);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[2], r, 0); // Cx+Gy+Kz+O
						W = vmlaq_lane_f32(W, V.val[2], r, 1); // Dx+Hy+Lz+P

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 5));

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
						V.val[0] = vdivq_f32(vResult0, W);
						V.val[1] = vdivq_f32(vResult1, W);
						V.val[2] = vdivq_f32(vResult2, W);
#else
						// 2 iterations of Newton-Raphson refinement of reciprocal
						float32x4_t Reciprocal = vrecpeq_f32(W);
						float32x4_t S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);
						S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);

						V.val[0] = vmulq_f32(vResult0, Reciprocal);
						V.val[1] = vmulq_f32(vResult1, Reciprocal);
						V.val[2] = vmulq_f32(vResult2, Reciprocal);
#endif

						vst3q_f32(reinterpret_cast<float*>(pOutputVector), V);
						pOutputVector += sizeof(Float3) * 4;

						i += 4;
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				float32x2_t VL = vld1_f32(reinterpret_cast<const float*>(pInputVector));
				float32x2_t zero = vdup_n_f32(0);
				float32x2_t VH = vld1_lane_f32(reinterpret_cast<const float*>(pInputVector) + 2, zero, 0);
				pInputVector += InputStride;

				Vector vResult = vmlaq_lane_f32(row3, row0, VL, 0); // X
				vResult = vmlaq_lane_f32(vResult, row1, VL, 1); // Y
				vResult = vmlaq_lane_f32(vResult, row2, VH, 0); // Z

				VH = vget_high_f32(vResult);
				Vector W = vdupq_lane_f32(VH, 1);

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
				vResult = vdivq_f32(vResult, W);
#else
				// 2 iterations of Newton-Raphson refinement of reciprocal for W
				float32x4_t Reciprocal = vrecpeq_f32(W);
				float32x4_t S = vrecpsq_f32(Reciprocal, W);
				Reciprocal = vmulq_f32(S, Reciprocal);
				S = vrecpsq_f32(Reciprocal, W);
				Reciprocal = vmulq_f32(S, Reciprocal);

				vResult = vmulq_f32(vResult, Reciprocal);
#endif

				VL = vget_low_f32(vResult);
				vst1_f32(reinterpret_cast<float*>(pOutputVector), VL);
				vst1q_lane_f32(reinterpret_cast<float*>(pOutputVector) + 2, vResult, 2);
				pOutputVector += OutputStride;
			}

			return pOutputStream;
#elif defined(_XM_SSE_INTRINSICS_)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row2 = M.r[2];
			const Vector row3 = M.r[3];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if (InputStride == sizeof(Float3))
				{
					if (OutputStride == sizeof(Float3))
					{
						if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF))
						{
							// Packed input, aligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								XM3UNPACK3INTO4(V1, L2, L3);

								// Result 1
								Vector Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								Vector Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								Vector X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								Vector vTemp = _mm_mul_ps(Z, row2);
								Vector vTemp2 = _mm_mul_ps(Y, row1);
								Vector vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V1 = _mm_div_ps(vTemp, W);

								// Result 2
								Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V2 = _mm_div_ps(vTemp, W);

								// Result 3
								Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V3 = _mm_div_ps(vTemp, W);

								// Result 4
								Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V4 = _mm_div_ps(vTemp, W);

								// Pack and store the vectors
								XM3PACK4INTO3(vTemp);
								XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), V1);
								XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
						else
						{
							// Packed input, unaligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								XM3UNPACK3INTO4(V1, L2, L3);

								// Result 1
								Vector Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								Vector Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								Vector X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								Vector vTemp = _mm_mul_ps(Z, row2);
								Vector vTemp2 = _mm_mul_ps(Y, row1);
								Vector vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V1 = _mm_div_ps(vTemp, W);

								// Result 2
								Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V2 = _mm_div_ps(vTemp, W);

								// Result 3
								Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V3 = _mm_div_ps(vTemp, W);

								// Result 4
								Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V4 = _mm_div_ps(vTemp, W);

								// Pack and store the vectors
								XM3PACK4INTO3(vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), V1);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
					}
					else
					{
						// Packed input, unpacked output
						for (size_t j = 0; j < four; ++j)
						{
							__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
							__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
							pInputVector += sizeof(Float3) * 4;

							// Unpack the 4 vectors (.w components are junk)
							XM3UNPACK3INTO4(V1, L2, L3);

							// Result 1
							Vector Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
							Vector Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
							Vector X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

							Vector vTemp = _mm_mul_ps(Z, row2);
							Vector vTemp2 = _mm_mul_ps(Y, row1);
							Vector vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

							vTemp = _mm_div_ps(vTemp, W);
							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 2
							Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

							vTemp = _mm_div_ps(vTemp, W);
							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 3
							Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

							vTemp = _mm_div_ps(vTemp, W);
							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 4
							Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

							vTemp = _mm_div_ps(vTemp, W);
							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 4;
						}
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				Vector V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
				pInputVector += InputStride;

				Vector Z = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
				Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
				Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

				Vector vTemp = _mm_mul_ps(Z, row2);
				Vector vTemp2 = _mm_mul_ps(Y, row1);
				Vector vTemp3 = _mm_mul_ps(X, row0);
				vTemp = _mm_add_ps(vTemp, row3);
				vTemp = _mm_add_ps(vTemp, vTemp2);
				vTemp = _mm_add_ps(vTemp, vTemp3);

				Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

				vTemp = _mm_div_ps(vTemp, W);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
				pOutputVector += OutputStride;
			}

			XM_SFENCE();

			return pOutputStream;
#endif
		}

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3TransformNormal
		(
			FVector V,
			FMatrix M
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Z = VectorSplatZ(V);
			Vector Y = VectorSplatY(V);
			Vector X = VectorSplatX(V);

			Vector Result = VectorMultiply(Z, M.r[2]);
			Result = VectorMultiplyAdd(Y, M.r[1], Result);
			Result = VectorMultiplyAdd(X, M.r[0], Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			Vector vResult = vmulq_lane_f32(M.r[0], VL, 0); // X
			vResult = vmlaq_lane_f32(vResult, M.r[1], VL, 1); // Y
			return vmlaq_lane_f32(vResult, M.r[2], vget_high_f32(V), 0); // Z
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
			vResult = _mm_mul_ps(vResult, M.r[0]);
			Vector vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			vTemp = _mm_mul_ps(vTemp, M.r[1]);
			vResult = _mm_add_ps(vResult, vTemp);
			vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			vTemp = _mm_mul_ps(vTemp, M.r[2]);
			vResult = _mm_add_ps(vResult, vTemp);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015 26019, "PREfast noise: Esp:1307" )
#endif


		inline Float3* XM_CALLCONV Vector3TransformNormalStream
		(
			Float3* pOutputStream,
			size_t          OutputStride,
			const Float3* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			FMatrix       M
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float3));
			_Analysis_assume_(InputStride >= sizeof(Float3));

			assert(OutputStride >= sizeof(Float3));
			_Analysis_assume_(OutputStride >= sizeof(Float3));

#if defined(_XM_NO_INTRINSICS_)

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row2 = M.r[2];

			for (size_t i = 0; i < VectorCount; i++)
			{
				Vector V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
				Vector Z = VectorSplatZ(V);
				Vector Y = VectorSplatY(V);
				Vector X = VectorSplatX(V);

				Vector Result = VectorMultiply(Z, row2);
				Result = VectorMultiplyAdd(Y, row1, Result);
				Result = VectorMultiplyAdd(X, row0, Result);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), Result);

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row2 = M.r[2];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float3)) && (OutputStride == sizeof(Float3)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x3_t V = vld3q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float3) * 4;

						float32x2_t r = vget_low_f32(row0);
						Vector vResult0 = vmulq_lane_f32(V.val[0], r, 0); // Ax
						Vector vResult1 = vmulq_lane_f32(V.val[0], r, 1); // Bx

						__prefetch(pInputVector);

						r = vget_high_f32(row0);
						Vector vResult2 = vmulq_lane_f32(V.val[0], r, 0); // Cx

						__prefetch(pInputVector + XM_CACHE_LINE_SIZE);

						r = vget_low_f32(row1);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(row1);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[1], r, 0); // Cx+Gy

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 3));

						r = vget_low_f32(row2);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[2], r, 0); // Ax+Ey+Iz
						vResult1 = vmlaq_lane_f32(vResult1, V.val[2], r, 1); // Bx+Fy+Jz

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 4));

						r = vget_high_f32(row2);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[2], r, 0); // Cx+Gy+Kz

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 5));

						V.val[0] = vResult0;
						V.val[1] = vResult1;
						V.val[2] = vResult2;

						vst3q_f32(reinterpret_cast<float*>(pOutputVector), V);
						pOutputVector += sizeof(Float3) * 4;

						i += 4;
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				float32x2_t VL = vld1_f32(reinterpret_cast<const float*>(pInputVector));
				float32x2_t zero = vdup_n_f32(0);
				float32x2_t VH = vld1_lane_f32(reinterpret_cast<const float*>(pInputVector) + 2, zero, 0);
				pInputVector += InputStride;

				Vector vResult = vmulq_lane_f32(row0, VL, 0); // X
				vResult = vmlaq_lane_f32(vResult, row1, VL, 1); // Y
				vResult = vmlaq_lane_f32(vResult, row2, VH, 0); // Z

				VL = vget_low_f32(vResult);
				vst1_f32(reinterpret_cast<float*>(pOutputVector), VL);
				vst1q_lane_f32(reinterpret_cast<float*>(pOutputVector) + 2, vResult, 2);
				pOutputVector += OutputStride;
			}

			return pOutputStream;
#elif defined(_XM_SSE_INTRINSICS_)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row2 = M.r[2];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if (InputStride == sizeof(Float3))
				{
					if (OutputStride == sizeof(Float3))
					{
						if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF))
						{
							// Packed input, aligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								XM3UNPACK3INTO4(V1, L2, L3);

								// Result 1
								Vector Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								Vector Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								Vector X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								Vector vTemp = _mm_mul_ps(Z, row2);
								Vector vTemp2 = _mm_mul_ps(Y, row1);
								Vector vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V1 = _mm_add_ps(vTemp, vTemp3);

								// Result 2
								Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V2 = _mm_add_ps(vTemp, vTemp3);

								// Result 3
								Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V3 = _mm_add_ps(vTemp, vTemp3);

								// Result 4
								Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V4 = _mm_add_ps(vTemp, vTemp3);

								// Pack and store the vectors
								XM3PACK4INTO3(vTemp);
								XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), V1);
								XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
						else
						{
							// Packed input, unaligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								XM3UNPACK3INTO4(V1, L2, L3);

								// Result 1
								Vector Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								Vector Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								Vector X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								Vector vTemp = _mm_mul_ps(Z, row2);
								Vector vTemp2 = _mm_mul_ps(Y, row1);
								Vector vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V1 = _mm_add_ps(vTemp, vTemp3);

								// Result 2
								Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V2 = _mm_add_ps(vTemp, vTemp3);

								// Result 3
								Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V3 = _mm_add_ps(vTemp, vTemp3);

								// Result 4
								Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V4 = _mm_add_ps(vTemp, vTemp3);

								// Pack and store the vectors
								XM3PACK4INTO3(vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), V1);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
					}
					else
					{
						// Packed input, unpacked output
						for (size_t j = 0; j < four; ++j)
						{
							__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
							__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
							pInputVector += sizeof(Float3) * 4;

							// Unpack the 4 vectors (.w components are junk)
							XM3UNPACK3INTO4(V1, L2, L3);

							// Result 1
							Vector Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
							Vector Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
							Vector X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

							Vector vTemp = _mm_mul_ps(Z, row2);
							Vector vTemp2 = _mm_mul_ps(Y, row1);
							Vector vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 2
							Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 3
							Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 4
							Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 4;
						}
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				Vector V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
				pInputVector += InputStride;

				Vector Z = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
				Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
				Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

				Vector vTemp = _mm_mul_ps(Z, row2);
				Vector vTemp2 = _mm_mul_ps(Y, row1);
				Vector vTemp3 = _mm_mul_ps(X, row0);
				vTemp = _mm_add_ps(vTemp, vTemp2);
				vTemp = _mm_add_ps(vTemp, vTemp3);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
				pOutputVector += OutputStride;
			}

			XM_SFENCE();

			return pOutputStream;
#endif
		}

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3Project
		(
			FVector V,
			float    ViewportX,
			float    ViewportY,
			float    ViewportWidth,
			float    ViewportHeight,
			float    ViewportMinZ,
			float    ViewportMaxZ,
			FMatrix Projection,
			CMatrix View,
			CMatrix World
		)
		{
			const float HalfViewportWidth = ViewportWidth * 0.5f;
			const float HalfViewportHeight = ViewportHeight * 0.5f;

			Vector Scale = VectorSet(HalfViewportWidth, -HalfViewportHeight, ViewportMaxZ - ViewportMinZ, 0.0f);
			Vector Offset = VectorSet(ViewportX + HalfViewportWidth, ViewportY + HalfViewportHeight, ViewportMinZ, 0.0f);

			Matrix Transform = MatrixMultiply(World, View);
			Transform = MatrixMultiply(Transform, Projection);

			Vector Result = Vector3TransformCoord(V, Transform);

			Result = VectorMultiplyAdd(Result, Scale, Offset);

			return Result;
		}

		//------------------------------------------------------------------------------

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015 26019, "PREfast noise: Esp:1307" )
#endif


		inline Float3* XM_CALLCONV Vector3ProjectStream
		(
			Float3* pOutputStream,
			size_t          OutputStride,
			const Float3* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			float           ViewportX,
			float           ViewportY,
			float           ViewportWidth,
			float           ViewportHeight,
			float           ViewportMinZ,
			float           ViewportMaxZ,
			FMatrix     Projection,
			CMatrix     View,
			CMatrix     World
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float3));
			_Analysis_assume_(InputStride >= sizeof(Float3));

			assert(OutputStride >= sizeof(Float3));
			_Analysis_assume_(OutputStride >= sizeof(Float3));

#if defined(_XM_NO_INTRINSICS_)

			const float HalfViewportWidth = ViewportWidth * 0.5f;
			const float HalfViewportHeight = ViewportHeight * 0.5f;

			Vector Scale = VectorSet(HalfViewportWidth, -HalfViewportHeight, ViewportMaxZ - ViewportMinZ, 1.0f);
			Vector Offset = VectorSet(ViewportX + HalfViewportWidth, ViewportY + HalfViewportHeight, ViewportMinZ, 0.0f);

			Matrix Transform = MatrixMultiply(World, View);
			Transform = MatrixMultiply(Transform, Projection);

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			for (size_t i = 0; i < VectorCount; i++)
			{
				Vector V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));

				Vector Result = Vector3TransformCoord(V, Transform);
				Result = VectorMultiplyAdd(Result, Scale, Offset);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), Result);

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			const float HalfViewportWidth = ViewportWidth * 0.5f;
			const float HalfViewportHeight = ViewportHeight * 0.5f;

			Matrix Transform = MatrixMultiply(World, View);
			Transform = MatrixMultiply(Transform, Projection);

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float3)) && (OutputStride == sizeof(Float3)))
				{
					Vector ScaleX = vdupq_n_f32(HalfViewportWidth);
					Vector ScaleY = vdupq_n_f32(-HalfViewportHeight);
					Vector ScaleZ = vdupq_n_f32(ViewportMaxZ - ViewportMinZ);

					Vector OffsetX = vdupq_n_f32(ViewportX + HalfViewportWidth);
					Vector OffsetY = vdupq_n_f32(ViewportY + HalfViewportHeight);
					Vector OffsetZ = vdupq_n_f32(ViewportMinZ);

					for (size_t j = 0; j < four; ++j)
					{
						float32x4x3_t V = vld3q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float3) * 4;

						float32x2_t r3 = vget_low_f32(Transform.r[3]);
						float32x2_t r = vget_low_f32(Transform.r[0]);
						Vector vResult0 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Ax+M
						Vector vResult1 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Bx+N

						__prefetch(pInputVector);

						r3 = vget_high_f32(Transform.r[3]);
						r = vget_high_f32(Transform.r[0]);
						Vector vResult2 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Cx+O
						Vector W = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Dx+P

						__prefetch(pInputVector + XM_CACHE_LINE_SIZE);

						r = vget_low_f32(Transform.r[1]);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy+N

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(Transform.r[1]);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[1], r, 0); // Cx+Gy+O
						W = vmlaq_lane_f32(W, V.val[1], r, 1); // Dx+Hy+P

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 3));

						r = vget_low_f32(Transform.r[2]);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[2], r, 0); // Ax+Ey+Iz+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[2], r, 1); // Bx+Fy+Jz+N

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 4));

						r = vget_high_f32(Transform.r[2]);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[2], r, 0); // Cx+Gy+Kz+O
						W = vmlaq_lane_f32(W, V.val[2], r, 1); // Dx+Hy+Lz+P

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 5));

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
						vResult0 = vdivq_f32(vResult0, W);
						vResult1 = vdivq_f32(vResult1, W);
						vResult2 = vdivq_f32(vResult2, W);
#else
						// 2 iterations of Newton-Raphson refinement of reciprocal
						float32x4_t Reciprocal = vrecpeq_f32(W);
						float32x4_t S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);
						S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);

						vResult0 = vmulq_f32(vResult0, Reciprocal);
						vResult1 = vmulq_f32(vResult1, Reciprocal);
						vResult2 = vmulq_f32(vResult2, Reciprocal);
#endif

						V.val[0] = vmlaq_f32(OffsetX, vResult0, ScaleX);
						V.val[1] = vmlaq_f32(OffsetY, vResult1, ScaleY);
						V.val[2] = vmlaq_f32(OffsetZ, vResult2, ScaleZ);

						vst3q_f32(reinterpret_cast<float*>(pOutputVector), V);
						pOutputVector += sizeof(Float3) * 4;

						i += 4;
					}
				}
			}

			if (i < VectorCount)
			{
				Vector Scale = VectorSet(HalfViewportWidth, -HalfViewportHeight, ViewportMaxZ - ViewportMinZ, 1.0f);
				Vector Offset = VectorSet(ViewportX + HalfViewportWidth, ViewportY + HalfViewportHeight, ViewportMinZ, 0.0f);

				for (; i < VectorCount; i++)
				{
					float32x2_t VL = vld1_f32(reinterpret_cast<const float*>(pInputVector));
					float32x2_t zero = vdup_n_f32(0);
					float32x2_t VH = vld1_lane_f32(reinterpret_cast<const float*>(pInputVector) + 2, zero, 0);
					pInputVector += InputStride;

					Vector vResult = vmlaq_lane_f32(Transform.r[3], Transform.r[0], VL, 0); // X
					vResult = vmlaq_lane_f32(vResult, Transform.r[1], VL, 1); // Y
					vResult = vmlaq_lane_f32(vResult, Transform.r[2], VH, 0); // Z

					VH = vget_high_f32(vResult);
					Vector W = vdupq_lane_f32(VH, 1);

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
					vResult = vdivq_f32(vResult, W);
#else
					// 2 iterations of Newton-Raphson refinement of reciprocal for W
					float32x4_t Reciprocal = vrecpeq_f32(W);
					float32x4_t S = vrecpsq_f32(Reciprocal, W);
					Reciprocal = vmulq_f32(S, Reciprocal);
					S = vrecpsq_f32(Reciprocal, W);
					Reciprocal = vmulq_f32(S, Reciprocal);

					vResult = vmulq_f32(vResult, Reciprocal);
#endif

					vResult = vmlaq_f32(Offset, vResult, Scale);

					VL = vget_low_f32(vResult);
					vst1_f32(reinterpret_cast<float*>(pOutputVector), VL);
					vst1q_lane_f32(reinterpret_cast<float*>(pOutputVector) + 2, vResult, 2);
					pOutputVector += OutputStride;
				}
			}

			return pOutputStream;
#elif defined(_XM_SSE_INTRINSICS_)
			const float HalfViewportWidth = ViewportWidth * 0.5f;
			const float HalfViewportHeight = ViewportHeight * 0.5f;

			Vector Scale = VectorSet(HalfViewportWidth, -HalfViewportHeight, ViewportMaxZ - ViewportMinZ, 1.0f);
			Vector Offset = VectorSet(ViewportX + HalfViewportWidth, ViewportY + HalfViewportHeight, ViewportMinZ, 0.0f);

			Matrix Transform = MatrixMultiply(World, View);
			Transform = MatrixMultiply(Transform, Projection);

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if (InputStride == sizeof(Float3))
				{
					if (OutputStride == sizeof(Float3))
					{
						if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF))
						{
							// Packed input, aligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								XM3UNPACK3INTO4(V1, L2, L3);

								// Result 1
								Vector Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								Vector Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								Vector X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								Vector vTemp = _mm_mul_ps(Z, Transform.r[2]);
								Vector vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								Vector vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V1 = _mm_add_ps(vTemp, Offset);

								// Result 2
								Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V2 = _mm_add_ps(vTemp, Offset);

								// Result 3
								Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V3 = _mm_add_ps(vTemp, Offset);

								// Result 4
								Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V4 = _mm_add_ps(vTemp, Offset);

								// Pack and store the vectors
								XM3PACK4INTO3(vTemp);
								XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), V1);
								XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
						else
						{
							// Packed input, unaligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								XM3UNPACK3INTO4(V1, L2, L3);

								// Result 1
								Vector Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								Vector Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								Vector X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								Vector vTemp = _mm_mul_ps(Z, Transform.r[2]);
								Vector vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								Vector vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V1 = _mm_add_ps(vTemp, Offset);

								// Result 2
								Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V2 = _mm_add_ps(vTemp, Offset);

								// Result 3
								Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V3 = _mm_add_ps(vTemp, Offset);

								// Result 4
								Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V4 = _mm_add_ps(vTemp, Offset);

								// Pack and store the vectors
								XM3PACK4INTO3(vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), V1);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
					}
					else
					{
						// Packed input, unpacked output
						for (size_t j = 0; j < four; ++j)
						{
							__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
							__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
							pInputVector += sizeof(Float3) * 4;

							// Unpack the 4 vectors (.w components are junk)
							XM3UNPACK3INTO4(V1, L2, L3);

							// Result 1
							Vector Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
							Vector Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
							Vector X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

							Vector vTemp = _mm_mul_ps(Z, Transform.r[2]);
							Vector vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							Vector vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							vTemp = _mm_mul_ps(vTemp, Scale);
							vTemp = _mm_add_ps(vTemp, Offset);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 2
							Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, Transform.r[2]);
							vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							vTemp = _mm_mul_ps(vTemp, Scale);
							vTemp = _mm_add_ps(vTemp, Offset);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 3
							Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, Transform.r[2]);
							vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							vTemp = _mm_mul_ps(vTemp, Scale);
							vTemp = _mm_add_ps(vTemp, Offset);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 4
							Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, Transform.r[2]);
							vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							vTemp = _mm_mul_ps(vTemp, Scale);
							vTemp = _mm_add_ps(vTemp, Offset);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 4;
						}
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				Vector V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
				pInputVector += InputStride;

				Vector Z = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
				Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
				Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

				Vector vTemp = _mm_mul_ps(Z, Transform.r[2]);
				Vector vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
				Vector vTemp3 = _mm_mul_ps(X, Transform.r[0]);
				vTemp = _mm_add_ps(vTemp, Transform.r[3]);
				vTemp = _mm_add_ps(vTemp, vTemp2);
				vTemp = _mm_add_ps(vTemp, vTemp3);

				Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
				vTemp = _mm_div_ps(vTemp, W);

				vTemp = _mm_mul_ps(vTemp, Scale);
				vTemp = _mm_add_ps(vTemp, Offset);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
				pOutputVector += OutputStride;
			}

			XM_SFENCE();

			return pOutputStream;
#endif
		}

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector3Unproject
		(
			FVector V,
			float     ViewportX,
			float     ViewportY,
			float     ViewportWidth,
			float     ViewportHeight,
			float     ViewportMinZ,
			float     ViewportMaxZ,
			FMatrix Projection,
			CMatrix View,
			CMatrix World
		)
		{
			static const VectorF32 D = { { { -1.0f, 1.0f, 0.0f, 0.0f } } };

			Vector Scale = VectorSet(ViewportWidth * 0.5f, -ViewportHeight * 0.5f, ViewportMaxZ - ViewportMinZ, 1.0f);
			Scale = VectorReciprocal(Scale);

			Vector Offset = VectorSet(-ViewportX, -ViewportY, -ViewportMinZ, 0.0f);
			Offset = VectorMultiplyAdd(Scale, Offset, D.v);

			Matrix Transform = MatrixMultiply(World, View);
			Transform = MatrixMultiply(Transform, Projection);
			Transform = MatrixInverse(nullptr, Transform);

			Vector Result = VectorMultiplyAdd(V, Scale, Offset);

			return Vector3TransformCoord(Result, Transform);
		}

		//------------------------------------------------------------------------------

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015 26019, "PREfast noise: Esp:1307" )
#endif


		inline Float3* XM_CALLCONV Vector3UnprojectStream
		(
			Float3* pOutputStream,
			size_t          OutputStride,
			const Float3* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			float           ViewportX,
			float           ViewportY,
			float           ViewportWidth,
			float           ViewportHeight,
			float           ViewportMinZ,
			float           ViewportMaxZ,
			FMatrix       Projection,
			CMatrix       View,
			CMatrix       World)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float3));
			_Analysis_assume_(InputStride >= sizeof(Float3));

			assert(OutputStride >= sizeof(Float3));
			_Analysis_assume_(OutputStride >= sizeof(Float3));

#if defined(_XM_NO_INTRINSICS_)

			static const VectorF32 D = { { { -1.0f, 1.0f, 0.0f, 0.0f } } };

			Vector Scale = VectorSet(ViewportWidth * 0.5f, -ViewportHeight * 0.5f, ViewportMaxZ - ViewportMinZ, 1.0f);
			Scale = VectorReciprocal(Scale);

			Vector Offset = VectorSet(-ViewportX, -ViewportY, -ViewportMinZ, 0.0f);
			Offset = VectorMultiplyAdd(Scale, Offset, D.v);

			Matrix Transform = MatrixMultiply(World, View);
			Transform = MatrixMultiply(Transform, Projection);
			Transform = MatrixInverse(nullptr, Transform);

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			for (size_t i = 0; i < VectorCount; i++)
			{
				Vector V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));

				Vector Result = VectorMultiplyAdd(V, Scale, Offset);

				Result = Vector3TransformCoord(Result, Transform);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), Result);

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Matrix Transform = MatrixMultiply(World, View);
			Transform = MatrixMultiply(Transform, Projection);
			Transform = MatrixInverse(nullptr, Transform);

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			float sx = 1.f / (ViewportWidth * 0.5f);
			float sy = 1.f / (-ViewportHeight * 0.5f);
			float sz = 1.f / (ViewportMaxZ - ViewportMinZ);

			float ox = (-ViewportX * sx) - 1.f;
			float oy = (-ViewportY * sy) + 1.f;
			float oz = (-ViewportMinZ * sz);

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float3)) && (OutputStride == sizeof(Float3)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x3_t V = vld3q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float3) * 4;

						Vector ScaleX = vdupq_n_f32(sx);
						Vector OffsetX = vdupq_n_f32(ox);
						Vector VX = vmlaq_f32(OffsetX, ScaleX, V.val[0]);

						float32x2_t r3 = vget_low_f32(Transform.r[3]);
						float32x2_t r = vget_low_f32(Transform.r[0]);
						Vector vResult0 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), VX, r, 0); // Ax+M
						Vector vResult1 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), VX, r, 1); // Bx+N

						__prefetch(pInputVector);

						r3 = vget_high_f32(Transform.r[3]);
						r = vget_high_f32(Transform.r[0]);
						Vector vResult2 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), VX, r, 0); // Cx+O
						Vector W = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), VX, r, 1); // Dx+P

						__prefetch(pInputVector + XM_CACHE_LINE_SIZE);

						Vector ScaleY = vdupq_n_f32(sy);
						Vector OffsetY = vdupq_n_f32(oy);
						Vector VY = vmlaq_f32(OffsetY, ScaleY, V.val[1]);

						r = vget_low_f32(Transform.r[1]);
						vResult0 = vmlaq_lane_f32(vResult0, VY, r, 0); // Ax+Ey+M
						vResult1 = vmlaq_lane_f32(vResult1, VY, r, 1); // Bx+Fy+N

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(Transform.r[1]);
						vResult2 = vmlaq_lane_f32(vResult2, VY, r, 0); // Cx+Gy+O
						W = vmlaq_lane_f32(W, VY, r, 1); // Dx+Hy+P

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 3));

						Vector ScaleZ = vdupq_n_f32(sz);
						Vector OffsetZ = vdupq_n_f32(oz);
						Vector VZ = vmlaq_f32(OffsetZ, ScaleZ, V.val[2]);

						r = vget_low_f32(Transform.r[2]);
						vResult0 = vmlaq_lane_f32(vResult0, VZ, r, 0); // Ax+Ey+Iz+M
						vResult1 = vmlaq_lane_f32(vResult1, VZ, r, 1); // Bx+Fy+Jz+N

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 4));

						r = vget_high_f32(Transform.r[2]);
						vResult2 = vmlaq_lane_f32(vResult2, VZ, r, 0); // Cx+Gy+Kz+O
						W = vmlaq_lane_f32(W, VZ, r, 1); // Dx+Hy+Lz+P

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 5));

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
						V.val[0] = vdivq_f32(vResult0, W);
						V.val[1] = vdivq_f32(vResult1, W);
						V.val[2] = vdivq_f32(vResult2, W);
#else
						// 2 iterations of Newton-Raphson refinement of reciprocal
						float32x4_t Reciprocal = vrecpeq_f32(W);
						float32x4_t S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);
						S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);

						V.val[0] = vmulq_f32(vResult0, Reciprocal);
						V.val[1] = vmulq_f32(vResult1, Reciprocal);
						V.val[2] = vmulq_f32(vResult2, Reciprocal);
#endif

						vst3q_f32(reinterpret_cast<float*>(pOutputVector), V);
						pOutputVector += sizeof(Float3) * 4;

						i += 4;
					}
				}
			}

			if (i < VectorCount)
			{
				float32x2_t ScaleL = vcreate_f32(
					static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&sx))
					| (static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&sy)) << 32));
				float32x2_t ScaleH = vcreate_f32(static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&sz)));

				float32x2_t OffsetL = vcreate_f32(
					static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&ox))
					| (static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&oy)) << 32));
				float32x2_t OffsetH = vcreate_f32(static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&oz)));

				for (; i < VectorCount; i++)
				{
					float32x2_t VL = vld1_f32(reinterpret_cast<const float*>(pInputVector));
					float32x2_t zero = vdup_n_f32(0);
					float32x2_t VH = vld1_lane_f32(reinterpret_cast<const float*>(pInputVector) + 2, zero, 0);
					pInputVector += InputStride;

					VL = vmla_f32(OffsetL, VL, ScaleL);
					VH = vmla_f32(OffsetH, VH, ScaleH);

					Vector vResult = vmlaq_lane_f32(Transform.r[3], Transform.r[0], VL, 0); // X
					vResult = vmlaq_lane_f32(vResult, Transform.r[1], VL, 1); // Y
					vResult = vmlaq_lane_f32(vResult, Transform.r[2], VH, 0); // Z

					VH = vget_high_f32(vResult);
					Vector W = vdupq_lane_f32(VH, 1);

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
					vResult = vdivq_f32(vResult, W);
#else
					// 2 iterations of Newton-Raphson refinement of reciprocal for W
					float32x4_t Reciprocal = vrecpeq_f32(W);
					float32x4_t S = vrecpsq_f32(Reciprocal, W);
					Reciprocal = vmulq_f32(S, Reciprocal);
					S = vrecpsq_f32(Reciprocal, W);
					Reciprocal = vmulq_f32(S, Reciprocal);

					vResult = vmulq_f32(vResult, Reciprocal);
#endif

					VL = vget_low_f32(vResult);
					vst1_f32(reinterpret_cast<float*>(pOutputVector), VL);
					vst1q_lane_f32(reinterpret_cast<float*>(pOutputVector) + 2, vResult, 2);
					pOutputVector += OutputStride;
				}
			}

			return pOutputStream;
#elif defined(_XM_SSE_INTRINSICS_)
			static const VectorF32 D = { { { -1.0f, 1.0f, 0.0f, 0.0f } } };

			Vector Scale = VectorSet(ViewportWidth * 0.5f, -ViewportHeight * 0.5f, ViewportMaxZ - ViewportMinZ, 1.0f);
			Scale = VectorReciprocal(Scale);

			Vector Offset = VectorSet(-ViewportX, -ViewportY, -ViewportMinZ, 0.0f);
			Offset = _mm_mul_ps(Scale, Offset);
			Offset = _mm_add_ps(Offset, D);

			Matrix Transform = MatrixMultiply(World, View);
			Transform = MatrixMultiply(Transform, Projection);
			Transform = MatrixInverse(nullptr, Transform);

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if (InputStride == sizeof(Float3))
				{
					if (OutputStride == sizeof(Float3))
					{
						if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF))
						{
							// Packed input, aligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								XM3UNPACK3INTO4(V1, L2, L3);

								// Result 1
								V1 = _mm_mul_ps(V1, Scale);
								V1 = _mm_add_ps(V1, Offset);

								Vector Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								Vector Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								Vector X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								Vector vTemp = _mm_mul_ps(Z, Transform.r[2]);
								Vector vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								Vector vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V1 = _mm_div_ps(vTemp, W);

								// Result 2
								V2 = _mm_mul_ps(V2, Scale);
								V2 = _mm_add_ps(V2, Offset);

								Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V2 = _mm_div_ps(vTemp, W);

								// Result 3
								V3 = _mm_mul_ps(V3, Scale);
								V3 = _mm_add_ps(V3, Offset);

								Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V3 = _mm_div_ps(vTemp, W);

								// Result 4
								V4 = _mm_mul_ps(V4, Scale);
								V4 = _mm_add_ps(V4, Offset);

								Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V4 = _mm_div_ps(vTemp, W);

								// Pack and store the vectors
								XM3PACK4INTO3(vTemp);
								XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), V1);
								XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
						else
						{
							// Packed input, unaligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								XM3UNPACK3INTO4(V1, L2, L3);

								// Result 1
								V1 = _mm_mul_ps(V1, Scale);
								V1 = _mm_add_ps(V1, Offset);

								Vector Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								Vector Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								Vector X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								Vector vTemp = _mm_mul_ps(Z, Transform.r[2]);
								Vector vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								Vector vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V1 = _mm_div_ps(vTemp, W);

								// Result 2
								V2 = _mm_mul_ps(V2, Scale);
								V2 = _mm_add_ps(V2, Offset);

								Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V2 = _mm_div_ps(vTemp, W);

								// Result 3
								V3 = _mm_mul_ps(V3, Scale);
								V3 = _mm_add_ps(V3, Offset);

								Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V3 = _mm_div_ps(vTemp, W);

								// Result 4
								V4 = _mm_mul_ps(V4, Scale);
								V4 = _mm_add_ps(V4, Offset);

								Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V4 = _mm_div_ps(vTemp, W);

								// Pack and store the vectors
								XM3PACK4INTO3(vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), V1);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
					}
					else
					{
						// Packed input, unpacked output
						for (size_t j = 0; j < four; ++j)
						{
							__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
							__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
							pInputVector += sizeof(Float3) * 4;

							// Unpack the 4 vectors (.w components are junk)
							XM3UNPACK3INTO4(V1, L2, L3);

							// Result 1
							V1 = _mm_mul_ps(V1, Scale);
							V1 = _mm_add_ps(V1, Offset);

							Vector Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
							Vector Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
							Vector X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

							Vector vTemp = _mm_mul_ps(Z, Transform.r[2]);
							Vector vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							Vector vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 2
							V2 = _mm_mul_ps(V2, Scale);
							V2 = _mm_add_ps(V2, Offset);

							Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, Transform.r[2]);
							vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 3
							V3 = _mm_mul_ps(V3, Scale);
							V3 = _mm_add_ps(V3, Offset);

							Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, Transform.r[2]);
							vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 4
							V4 = _mm_mul_ps(V4, Scale);
							V4 = _mm_add_ps(V4, Offset);

							Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
							Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
							X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, Transform.r[2]);
							vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 4;
						}
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				Vector V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
				pInputVector += InputStride;

				V = _mm_mul_ps(V, Scale);
				V = _mm_add_ps(V, Offset);

				Vector Z = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
				Vector Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
				Vector X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

				Vector vTemp = _mm_mul_ps(Z, Transform.r[2]);
				Vector vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
				Vector vTemp3 = _mm_mul_ps(X, Transform.r[0]);
				vTemp = _mm_add_ps(vTemp, Transform.r[3]);
				vTemp = _mm_add_ps(vTemp, vTemp2);
				vTemp = _mm_add_ps(vTemp, vTemp3);

				Vector W = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
				vTemp = _mm_div_ps(vTemp, W);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
				pOutputVector += OutputStride;
			}

			XM_SFENCE();

			return pOutputStream;
#endif
		}

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

		/****************************************************************************
		 *
		 * 4D Vector
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------
		 // Comparison operations
		 //------------------------------------------------------------------------------

		 //------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector4Equal
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] == V2.vector4_f32[0]) && (V1.vector4_f32[1] == V2.vector4_f32[1]) && (V1.vector4_f32[2] == V2.vector4_f32[2]) && (V1.vector4_f32[3] == V2.vector4_f32[3])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vceqq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpeq_ps(V1, V2);
			return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#else
			return ComparisonAllTrue(Vector4EqualR(V1, V2));
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t XM_CALLCONV Vector4EqualR
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			uint32_t CR = 0;

			if ((V1.vector4_f32[0] == V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] == V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] == V2.vector4_f32[2]) &&
				(V1.vector4_f32[3] == V2.vector4_f32[3]))
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] != V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] != V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] != V2.vector4_f32[2]) &&
				(V1.vector4_f32[3] != V2.vector4_f32[3]))
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vceqq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);

			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpeq_ps(V1, V2);
			int iTest = _mm_movemask_ps(vTemp);
			uint32_t CR = 0;
			if (iTest == 0xf)     // All equal?
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (iTest == 0)  // All not equal?
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector4EqualInt
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_u32[0] == V2.vector4_u32[0]) && (V1.vector4_u32[1] == V2.vector4_u32[1]) && (V1.vector4_u32[2] == V2.vector4_u32[2]) && (V1.vector4_u32[3] == V2.vector4_u32[3])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vceqq_u32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return ((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) == 0xf) != 0);
#else
			return ComparisonAllTrue(Vector4EqualIntR(V1, V2));
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t XM_CALLCONV Vector4EqualIntR
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			uint32_t CR = 0;
			if (V1.vector4_u32[0] == V2.vector4_u32[0] &&
				V1.vector4_u32[1] == V2.vector4_u32[1] &&
				V1.vector4_u32[2] == V2.vector4_u32[2] &&
				V1.vector4_u32[3] == V2.vector4_u32[3])
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (V1.vector4_u32[0] != V2.vector4_u32[0] &&
				V1.vector4_u32[1] != V2.vector4_u32[1] &&
				V1.vector4_u32[2] != V2.vector4_u32[2] &&
				V1.vector4_u32[3] != V2.vector4_u32[3])
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vceqq_u32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);

			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			int iTest = _mm_movemask_ps(_mm_castsi128_ps(vTemp));
			uint32_t CR = 0;
			if (iTest == 0xf)     // All equal?
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (iTest == 0)  // All not equal?
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		inline bool XM_CALLCONV Vector4NearEqual
		(
			FVector V1,
			FVector V2,
			FVector Epsilon
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			float dx, dy, dz, dw;

			dx = fabsf(V1.vector4_f32[0] - V2.vector4_f32[0]);
			dy = fabsf(V1.vector4_f32[1] - V2.vector4_f32[1]);
			dz = fabsf(V1.vector4_f32[2] - V2.vector4_f32[2]);
			dw = fabsf(V1.vector4_f32[3] - V2.vector4_f32[3]);
			return (((dx <= Epsilon.vector4_f32[0]) &&
				(dy <= Epsilon.vector4_f32[1]) &&
				(dz <= Epsilon.vector4_f32[2]) &&
				(dw <= Epsilon.vector4_f32[3])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4_t vDelta = vsubq_f32(V1, V2);
			uint32x4_t vResult = vacleq_f32(vDelta, Epsilon);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			// Get the difference
			Vector vDelta = _mm_sub_ps(V1, V2);
			// Get the absolute value of the difference
			Vector vTemp = _mm_setzero_ps();
			vTemp = _mm_sub_ps(vTemp, vDelta);
			vTemp = _mm_max_ps(vTemp, vDelta);
			vTemp = _mm_cmple_ps(vTemp, Epsilon);
			return ((_mm_movemask_ps(vTemp) == 0xf) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector4NotEqual
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] != V2.vector4_f32[0]) || (V1.vector4_f32[1] != V2.vector4_f32[1]) || (V1.vector4_f32[2] != V2.vector4_f32[2]) || (V1.vector4_f32[3] != V2.vector4_f32[3])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vceqq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) != 0xFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpneq_ps(V1, V2);
			return ((_mm_movemask_ps(vTemp)) != 0);
#else
			return ComparisonAnyFalse(Vector4EqualR(V1, V2));
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector4NotEqualInt
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_u32[0] != V2.vector4_u32[0]) || (V1.vector4_u32[1] != V2.vector4_u32[1]) || (V1.vector4_u32[2] != V2.vector4_u32[2]) || (V1.vector4_u32[3] != V2.vector4_u32[3])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vceqq_u32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) != 0xFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return ((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) != 0xF) != 0);
#else
			return ComparisonAnyFalse(Vector4EqualIntR(V1, V2));
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector4Greater
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] > V2.vector4_f32[0]) && (V1.vector4_f32[1] > V2.vector4_f32[1]) && (V1.vector4_f32[2] > V2.vector4_f32[2]) && (V1.vector4_f32[3] > V2.vector4_f32[3])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vcgtq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpgt_ps(V1, V2);
			return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#else
			return ComparisonAllTrue(Vector4GreaterR(V1, V2));
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t XM_CALLCONV Vector4GreaterR
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			uint32_t CR = 0;
			if (V1.vector4_f32[0] > V2.vector4_f32[0] &&
				V1.vector4_f32[1] > V2.vector4_f32[1] &&
				V1.vector4_f32[2] > V2.vector4_f32[2] &&
				V1.vector4_f32[3] > V2.vector4_f32[3])
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (V1.vector4_f32[0] <= V2.vector4_f32[0] &&
				V1.vector4_f32[1] <= V2.vector4_f32[1] &&
				V1.vector4_f32[2] <= V2.vector4_f32[2] &&
				V1.vector4_f32[3] <= V2.vector4_f32[3])
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vcgtq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);

			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(_XM_SSE_INTRINSICS_)
			uint32_t CR = 0;
			Vector vTemp = _mm_cmpgt_ps(V1, V2);
			int iTest = _mm_movemask_ps(vTemp);
			if (iTest == 0xf) {
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector4GreaterOrEqual
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] >= V2.vector4_f32[0]) && (V1.vector4_f32[1] >= V2.vector4_f32[1]) && (V1.vector4_f32[2] >= V2.vector4_f32[2]) && (V1.vector4_f32[3] >= V2.vector4_f32[3])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vcgeq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmpge_ps(V1, V2);
			return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#else
			return ComparisonAllTrue(Vector4GreaterOrEqualR(V1, V2));
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t XM_CALLCONV Vector4GreaterOrEqualR
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			uint32_t CR = 0;
			if ((V1.vector4_f32[0] >= V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] >= V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] >= V2.vector4_f32[2]) &&
				(V1.vector4_f32[3] >= V2.vector4_f32[3]))
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] < V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] < V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] < V2.vector4_f32[2]) &&
				(V1.vector4_f32[3] < V2.vector4_f32[3]))
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vcgeq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);

			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(_XM_SSE_INTRINSICS_)
			uint32_t CR = 0;
			Vector vTemp = _mm_cmpge_ps(V1, V2);
			int iTest = _mm_movemask_ps(vTemp);
			if (iTest == 0x0f)
			{
				CR = XM_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = XM_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector4Less
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] < V2.vector4_f32[0]) && (V1.vector4_f32[1] < V2.vector4_f32[1]) && (V1.vector4_f32[2] < V2.vector4_f32[2]) && (V1.vector4_f32[3] < V2.vector4_f32[3])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vcltq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmplt_ps(V1, V2);
			return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#else
			return ComparisonAllTrue(Vector4GreaterR(V2, V1));
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector4LessOrEqual
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V1.vector4_f32[0] <= V2.vector4_f32[0]) && (V1.vector4_f32[1] <= V2.vector4_f32[1]) && (V1.vector4_f32[2] <= V2.vector4_f32[2]) && (V1.vector4_f32[3] <= V2.vector4_f32[3])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			uint32x4_t vResult = vcleq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp = _mm_cmple_ps(V1, V2);
			return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#else
			return ComparisonAllTrue(Vector4GreaterOrEqualR(V2, V1));
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector4InBounds
		(
			FVector V,
			FVector Bounds
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (((V.vector4_f32[0] <= Bounds.vector4_f32[0] && V.vector4_f32[0] >= -Bounds.vector4_f32[0]) &&
				(V.vector4_f32[1] <= Bounds.vector4_f32[1] && V.vector4_f32[1] >= -Bounds.vector4_f32[1]) &&
				(V.vector4_f32[2] <= Bounds.vector4_f32[2] && V.vector4_f32[2] >= -Bounds.vector4_f32[2]) &&
				(V.vector4_f32[3] <= Bounds.vector4_f32[3] && V.vector4_f32[3] >= -Bounds.vector4_f32[3])) != 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Test if less than or equal
			uint32x4_t ivTemp1 = vcleq_f32(V, Bounds);
			// Negate the bounds
			float32x4_t vTemp2 = vnegq_f32(Bounds);
			// Test if greater or equal (Reversed)
			uint32x4_t ivTemp2 = vcleq_f32(vTemp2, V);
			// Blend answers
			ivTemp1 = vandq_u32(ivTemp1, ivTemp2);
			// in bounds?
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(ivTemp1), vget_high_u8(ivTemp1));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			// Test if less than or equal
			Vector vTemp1 = _mm_cmple_ps(V, Bounds);
			// Negate the bounds
			Vector vTemp2 = _mm_mul_ps(Bounds, g_XMNegativeOne);
			// Test if greater or equal (Reversed)
			vTemp2 = _mm_cmple_ps(vTemp2, V);
			// Blend answers
			vTemp1 = _mm_and_ps(vTemp1, vTemp2);
			// All in bounds?
			return ((_mm_movemask_ps(vTemp1) == 0x0f) != 0);
#else
			return ComparisonAllInBounds(Vector4InBoundsR(V, Bounds));
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector4IsNaN
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			return (XMISNAN(V.vector4_f32[0]) ||
				XMISNAN(V.vector4_f32[1]) ||
				XMISNAN(V.vector4_f32[2]) ||
				XMISNAN(V.vector4_f32[3]));
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Test against itself. NaN is always not equal
			uint32x4_t vTempNan = vceqq_f32(V, V);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTempNan), vget_high_u8(vTempNan));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			// If any are NaN, the mask is zero
			return (vget_lane_u32(vTemp.val[1], 1) != 0xFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			// Test against itself. NaN is always not equal
			Vector vTempNan = _mm_cmpneq_ps(V, V);
			// If any are NaN, the mask is non-zero
			return (_mm_movemask_ps(vTempNan) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV Vector4IsInfinite
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			return (XMISINF(V.vector4_f32[0]) ||
				XMISINF(V.vector4_f32[1]) ||
				XMISINF(V.vector4_f32[2]) ||
				XMISINF(V.vector4_f32[3]));

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Mask off the sign bit
			uint32x4_t vTempInf = vandq_u32(V, g_XMAbsMask);
			// Compare to infinity
			vTempInf = vceqq_f32(vTempInf, g_XMInfinity);
			// If any are infinity, the signs are true.
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTempInf), vget_high_u8(vTempInf));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) != 0);
#elif defined(_XM_SSE_INTRINSICS_)
			// Mask off the sign bit
			Vector vTemp = _mm_and_ps(V, g_XMAbsMask);
			// Compare to infinity
			vTemp = _mm_cmpeq_ps(vTemp, g_XMInfinity);
			// If any are infinity, the signs are true.
			return (_mm_movemask_ps(vTemp) != 0);
#endif
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4Dot
		(
			FVector V1,
			FVector V2
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result;
			Result.f[0] =
				Result.f[1] =
				Result.f[2] =
				Result.f[3] = V1.vector4_f32[0] * V2.vector4_f32[0] + V1.vector4_f32[1] * V2.vector4_f32[1] + V1.vector4_f32[2] * V2.vector4_f32[2] + V1.vector4_f32[3] * V2.vector4_f32[3];
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4_t vTemp = vmulq_f32(V1, V2);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			return vcombine_f32(v1, v1);
#elif defined(_XM_SSE4_INTRINSICS_)
			return _mm_dp_ps(V1, V2, 0xff);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vTemp = _mm_mul_ps(V1, V2);
			vTemp = _mm_hadd_ps(vTemp, vTemp);
			return _mm_hadd_ps(vTemp, vTemp);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp2 = V2;
			Vector vTemp = _mm_mul_ps(V1, vTemp2);
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp, _MM_SHUFFLE(1, 0, 0, 0)); // Copy X to the Z position and Y to the W position
			vTemp2 = _mm_add_ps(vTemp2, vTemp);          // Add Z = X+Z; W = Y+W;
			vTemp = _mm_shuffle_ps(vTemp, vTemp2, _MM_SHUFFLE(0, 3, 0, 0));  // Copy W to the Z position
			vTemp = _mm_add_ps(vTemp, vTemp2);           // Add Z and W together
			return XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(2, 2, 2, 2));    // Splat Z and return
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4Cross
		(
			FVector V1,
			FVector V2,
			FVector V3
		)
		{
			// [ ((v2.z*v3.w-v2.w*v3.z)*v1.y)-((v2.y*v3.w-v2.w*v3.y)*v1.z)+((v2.y*v3.z-v2.z*v3.y)*v1.w),
			//   ((v2.w*v3.z-v2.z*v3.w)*v1.x)-((v2.w*v3.x-v2.x*v3.w)*v1.z)+((v2.z*v3.x-v2.x*v3.z)*v1.w),
			//   ((v2.y*v3.w-v2.w*v3.y)*v1.x)-((v2.x*v3.w-v2.w*v3.x)*v1.y)+((v2.x*v3.y-v2.y*v3.x)*v1.w),
			//   ((v2.z*v3.y-v2.y*v3.z)*v1.x)-((v2.z*v3.x-v2.x*v3.z)*v1.y)+((v2.y*v3.x-v2.x*v3.y)*v1.z) ]

#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result = { { {
					(((V2.vector4_f32[2] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[2])) * V1.vector4_f32[1]) - (((V2.vector4_f32[1] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[1])) * V1.vector4_f32[2]) + (((V2.vector4_f32[1] * V3.vector4_f32[2]) - (V2.vector4_f32[2] * V3.vector4_f32[1])) * V1.vector4_f32[3]),
					(((V2.vector4_f32[3] * V3.vector4_f32[2]) - (V2.vector4_f32[2] * V3.vector4_f32[3])) * V1.vector4_f32[0]) - (((V2.vector4_f32[3] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[3])) * V1.vector4_f32[2]) + (((V2.vector4_f32[2] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[2])) * V1.vector4_f32[3]),
					(((V2.vector4_f32[1] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[1])) * V1.vector4_f32[0]) - (((V2.vector4_f32[0] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[0])) * V1.vector4_f32[1]) + (((V2.vector4_f32[0] * V3.vector4_f32[1]) - (V2.vector4_f32[1] * V3.vector4_f32[0])) * V1.vector4_f32[3]),
					(((V2.vector4_f32[2] * V3.vector4_f32[1]) - (V2.vector4_f32[1] * V3.vector4_f32[2])) * V1.vector4_f32[0]) - (((V2.vector4_f32[2] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[2])) * V1.vector4_f32[1]) + (((V2.vector4_f32[1] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[1])) * V1.vector4_f32[2]),
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			const float32x2_t select = vget_low_f32(g_XMMaskX);

			// Term1: V2zwyz * V3wzwy
			const float32x2_t v2xy = vget_low_f32(V2);
			const float32x2_t v2zw = vget_high_f32(V2);
			const float32x2_t v2yx = vrev64_f32(v2xy);
			const float32x2_t v2wz = vrev64_f32(v2zw);
			const float32x2_t v2yz = vbsl_f32(select, v2yx, v2wz);

			const float32x2_t v3zw = vget_high_f32(V3);
			const float32x2_t v3wz = vrev64_f32(v3zw);
			const float32x2_t v3xy = vget_low_f32(V3);
			const float32x2_t v3wy = vbsl_f32(select, v3wz, v3xy);

			float32x4_t vTemp1 = vcombine_f32(v2zw, v2yz);
			float32x4_t vTemp2 = vcombine_f32(v3wz, v3wy);
			Vector vResult = vmulq_f32(vTemp1, vTemp2);

			// - V2wzwy * V3zwyz
			const float32x2_t v2wy = vbsl_f32(select, v2wz, v2xy);

			const float32x2_t v3yx = vrev64_f32(v3xy);
			const float32x2_t v3yz = vbsl_f32(select, v3yx, v3wz);

			vTemp1 = vcombine_f32(v2wz, v2wy);
			vTemp2 = vcombine_f32(v3zw, v3yz);
			vResult = vmlsq_f32(vResult, vTemp1, vTemp2);

			// term1 * V1yxxx
			const float32x2_t v1xy = vget_low_f32(V1);
			const float32x2_t v1yx = vrev64_f32(v1xy);

			vTemp1 = vcombine_f32(v1yx, vdup_lane_f32(v1yx, 1));
			vResult = vmulq_f32(vResult, vTemp1);

			// Term2: V2ywxz * V3wxwx
			const float32x2_t v2yw = vrev64_f32(v2wy);
			const float32x2_t v2xz = vbsl_f32(select, v2xy, v2wz);

			const float32x2_t v3wx = vbsl_f32(select, v3wz, v3yx);

			vTemp1 = vcombine_f32(v2yw, v2xz);
			vTemp2 = vcombine_f32(v3wx, v3wx);
			float32x4_t vTerm = vmulq_f32(vTemp1, vTemp2);

			// - V2wxwx * V3ywxz
			const float32x2_t v2wx = vbsl_f32(select, v2wz, v2yx);

			const float32x2_t v3yw = vrev64_f32(v3wy);
			const float32x2_t v3xz = vbsl_f32(select, v3xy, v3wz);

			vTemp1 = vcombine_f32(v2wx, v2wx);
			vTemp2 = vcombine_f32(v3yw, v3xz);
			vTerm = vmlsq_f32(vTerm, vTemp1, vTemp2);

			// vResult - term2 * V1zzyy
			const float32x2_t v1zw = vget_high_f32(V1);

			vTemp1 = vcombine_f32(vdup_lane_f32(v1zw, 0), vdup_lane_f32(v1yx, 0));
			vResult = vmlsq_f32(vResult, vTerm, vTemp1);

			// Term3: V2yzxy * V3zxyx
			const float32x2_t v3zx = vrev64_f32(v3xz);

			vTemp1 = vcombine_f32(v2yz, v2xy);
			vTemp2 = vcombine_f32(v3zx, v3yx);
			vTerm = vmulq_f32(vTemp1, vTemp2);

			// - V2zxyx * V3yzxy
			const float32x2_t v2zx = vrev64_f32(v2xz);

			vTemp1 = vcombine_f32(v2zx, v2yx);
			vTemp2 = vcombine_f32(v3yz, v3xy);
			vTerm = vmlsq_f32(vTerm, vTemp1, vTemp2);

			// vResult + term3 * V1wwwz
			const float32x2_t v1wz = vrev64_f32(v1zw);

			vTemp1 = vcombine_f32(vdup_lane_f32(v1wz, 0), v1wz);
			return vmlaq_f32(vResult, vTerm, vTemp1);
#elif defined(_XM_SSE_INTRINSICS_)
	// V2zwyz * V3wzwy
			Vector vResult = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 1, 3, 2));
			Vector vTemp3 = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 3, 2, 3));
			vResult = _mm_mul_ps(vResult, vTemp3);
			// - V2wzwy * V3zwyz
			Vector vTemp2 = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 3, 2, 3));
			vTemp3 = XM_PERMUTE_PS(vTemp3, _MM_SHUFFLE(1, 3, 0, 1));
			vTemp2 = _mm_mul_ps(vTemp2, vTemp3);
			vResult = _mm_sub_ps(vResult, vTemp2);
			// term1 * V1yxxx
			Vector vTemp1 = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 1));
			vResult = _mm_mul_ps(vResult, vTemp1);

			// V2ywxz * V3wxwx
			vTemp2 = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 0, 3, 1));
			vTemp3 = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 3, 0, 3));
			vTemp3 = _mm_mul_ps(vTemp3, vTemp2);
			// - V2wxwx * V3ywxz
			vTemp2 = XM_PERMUTE_PS(vTemp2, _MM_SHUFFLE(2, 1, 2, 1));
			vTemp1 = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 0, 3, 1));
			vTemp2 = _mm_mul_ps(vTemp2, vTemp1);
			vTemp3 = _mm_sub_ps(vTemp3, vTemp2);
			// vResult - temp * V1zzyy
			vTemp1 = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 2, 2));
			vTemp1 = _mm_mul_ps(vTemp1, vTemp3);
			vResult = _mm_sub_ps(vResult, vTemp1);

			// V2yzxy * V3zxyx
			vTemp2 = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 0, 2, 1));
			vTemp3 = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 1, 0, 2));
			vTemp3 = _mm_mul_ps(vTemp3, vTemp2);
			// - V2zxyx * V3yzxy
			vTemp2 = XM_PERMUTE_PS(vTemp2, _MM_SHUFFLE(2, 0, 2, 1));
			vTemp1 = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 0, 2, 1));
			vTemp1 = _mm_mul_ps(vTemp1, vTemp2);
			vTemp3 = _mm_sub_ps(vTemp3, vTemp1);
			// vResult + term * V1wwwz
			vTemp1 = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 3, 3, 3));
			vTemp3 = _mm_mul_ps(vTemp3, vTemp1);
			vResult = _mm_add_ps(vResult, vTemp3);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4LengthSq
		(
			FVector V
		)
		{
			return Vector4Dot(V, V);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4ReciprocalLengthEst
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Result;

			Result = Vector4LengthSq(V);
			Result = VectorReciprocalSqrtEst(Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Dot4
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			// Reciprocal sqrt (estimate)
			v2 = vrsqrte_f32(v1);
			return vcombine_f32(v2, v2);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(V, V, 0xff);
			return _mm_rsqrt_ps(vTemp);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_rsqrt_ps(vLengthSq);
			return vLengthSq;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x,y,z and w
			Vector vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and w
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			// x+z, y+w
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// x+z,x+z,x+z,y+w
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			// ??,??,y+w,y+w
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			// ??,??,x+z+y+w,??
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// Splat the length
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			// Get the reciprocal
			vLengthSq = _mm_rsqrt_ps(vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4ReciprocalLength
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Result;

			Result = Vector4LengthSq(V);
			Result = VectorReciprocalSqrt(Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Dot4
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			// Reciprocal sqrt
			float32x2_t  S0 = vrsqrte_f32(v1);
			float32x2_t  P0 = vmul_f32(v1, S0);
			float32x2_t  R0 = vrsqrts_f32(P0, S0);
			float32x2_t  S1 = vmul_f32(S0, R0);
			float32x2_t  P1 = vmul_f32(v1, S1);
			float32x2_t  R1 = vrsqrts_f32(P1, S1);
			float32x2_t Result = vmul_f32(S1, R1);
			return vcombine_f32(Result, Result);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(V, V, 0xff);
			Vector vLengthSq = _mm_sqrt_ps(vTemp);
			return _mm_div_ps(g_XMOne, vLengthSq);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			vLengthSq = _mm_div_ps(g_XMOne, vLengthSq);
			return vLengthSq;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x,y,z and w
			Vector vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and w
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			// x+z, y+w
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// x+z,x+z,x+z,y+w
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			// ??,??,y+w,y+w
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			// ??,??,x+z+y+w,??
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// Splat the length
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			// Get the reciprocal
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			// Accurate!
			vLengthSq = _mm_div_ps(g_XMOne, vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4LengthEst
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Result;

			Result = Vector4LengthSq(V);
			Result = VectorSqrtEst(Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Dot4
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			const float32x2_t zero = vdup_n_f32(0);
			uint32x2_t VEqualsZero = vceq_f32(v1, zero);
			// Sqrt (estimate)
			float32x2_t Result = vrsqrte_f32(v1);
			Result = vmul_f32(v1, Result);
			Result = vbsl_f32(VEqualsZero, zero, Result);
			return vcombine_f32(Result, Result);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(V, V, 0xff);
			return _mm_sqrt_ps(vTemp);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x,y,z and w
			Vector vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and w
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			// x+z, y+w
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// x+z,x+z,x+z,y+w
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			// ??,??,y+w,y+w
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			// ??,??,x+z+y+w,??
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// Splat the length
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			// Get the length
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4Length
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_) 

			Vector Result;

			Result = Vector4LengthSq(V);
			Result = VectorSqrt(Result);

			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Dot4
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			const float32x2_t zero = vdup_n_f32(0);
			uint32x2_t VEqualsZero = vceq_f32(v1, zero);
			// Sqrt
			float32x2_t S0 = vrsqrte_f32(v1);
			float32x2_t P0 = vmul_f32(v1, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(v1, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			float32x2_t Result = vmul_f32(S1, R1);
			Result = vmul_f32(v1, Result);
			Result = vbsl_f32(VEqualsZero, zero, Result);
			return vcombine_f32(Result, Result);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(V, V, 0xff);
			return _mm_sqrt_ps(vTemp);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x,y,z and w
			Vector vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and w
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			// x+z, y+w
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// x+z,x+z,x+z,y+w
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			// ??,??,y+w,y+w
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			// ??,??,x+z+y+w,??
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// Splat the length
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			// Get the length
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------
		// Vector4NormalizeEst uses a reciprocal estimate and
		// returns QNaN on zero and infinite vectors.

		inline Vector XM_CALLCONV Vector4NormalizeEst
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector Result;
			Result = Vector4ReciprocalLength(V);
			Result = VectorMultiply(V, Result);
			return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Dot4
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			// Reciprocal sqrt (estimate)
			v2 = vrsqrte_f32(v1);
			// Normalize
			return vmulq_f32(V, vcombine_f32(v2, v2));
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(V, V, 0xff);
			Vector vResult = _mm_rsqrt_ps(vTemp);
			return _mm_mul_ps(vResult, V);
#elif defined(_XM_SSE3_INTRINSICS_)
			Vector vDot = _mm_mul_ps(V, V);
			vDot = _mm_hadd_ps(vDot, vDot);
			vDot = _mm_hadd_ps(vDot, vDot);
			vDot = _mm_rsqrt_ps(vDot);
			vDot = _mm_mul_ps(vDot, V);
			return vDot;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x,y,z and w
			Vector vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and w
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			// x+z, y+w
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// x+z,x+z,x+z,y+w
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			// ??,??,y+w,y+w
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			// ??,??,x+z+y+w,??
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// Splat the length
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			// Get the reciprocal
			Vector vResult = _mm_rsqrt_ps(vLengthSq);
			// Reciprocal mul to perform the normalization
			vResult = _mm_mul_ps(vResult, V);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4Normalize
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			float fLength;
			Vector vResult;

			vResult = Vector4Length(V);
			fLength = vResult.vector4_f32[0];

			// Prevent divide by zero
			if (fLength > 0) {
				fLength = 1.0f / fLength;
			}

			vResult.vector4_f32[0] = V.vector4_f32[0] * fLength;
			vResult.vector4_f32[1] = V.vector4_f32[1] * fLength;
			vResult.vector4_f32[2] = V.vector4_f32[2] * fLength;
			vResult.vector4_f32[3] = V.vector4_f32[3] * fLength;
			return vResult;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Dot4
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			uint32x2_t VEqualsZero = vceq_f32(v1, vdup_n_f32(0));
			uint32x2_t VEqualsInf = vceq_f32(v1, vget_low_f32(g_XMInfinity));
			// Reciprocal sqrt (2 iterations of Newton-Raphson)
			float32x2_t S0 = vrsqrte_f32(v1);
			float32x2_t P0 = vmul_f32(v1, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(v1, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			v2 = vmul_f32(S1, R1);
			// Normalize
			Vector vResult = vmulq_f32(V, vcombine_f32(v2, v2));
			vResult = vbslq_f32(vcombine_f32(VEqualsZero, VEqualsZero), vdupq_n_f32(0), vResult);
			return vbslq_f32(vcombine_f32(VEqualsInf, VEqualsInf), g_XMQNaN, vResult);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vLengthSq = _mm_dp_ps(V, V, 0xff);
			// Prepare for the division
			Vector vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			Vector vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_XMInfinity);
			// Divide to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			Vector vTemp1 = _mm_andnot_ps(vLengthSq, g_XMQNaN);
			Vector vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#elif defined(_XM_SSE3_INTRINSICS_)
			// Perform the dot product on x,y,z and w
			Vector vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			// Prepare for the division
			Vector vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			Vector vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_XMInfinity);
			// Divide to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			Vector vTemp1 = _mm_andnot_ps(vLengthSq, g_XMQNaN);
			Vector vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x,y,z and w
			Vector vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and w
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			// x+z, y+w
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// x+z,x+z,x+z,y+w
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			// ??,??,y+w,y+w
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			// ??,??,x+z+y+w,??
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// Splat the length
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			// Prepare for the division
			Vector vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			Vector vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_XMInfinity);
			// Divide to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			Vector vTemp1 = _mm_andnot_ps(vLengthSq, g_XMQNaN);
			Vector vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4ClampLength
		(
			FVector V,
			float    LengthMin,
			float    LengthMax
		)
		{
			Vector ClampMax = VectorReplicate(LengthMax);
			Vector ClampMin = VectorReplicate(LengthMin);

			return Vector4ClampLengthV(V, ClampMin, ClampMax);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4ClampLengthV
		(
			FVector V,
			FVector LengthMin,
			FVector LengthMax
		)
		{
			assert((VectorGetY(LengthMin) == VectorGetX(LengthMin)) && (VectorGetZ(LengthMin) == VectorGetX(LengthMin)) && (VectorGetW(LengthMin) == VectorGetX(LengthMin)));
			assert((VectorGetY(LengthMax) == VectorGetX(LengthMax)) && (VectorGetZ(LengthMax) == VectorGetX(LengthMax)) && (VectorGetW(LengthMax) == VectorGetX(LengthMax)));
			assert(Vector4GreaterOrEqual(LengthMin, VectorZero()));
			assert(Vector4GreaterOrEqual(LengthMax, VectorZero()));
			assert(Vector4GreaterOrEqual(LengthMax, LengthMin));

			Vector LengthSq = Vector4LengthSq(V);

			const Vector Zero = VectorZero();

			Vector RcpLength = VectorReciprocalSqrt(LengthSq);

			Vector InfiniteLength = VectorEqualInt(LengthSq, g_XMInfinity.v);
			Vector ZeroLength = VectorEqual(LengthSq, Zero);

			Vector Normal = VectorMultiply(V, RcpLength);

			Vector Length = VectorMultiply(LengthSq, RcpLength);

			Vector Select = VectorEqualInt(InfiniteLength, ZeroLength);
			Length = VectorSelect(LengthSq, Length, Select);
			Normal = VectorSelect(LengthSq, Normal, Select);

			Vector ControlMax = VectorGreater(Length, LengthMax);
			Vector ControlMin = VectorLess(Length, LengthMin);

			Vector ClampLength = VectorSelect(Length, LengthMax, ControlMax);
			ClampLength = VectorSelect(ClampLength, LengthMin, ControlMin);

			Vector Result = VectorMultiply(Normal, ClampLength);

			// Preserve the original vector (with no precision loss) if the length falls within the given range
			Vector Control = VectorEqualInt(ControlMax, ControlMin);
			Result = VectorSelect(Result, V, Control);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4Reflect
		(
			FVector Incident,
			FVector Normal
		)
		{
			// Result = Incident - (2 * dot(Incident, Normal)) * Normal

			Vector Result = Vector4Dot(Incident, Normal);
			Result = VectorAdd(Result, Result);
			Result = VectorNegativeMultiplySubtract(Result, Normal, Incident);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4Refract
		(
			FVector Incident,
			FVector Normal,
			float    RefractionIndex
		)
		{
			Vector Index = VectorReplicate(RefractionIndex);
			return Vector4RefractV(Incident, Normal, Index);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4RefractV
		(
			FVector Incident,
			FVector Normal,
			FVector RefractionIndex
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Vector        IDotN;
			Vector        R;
			const Vector  Zero = VectorZero();

			// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
			// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))

			IDotN = Vector4Dot(Incident, Normal);

			// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			R = VectorNegativeMultiplySubtract(IDotN, IDotN, g_XMOne.v);
			R = VectorMultiply(R, RefractionIndex);
			R = VectorNegativeMultiplySubtract(R, RefractionIndex, g_XMOne.v);

			if (Vector4LessOrEqual(R, Zero))
			{
				// Total internal reflection
				return Zero;
			}
			else
			{
				Vector Result;

				// R = RefractionIndex * IDotN + sqrt(R)
				R = VectorSqrt(R);
				R = VectorMultiplyAdd(RefractionIndex, IDotN, R);

				// Result = RefractionIndex * Incident - Normal * R
				Result = VectorMultiply(RefractionIndex, Incident);
				Result = VectorNegativeMultiplySubtract(Normal, R, Result);

				return Result;
			}

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Vector IDotN = Vector4Dot(Incident, Normal);

			// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			float32x4_t R = vmlsq_f32(g_XMOne, IDotN, IDotN);
			R = vmulq_f32(R, RefractionIndex);
			R = vmlsq_f32(g_XMOne, R, RefractionIndex);

			uint32x4_t vResult = vcleq_f32(R, g_XMZero);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			if (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU)
			{
				// Total internal reflection
				vResult = g_XMZero;
			}
			else
			{
				// Sqrt(R)
				float32x4_t S0 = vrsqrteq_f32(R);
				float32x4_t P0 = vmulq_f32(R, S0);
				float32x4_t R0 = vrsqrtsq_f32(P0, S0);
				float32x4_t S1 = vmulq_f32(S0, R0);
				float32x4_t P1 = vmulq_f32(R, S1);
				float32x4_t R1 = vrsqrtsq_f32(P1, S1);
				float32x4_t S2 = vmulq_f32(S1, R1);
				R = vmulq_f32(R, S2);
				// R = RefractionIndex * IDotN + sqrt(R)
				R = vmlaq_f32(R, RefractionIndex, IDotN);
				// Result = RefractionIndex * Incident - Normal * R
				vResult = vmulq_f32(RefractionIndex, Incident);
				vResult = vmlsq_f32(vResult, R, Normal);
			}
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			Vector IDotN = Vector4Dot(Incident, Normal);

			// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			Vector R = _mm_mul_ps(IDotN, IDotN);
			R = _mm_sub_ps(g_XMOne, R);
			R = _mm_mul_ps(R, RefractionIndex);
			R = _mm_mul_ps(R, RefractionIndex);
			R = _mm_sub_ps(g_XMOne, R);

			Vector vResult = _mm_cmple_ps(R, g_XMZero);
			if (_mm_movemask_ps(vResult) == 0x0f)
			{
				// Total internal reflection
				vResult = g_XMZero;
			}
			else
			{
				// R = RefractionIndex * IDotN + sqrt(R)
				R = _mm_sqrt_ps(R);
				vResult = _mm_mul_ps(RefractionIndex, IDotN);
				R = _mm_add_ps(R, vResult);
				// Result = RefractionIndex * Incident - Normal * R
				vResult = _mm_mul_ps(RefractionIndex, Incident);
				R = _mm_mul_ps(R, Normal);
				vResult = _mm_sub_ps(vResult, R);
			}
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4Orthogonal
		(
			FVector V
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 Result = { { {
					V.vector4_f32[2],
					V.vector4_f32[3],
					-V.vector4_f32[0],
					-V.vector4_f32[1]
				} } };
			return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			static const VectorF32 Negate = { { { 1.f, 1.f, -1.f, -1.f } } };

			float32x4_t Result = vcombine_f32(vget_high_f32(V), vget_low_f32(V));
			return vmulq_f32(Result, Negate);
#elif defined(_XM_SSE_INTRINSICS_)
			static const VectorF32 FlipZW = { { { 1.0f, 1.0f, -1.0f, -1.0f } } };
			Vector vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 0, 3, 2));
			vResult = _mm_mul_ps(vResult, FlipZW);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4AngleBetweenNormalsEst
		(
			FVector N1,
			FVector N2
		)
		{
			Vector Result = Vector4Dot(N1, N2);
			Result = VectorClamp(Result, g_XMNegativeOne.v, g_XMOne.v);
			Result = VectorACosEst(Result);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4AngleBetweenNormals
		(
			FVector N1,
			FVector N2
		)
		{
			Vector Result = Vector4Dot(N1, N2);
			Result = VectorClamp(Result, g_XMNegativeOne.v, g_XMOne.v);
			Result = VectorACos(Result);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4AngleBetweenVectors
		(
			FVector V1,
			FVector V2
		)
		{
			Vector L1 = Vector4ReciprocalLength(V1);
			Vector L2 = Vector4ReciprocalLength(V2);

			Vector Dot = Vector4Dot(V1, V2);

			L1 = VectorMultiply(L1, L2);

			Vector CosAngle = VectorMultiply(Dot, L1);
			CosAngle = VectorClamp(CosAngle, g_XMNegativeOne.v, g_XMOne.v);

			return VectorACos(CosAngle);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV Vector4Transform
		(
			FVector V,
			FMatrix M
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			float fX = (M.m[0][0] * V.vector4_f32[0]) + (M.m[1][0] * V.vector4_f32[1]) + (M.m[2][0] * V.vector4_f32[2]) + (M.m[3][0] * V.vector4_f32[3]);
			float fY = (M.m[0][1] * V.vector4_f32[0]) + (M.m[1][1] * V.vector4_f32[1]) + (M.m[2][1] * V.vector4_f32[2]) + (M.m[3][1] * V.vector4_f32[3]);
			float fZ = (M.m[0][2] * V.vector4_f32[0]) + (M.m[1][2] * V.vector4_f32[1]) + (M.m[2][2] * V.vector4_f32[2]) + (M.m[3][2] * V.vector4_f32[3]);
			float fW = (M.m[0][3] * V.vector4_f32[0]) + (M.m[1][3] * V.vector4_f32[1]) + (M.m[2][3] * V.vector4_f32[2]) + (M.m[3][3] * V.vector4_f32[3]);
			VectorF32 vResult = { { { fX, fY, fZ, fW } } };
			return vResult.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(V);
			Vector vResult = vmulq_lane_f32(M.r[0], VL, 0); // X
			vResult = vmlaq_lane_f32(vResult, M.r[1], VL, 1); // Y
			float32x2_t VH = vget_high_f32(V);
			vResult = vmlaq_lane_f32(vResult, M.r[2], VH, 0); // Z
			return vmlaq_lane_f32(vResult, M.r[3], VH, 1); // W
#elif defined(_XM_SSE_INTRINSICS_)
			// Splat x,y,z and w
			Vector vTempX = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
			Vector vTempY = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			Vector vTempZ = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			Vector vTempW = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
			// Mul by the matrix
			vTempX = _mm_mul_ps(vTempX, M.r[0]);
			vTempY = _mm_mul_ps(vTempY, M.r[1]);
			vTempZ = _mm_mul_ps(vTempZ, M.r[2]);
			vTempW = _mm_mul_ps(vTempW, M.r[3]);
			// Add them all together
			vTempX = _mm_add_ps(vTempX, vTempY);
			vTempZ = _mm_add_ps(vTempZ, vTempW);
			vTempX = _mm_add_ps(vTempX, vTempZ);
			return vTempX;
#endif
		}

		//------------------------------------------------------------------------------

		inline Float4* XM_CALLCONV Vector4TransformStream
		(
			Float4* pOutputStream,
			size_t          OutputStride,
			const Float4* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			FMatrix       M
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float4));
			_Analysis_assume_(InputStride >= sizeof(Float4));

			assert(OutputStride >= sizeof(Float4));
			_Analysis_assume_(OutputStride >= sizeof(Float4));

#if defined(_XM_NO_INTRINSICS_)

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row2 = M.r[2];
			const Vector row3 = M.r[3];

			for (size_t i = 0; i < VectorCount; i++)
			{
				Vector V = LoadFloat4(reinterpret_cast<const Float4*>(pInputVector));
				Vector W = VectorSplatW(V);
				Vector Z = VectorSplatZ(V);
				Vector Y = VectorSplatY(V);
				Vector X = VectorSplatX(V);

				Vector Result = VectorMultiply(W, row3);
				Result = VectorMultiplyAdd(Z, row2, Result);
				Result = VectorMultiplyAdd(Y, row1, Result);
				Result = VectorMultiplyAdd(X, row0, Result);

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015, "PREfast noise: Esp:1307" )
#endif

				StoreFloat4(reinterpret_cast<Float4*>(pOutputVector), Result);

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row2 = M.r[2];
			const Vector row3 = M.r[3];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float4)) && (OutputStride == sizeof(Float4)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x4_t V = vld4q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float4) * 4;

						float32x2_t r = vget_low_f32(row0);
						Vector vResult0 = vmulq_lane_f32(V.val[0], r, 0); // Ax
						Vector vResult1 = vmulq_lane_f32(V.val[0], r, 1); // Bx

						__prefetch(pInputVector);

						r = vget_high_f32(row0);
						Vector vResult2 = vmulq_lane_f32(V.val[0], r, 0); // Cx
						Vector vResult3 = vmulq_lane_f32(V.val[0], r, 1); // Dx

						__prefetch(pInputVector + XM_CACHE_LINE_SIZE);

						r = vget_low_f32(row1);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(row1);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[1], r, 0); // Cx+Gy
						vResult3 = vmlaq_lane_f32(vResult3, V.val[1], r, 1); // Dx+Hy

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 3));

						r = vget_low_f32(row2);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[2], r, 0); // Ax+Ey+Iz
						vResult1 = vmlaq_lane_f32(vResult1, V.val[2], r, 1); // Bx+Fy+Jz

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 4));

						r = vget_high_f32(row2);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[2], r, 0); // Cx+Gy+Kz
						vResult3 = vmlaq_lane_f32(vResult3, V.val[2], r, 1); // Dx+Hy+Lz

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 5));

						r = vget_low_f32(row3);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[3], r, 0); // Ax+Ey+Iz+Mw
						vResult1 = vmlaq_lane_f32(vResult1, V.val[3], r, 1); // Bx+Fy+Jz+Nw

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 6));

						r = vget_high_f32(row3);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[3], r, 0); // Cx+Gy+Kz+Ow
						vResult3 = vmlaq_lane_f32(vResult3, V.val[3], r, 1); // Dx+Hy+Lz+Pw

						__prefetch(pInputVector + (XM_CACHE_LINE_SIZE * 7));

						V.val[0] = vResult0;
						V.val[1] = vResult1;
						V.val[2] = vResult2;
						V.val[3] = vResult3;

						vst4q_f32(reinterpret_cast<float*>(pOutputVector), V);
						pOutputVector += sizeof(Float4) * 4;

						i += 4;
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				Vector V = vld1q_f32(reinterpret_cast<const float*>(pInputVector));
				pInputVector += InputStride;

				float32x2_t VL = vget_low_f32(V);
				Vector vResult = vmulq_lane_f32(row0, VL, 0); // X
				vResult = vmlaq_lane_f32(vResult, row1, VL, 1); // Y
				float32x2_t VH = vget_high_f32(V);
				vResult = vmlaq_lane_f32(vResult, row2, VH, 0); // Z
				vResult = vmlaq_lane_f32(vResult, row3, VH, 1); // W

				vst1q_f32(reinterpret_cast<float*>(pOutputVector), vResult);
				pOutputVector += OutputStride;
			}

			return pOutputStream;
#elif defined(_XM_SSE_INTRINSICS_)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const Vector row0 = M.r[0];
			const Vector row1 = M.r[1];
			const Vector row2 = M.r[2];
			const Vector row3 = M.r[3];

			if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF) && !(OutputStride & 0xF))
			{
				if (!(reinterpret_cast<uintptr_t>(pInputStream) & 0xF) && !(InputStride & 0xF))
				{
					// Aligned input, aligned output
					for (size_t i = 0; i < VectorCount; i++)
					{
						__m128 V = _mm_load_ps(reinterpret_cast<const float*>(pInputVector));
						pInputVector += InputStride;

						Vector vTempX = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
						Vector vTempY = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						Vector vTempZ = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
						Vector vTempW = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));

						vTempX = _mm_mul_ps(vTempX, row0);
						vTempY = _mm_mul_ps(vTempY, row1);
						vTempZ = _mm_mul_ps(vTempZ, row2);
						vTempW = _mm_mul_ps(vTempW, row3);

						vTempX = _mm_add_ps(vTempX, vTempY);
						vTempZ = _mm_add_ps(vTempZ, vTempW);
						vTempX = _mm_add_ps(vTempX, vTempZ);

						XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTempX);
						pOutputVector += OutputStride;
					}
				}
				else
				{
					// Unaligned input, aligned output
					for (size_t i = 0; i < VectorCount; i++)
					{
						__m128 V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
						pInputVector += InputStride;

						Vector vTempX = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
						Vector vTempY = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						Vector vTempZ = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
						Vector vTempW = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));

						vTempX = _mm_mul_ps(vTempX, row0);
						vTempY = _mm_mul_ps(vTempY, row1);
						vTempZ = _mm_mul_ps(vTempZ, row2);
						vTempW = _mm_mul_ps(vTempW, row3);

						vTempX = _mm_add_ps(vTempX, vTempY);
						vTempZ = _mm_add_ps(vTempZ, vTempW);
						vTempX = _mm_add_ps(vTempX, vTempZ);

						XM_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTempX);
						pOutputVector += OutputStride;
					}
				}
			}
			else
			{
				if (!(reinterpret_cast<uintptr_t>(pInputStream) & 0xF) && !(InputStride & 0xF))
				{
					// Aligned input, unaligned output
					for (size_t i = 0; i < VectorCount; i++)
					{
						__m128 V = _mm_load_ps(reinterpret_cast<const float*>(pInputVector));
						pInputVector += InputStride;

						Vector vTempX = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
						Vector vTempY = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						Vector vTempZ = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
						Vector vTempW = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));

						vTempX = _mm_mul_ps(vTempX, row0);
						vTempY = _mm_mul_ps(vTempY, row1);
						vTempZ = _mm_mul_ps(vTempZ, row2);
						vTempW = _mm_mul_ps(vTempW, row3);

						vTempX = _mm_add_ps(vTempX, vTempY);
						vTempZ = _mm_add_ps(vTempZ, vTempW);
						vTempX = _mm_add_ps(vTempX, vTempZ);

						_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTempX);
						pOutputVector += OutputStride;
					}
				}
				else
				{
					// Unaligned input, unaligned output
					for (size_t i = 0; i < VectorCount; i++)
					{
						__m128 V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
						pInputVector += InputStride;

						Vector vTempX = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
						Vector vTempY = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						Vector vTempZ = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
						Vector vTempW = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));

						vTempX = _mm_mul_ps(vTempX, row0);
						vTempY = _mm_mul_ps(vTempY, row1);
						vTempZ = _mm_mul_ps(vTempZ, row2);
						vTempW = _mm_mul_ps(vTempW, row3);

						vTempX = _mm_add_ps(vTempX, vTempY);
						vTempZ = _mm_add_ps(vTempZ, vTempW);
						vTempX = _mm_add_ps(vTempX, vTempZ);

						_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTempX);
						pOutputVector += OutputStride;
					}
				}
			}

			XM_SFENCE();

			return pOutputStream;
#endif
		}

		/****************************************************************************
		 *
		 * Vector operators
		 *
		 ****************************************************************************/

#ifndef _XM_NO_Vector_OVERLOADS_

		 //------------------------------------------------------------------------------

		inline Vector XM_CALLCONV operator+ (FVector V)
		{
			return V;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV operator- (FVector V)
		{
			return VectorNegate(V);
		}

		//------------------------------------------------------------------------------

		inline Vector& XM_CALLCONV operator+=
			(
				Vector& V1,
				FVector       V2
				)
		{
			V1 = VectorAdd(V1, V2);
			return V1;
		}

		//------------------------------------------------------------------------------

		inline Vector& XM_CALLCONV operator-=
			(
				Vector& V1,
				FVector       V2
				)
		{
			V1 = VectorSubtract(V1, V2);
			return V1;
		}

		//------------------------------------------------------------------------------

		inline Vector& XM_CALLCONV operator*=
			(
				Vector& V1,
				FVector       V2
				)
		{
			V1 = VectorMultiply(V1, V2);
			return V1;
		}

		//------------------------------------------------------------------------------

		inline Vector& XM_CALLCONV operator/=
			(
				Vector& V1,
				FVector       V2
				)
		{
			V1 = VectorDivide(V1, V2);
			return V1;
		}

		//------------------------------------------------------------------------------

		inline Vector& operator*=
			(
				Vector& V,
				const float S
				)
		{
			V = VectorScale(V, S);
			return V;
		}

		//------------------------------------------------------------------------------

		inline Vector& operator/=
			(
				Vector& V,
				const float S
				)
		{
			Vector vS = VectorReplicate(S);
			V = VectorDivide(V, vS);
			return V;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV operator+
			(
				FVector V1,
				FVector V2
				)
		{
			return VectorAdd(V1, V2);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV operator-
			(
				FVector V1,
				FVector V2
				)
		{
			return VectorSubtract(V1, V2);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV operator*
			(
				FVector V1,
				FVector V2
				)
		{
			return VectorMultiply(V1, V2);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV operator/
			(
				FVector V1,
				FVector V2
				)
		{
			return VectorDivide(V1, V2);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV operator*
			(
				FVector      V,
				const float    S
				)
		{
			return VectorScale(V, S);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV operator/
			(
				FVector      V,
				const float    S
				)
		{
			Vector vS = VectorReplicate(S);
			return VectorDivide(V, vS);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV operator*
			(
				float           S,
				FVector  	    V
				)
		{
			return VectorScale(V, S);
		}

#endif /* !_XM_NO_Vector_OVERLOADS_ */

#if defined(_XM_NO_INTRINSICS_)
#undef XMISNAN
#undef XMISINF
#endif

#if defined(_XM_SSE_INTRINSICS_)
#undef XM3UNPACK3INTO4
#undef XM3PACK4INTO3
#endif



		//-------------------------------------------------------------------------------------
	// DirectXMathMatrix.inl -- SIMD C++ Math library
	//
	// Copyright (c) Microsoft Corporation. All rights reserved.
	// Licensed under the MIT License.
	//
	// http://go.microsoft.com/fwlink/?LinkID=615560
	//-------------------------------------------------------------------------------------


	/****************************************************************************
	 *
	 * Matrix
	 *
	 ****************************************************************************/

	 //------------------------------------------------------------------------------
	 // Comparison operations
	 //------------------------------------------------------------------------------

	 //------------------------------------------------------------------------------

	 // Return true if any entry in the matrix is NaN
		inline bool XM_CALLCONV MatrixIsNaN
		(
			FMatrix M
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			size_t i = 16;
			auto pWork = reinterpret_cast<const uint32_t*>(&M.m[0][0]);
			do {
				// Fetch value into integer unit
				uint32_t uTest = pWork[0];
				// Remove sign
				uTest &= 0x7FFFFFFFU;
				// NaN is 0x7F800001 through 0x7FFFFFFF inclusive
				uTest -= 0x7F800001U;
				if (uTest < 0x007FFFFFU) {
					break;      // NaN found
				}
				++pWork;        // Next entry
			} while (--i);
			return (i != 0);      // i == 0 if nothing matched
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Load in registers
			Vector vX = M.r[0];
			Vector vY = M.r[1];
			Vector vZ = M.r[2];
			Vector vW = M.r[3];
			// Test themselves to check for NaN
			vX = vmvnq_u32(vceqq_f32(vX, vX));
			vY = vmvnq_u32(vceqq_f32(vY, vY));
			vZ = vmvnq_u32(vceqq_f32(vZ, vZ));
			vW = vmvnq_u32(vceqq_f32(vW, vW));
			// Or all the results
			vX = vorrq_u32(vX, vZ);
			vY = vorrq_u32(vY, vW);
			vX = vorrq_u32(vX, vY);
			// If any tested true, return true
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vX), vget_high_u8(vX));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			return (r != 0);
#elif defined(_XM_SSE_INTRINSICS_)
			// Load in registers
			Vector vX = M.r[0];
			Vector vY = M.r[1];
			Vector vZ = M.r[2];
			Vector vW = M.r[3];
			// Test themselves to check for NaN
			vX = _mm_cmpneq_ps(vX, vX);
			vY = _mm_cmpneq_ps(vY, vY);
			vZ = _mm_cmpneq_ps(vZ, vZ);
			vW = _mm_cmpneq_ps(vW, vW);
			// Or all the results
			vX = _mm_or_ps(vX, vZ);
			vY = _mm_or_ps(vY, vW);
			vX = _mm_or_ps(vX, vY);
			// If any tested true, return true
			return (_mm_movemask_ps(vX) != 0);
#else
#endif
		}

		//------------------------------------------------------------------------------

		// Return true if any entry in the matrix is +/-INF
		inline bool XM_CALLCONV MatrixIsInfinite
		(
			FMatrix M
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			size_t i = 16;
			auto pWork = reinterpret_cast<const uint32_t*>(&M.m[0][0]);
			do {
				// Fetch value into integer unit
				uint32_t uTest = pWork[0];
				// Remove sign
				uTest &= 0x7FFFFFFFU;
				// INF is 0x7F800000
				if (uTest == 0x7F800000U) {
					break;      // INF found
				}
				++pWork;        // Next entry
			} while (--i);
			return (i != 0);      // i == 0 if nothing matched
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			// Mask off the sign bits
			Vector vTemp1 = vandq_u32(M.r[0], g_XMAbsMask);
			Vector vTemp2 = vandq_u32(M.r[1], g_XMAbsMask);
			Vector vTemp3 = vandq_u32(M.r[2], g_XMAbsMask);
			Vector vTemp4 = vandq_u32(M.r[3], g_XMAbsMask);
			// Compare to infinity
			vTemp1 = vceqq_f32(vTemp1, g_XMInfinity);
			vTemp2 = vceqq_f32(vTemp2, g_XMInfinity);
			vTemp3 = vceqq_f32(vTemp3, g_XMInfinity);
			vTemp4 = vceqq_f32(vTemp4, g_XMInfinity);
			// Or the answers together
			vTemp1 = vorrq_u32(vTemp1, vTemp2);
			vTemp3 = vorrq_u32(vTemp3, vTemp4);
			vTemp1 = vorrq_u32(vTemp1, vTemp3);
			// If any are infinity, the signs are true.
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTemp1), vget_high_u8(vTemp1));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			return (r != 0);
#elif defined(_XM_SSE_INTRINSICS_)
			// Mask off the sign bits
			Vector vTemp1 = _mm_and_ps(M.r[0], g_XMAbsMask);
			Vector vTemp2 = _mm_and_ps(M.r[1], g_XMAbsMask);
			Vector vTemp3 = _mm_and_ps(M.r[2], g_XMAbsMask);
			Vector vTemp4 = _mm_and_ps(M.r[3], g_XMAbsMask);
			// Compare to infinity
			vTemp1 = _mm_cmpeq_ps(vTemp1, g_XMInfinity);
			vTemp2 = _mm_cmpeq_ps(vTemp2, g_XMInfinity);
			vTemp3 = _mm_cmpeq_ps(vTemp3, g_XMInfinity);
			vTemp4 = _mm_cmpeq_ps(vTemp4, g_XMInfinity);
			// Or the answers together
			vTemp1 = _mm_or_ps(vTemp1, vTemp2);
			vTemp3 = _mm_or_ps(vTemp3, vTemp4);
			vTemp1 = _mm_or_ps(vTemp1, vTemp3);
			// If any are infinity, the signs are true.
			return (_mm_movemask_ps(vTemp1) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		// Return true if the Matrix is equal to identity
		inline bool XM_CALLCONV MatrixIsIdentity
		(
			FMatrix M
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			// Use the integer pipeline to reduce branching to a minimum
			auto pWork = reinterpret_cast<const uint32_t*>(&M.m[0][0]);
			// Convert 1.0f to zero and or them together
			uint32_t uOne = pWork[0] ^ 0x3F800000U;
			// Or all the 0.0f entries together
			uint32_t uZero = pWork[1];
			uZero |= pWork[2];
			uZero |= pWork[3];
			// 2nd row
			uZero |= pWork[4];
			uOne |= pWork[5] ^ 0x3F800000U;
			uZero |= pWork[6];
			uZero |= pWork[7];
			// 3rd row
			uZero |= pWork[8];
			uZero |= pWork[9];
			uOne |= pWork[10] ^ 0x3F800000U;
			uZero |= pWork[11];
			// 4th row
			uZero |= pWork[12];
			uZero |= pWork[13];
			uZero |= pWork[14];
			uOne |= pWork[15] ^ 0x3F800000U;
			// If all zero entries are zero, the uZero==0
			uZero &= 0x7FFFFFFF;    // Allow -0.0f
			// If all 1.0f entries are 1.0f, then uOne==0
			uOne |= uZero;
			return (uOne == 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Vector vTemp1 = vceqq_f32(M.r[0], g_XMIdentityR0);
			Vector vTemp2 = vceqq_f32(M.r[1], g_XMIdentityR1);
			Vector vTemp3 = vceqq_f32(M.r[2], g_XMIdentityR2);
			Vector vTemp4 = vceqq_f32(M.r[3], g_XMIdentityR3);
			vTemp1 = vandq_u32(vTemp1, vTemp2);
			vTemp3 = vandq_u32(vTemp3, vTemp4);
			vTemp1 = vandq_u32(vTemp1, vTemp3);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTemp1), vget_high_u8(vTemp1));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			return (r == 0xFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vTemp1 = _mm_cmpeq_ps(M.r[0], g_XMIdentityR0);
			Vector vTemp2 = _mm_cmpeq_ps(M.r[1], g_XMIdentityR1);
			Vector vTemp3 = _mm_cmpeq_ps(M.r[2], g_XMIdentityR2);
			Vector vTemp4 = _mm_cmpeq_ps(M.r[3], g_XMIdentityR3);
			vTemp1 = _mm_and_ps(vTemp1, vTemp2);
			vTemp3 = _mm_and_ps(vTemp3, vTemp4);
			vTemp1 = _mm_and_ps(vTemp1, vTemp3);
			return (_mm_movemask_ps(vTemp1) == 0x0f);
#endif
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------
		// Perform a 4x4 matrix multiply by a 4x4 matrix
		inline Matrix XM_CALLCONV MatrixMultiply
		(
			FMatrix M1,
			CMatrix M2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			Matrix mResult;
			// Cache the invariants in registers
			float x = M1.m[0][0];
			float y = M1.m[0][1];
			float z = M1.m[0][2];
			float w = M1.m[0][3];
			// Perform the operation on the first row
			mResult.m[0][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
			mResult.m[0][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
			mResult.m[0][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
			mResult.m[0][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
			// Repeat for all the other rows
			x = M1.m[1][0];
			y = M1.m[1][1];
			z = M1.m[1][2];
			w = M1.m[1][3];
			mResult.m[1][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
			mResult.m[1][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
			mResult.m[1][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
			mResult.m[1][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
			x = M1.m[2][0];
			y = M1.m[2][1];
			z = M1.m[2][2];
			w = M1.m[2][3];
			mResult.m[2][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
			mResult.m[2][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
			mResult.m[2][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
			mResult.m[2][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
			x = M1.m[3][0];
			y = M1.m[3][1];
			z = M1.m[3][2];
			w = M1.m[3][3];
			mResult.m[3][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
			mResult.m[3][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
			mResult.m[3][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
			mResult.m[3][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
			return mResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Matrix mResult;
			float32x2_t VL = vget_low_f32(M1.r[0]);
			float32x2_t VH = vget_high_f32(M1.r[0]);
			// Perform the operation on the first row
			Vector vX = vmulq_lane_f32(M2.r[0], VL, 0);
			Vector vY = vmulq_lane_f32(M2.r[1], VL, 1);
			Vector vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			Vector vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			mResult.r[0] = vaddq_f32(vZ, vW);
			// Repeat for the other 3 rows
			VL = vget_low_f32(M1.r[1]);
			VH = vget_high_f32(M1.r[1]);
			vX = vmulq_lane_f32(M2.r[0], VL, 0);
			vY = vmulq_lane_f32(M2.r[1], VL, 1);
			vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			mResult.r[1] = vaddq_f32(vZ, vW);
			VL = vget_low_f32(M1.r[2]);
			VH = vget_high_f32(M1.r[2]);
			vX = vmulq_lane_f32(M2.r[0], VL, 0);
			vY = vmulq_lane_f32(M2.r[1], VL, 1);
			vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			mResult.r[2] = vaddq_f32(vZ, vW);
			VL = vget_low_f32(M1.r[3]);
			VH = vget_high_f32(M1.r[3]);
			vX = vmulq_lane_f32(M2.r[0], VL, 0);
			vY = vmulq_lane_f32(M2.r[1], VL, 1);
			vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			mResult.r[3] = vaddq_f32(vZ, vW);
			return mResult;
#elif defined(_XM_SSE_INTRINSICS_)
			Matrix mResult;
			// Splat the component X,Y,Z then W
#if defined(_XM_AVX_INTRINSICS_)
			Vector vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 0);
			Vector vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 1);
			Vector vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 2);
			Vector vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 3);
#else
	// Use vW to hold the original row
			Vector vW = M1.r[0];
			Vector vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			Vector vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			Vector vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			// Perform the operation on the first row
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			// Perform a binary add to reduce cumulative errors
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			mResult.r[0] = vX;
			// Repeat for the other 3 rows
#if defined(_XM_AVX_INTRINSICS_)
			vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 0);
			vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 1);
			vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 2);
			vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 3);
#else
			vW = M1.r[1];
			vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			mResult.r[1] = vX;
#if defined(_XM_AVX_INTRINSICS_)
			vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 0);
			vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 1);
			vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 2);
			vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 3);
#else
			vW = M1.r[2];
			vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			mResult.r[2] = vX;
#if defined(_XM_AVX_INTRINSICS_)
			vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 0);
			vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 1);
			vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 2);
			vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 3);
#else
			vW = M1.r[3];
			vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			mResult.r[3] = vX;
			return mResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixMultiplyTranspose
		(
			FMatrix M1,
			CMatrix M2
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			Matrix mResult;
			// Cache the invariants in registers
			float x = M2.m[0][0];
			float y = M2.m[1][0];
			float z = M2.m[2][0];
			float w = M2.m[3][0];
			// Perform the operation on the first row
			mResult.m[0][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
			mResult.m[0][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
			mResult.m[0][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
			mResult.m[0][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
			// Repeat for all the other rows
			x = M2.m[0][1];
			y = M2.m[1][1];
			z = M2.m[2][1];
			w = M2.m[3][1];
			mResult.m[1][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
			mResult.m[1][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
			mResult.m[1][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
			mResult.m[1][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
			x = M2.m[0][2];
			y = M2.m[1][2];
			z = M2.m[2][2];
			w = M2.m[3][2];
			mResult.m[2][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
			mResult.m[2][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
			mResult.m[2][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
			mResult.m[2][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
			x = M2.m[0][3];
			y = M2.m[1][3];
			z = M2.m[2][3];
			w = M2.m[3][3];
			mResult.m[3][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
			mResult.m[3][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
			mResult.m[3][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
			mResult.m[3][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
			return mResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x2_t VL = vget_low_f32(M1.r[0]);
			float32x2_t VH = vget_high_f32(M1.r[0]);
			// Perform the operation on the first row
			Vector vX = vmulq_lane_f32(M2.r[0], VL, 0);
			Vector vY = vmulq_lane_f32(M2.r[1], VL, 1);
			Vector vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			Vector vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			float32x4_t r0 = vaddq_f32(vZ, vW);
			// Repeat for the other 3 rows
			VL = vget_low_f32(M1.r[1]);
			VH = vget_high_f32(M1.r[1]);
			vX = vmulq_lane_f32(M2.r[0], VL, 0);
			vY = vmulq_lane_f32(M2.r[1], VL, 1);
			vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			float32x4_t r1 = vaddq_f32(vZ, vW);
			VL = vget_low_f32(M1.r[2]);
			VH = vget_high_f32(M1.r[2]);
			vX = vmulq_lane_f32(M2.r[0], VL, 0);
			vY = vmulq_lane_f32(M2.r[1], VL, 1);
			vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			float32x4_t r2 = vaddq_f32(vZ, vW);
			VL = vget_low_f32(M1.r[3]);
			VH = vget_high_f32(M1.r[3]);
			vX = vmulq_lane_f32(M2.r[0], VL, 0);
			vY = vmulq_lane_f32(M2.r[1], VL, 1);
			vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			float32x4_t r3 = vaddq_f32(vZ, vW);

			// Transpose result
			float32x4x2_t P0 = vzipq_f32(r0, r2);
			float32x4x2_t P1 = vzipq_f32(r1, r3);

			float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
			float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);

			Matrix mResult;
			mResult.r[0] = T0.val[0];
			mResult.r[1] = T0.val[1];
			mResult.r[2] = T1.val[0];
			mResult.r[3] = T1.val[1];
			return mResult;
#elif defined(_XM_SSE_INTRINSICS_)
			// Splat the component X,Y,Z then W
#if defined(_XM_AVX_INTRINSICS_)
			Vector vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 0);
			Vector vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 1);
			Vector vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 2);
			Vector vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 3);
#else
	// Use vW to hold the original row
			Vector vW = M1.r[0];
			Vector vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			Vector vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			Vector vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			// Perform the operation on the first row
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			// Perform a binary add to reduce cumulative errors
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			Vector r0 = vX;
			// Repeat for the other 3 rows
#if defined(_XM_AVX_INTRINSICS_)
			vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 0);
			vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 1);
			vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 2);
			vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 3);
#else
			vW = M1.r[1];
			vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			Vector r1 = vX;
#if defined(_XM_AVX_INTRINSICS_)
			vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 0);
			vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 1);
			vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 2);
			vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 3);
#else
			vW = M1.r[2];
			vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			Vector r2 = vX;
#if defined(_XM_AVX_INTRINSICS_)
			vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 0);
			vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 1);
			vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 2);
			vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 3);
#else
			vW = M1.r[3];
			vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			Vector r3 = vX;

			// x.x,x.y,y.x,y.y
			Vector vTemp1 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 0, 1, 0));
			// x.z,x.w,y.z,y.w
			Vector vTemp3 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(3, 2, 3, 2));
			// z.x,z.y,w.x,w.y
			Vector vTemp2 = _mm_shuffle_ps(r2, r3, _MM_SHUFFLE(1, 0, 1, 0));
			// z.z,z.w,w.z,w.w
			Vector vTemp4 = _mm_shuffle_ps(r2, r3, _MM_SHUFFLE(3, 2, 3, 2));

			Matrix mResult;
			// x.x,y.x,z.x,w.x
			mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			// x.y,y.y,z.y,w.y
			mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			// x.z,y.z,z.z,w.z
			mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
			// x.w,y.w,z.w,w.w
			mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
			return mResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixTranspose
		(
			FMatrix M
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			// Original matrix:
			//
			//     m00m01m02m03
			//     m10m11m12m13
			//     m20m21m22m23
			//     m30m31m32m33

			Matrix P;
			P.r[0] = VectorMergeXY(M.r[0], M.r[2]); // m00m20m01m21
			P.r[1] = VectorMergeXY(M.r[1], M.r[3]); // m10m30m11m31
			P.r[2] = VectorMergeZW(M.r[0], M.r[2]); // m02m22m03m23
			P.r[3] = VectorMergeZW(M.r[1], M.r[3]); // m12m32m13m33

			Matrix MT;
			MT.r[0] = VectorMergeXY(P.r[0], P.r[1]); // m00m10m20m30
			MT.r[1] = VectorMergeZW(P.r[0], P.r[1]); // m01m11m21m31
			MT.r[2] = VectorMergeXY(P.r[2], P.r[3]); // m02m12m22m32
			MT.r[3] = VectorMergeZW(P.r[2], P.r[3]); // m03m13m23m33
			return MT;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float32x4x2_t P0 = vzipq_f32(M.r[0], M.r[2]);
			float32x4x2_t P1 = vzipq_f32(M.r[1], M.r[3]);

			float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
			float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);

			Matrix mResult;
			mResult.r[0] = T0.val[0];
			mResult.r[1] = T0.val[1];
			mResult.r[2] = T1.val[0];
			mResult.r[3] = T1.val[1];
			return mResult;
#elif defined(_XM_SSE_INTRINSICS_)
			// x.x,x.y,y.x,y.y
			Vector vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
			// x.z,x.w,y.z,y.w
			Vector vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
			// z.x,z.y,w.x,w.y
			Vector vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
			// z.z,z.w,w.z,w.w
			Vector vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));
			Matrix mResult;

			// x.x,y.x,z.x,w.x
			mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			// x.y,y.y,z.y,w.y
			mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			// x.z,y.z,z.z,w.z
			mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
			// x.w,y.w,z.w,w.w
			mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
			return mResult;
#endif
		}

		//------------------------------------------------------------------------------
		// Return the inverse and the determinant of a 4x4 matrix

		inline Matrix XM_CALLCONV MatrixInverse
		(
			Vector* pDeterminant,
			FMatrix  M
		)
		{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)

			Matrix MT = MatrixTranspose(M);

			Vector V0[4], V1[4];
			V0[0] = VectorSwizzle<XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(MT.r[2]);
			V1[0] = VectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_W>(MT.r[3]);
			V0[1] = VectorSwizzle<XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(MT.r[0]);
			V1[1] = VectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_W>(MT.r[1]);
			V0[2] = VectorPermute<XM_PERMUTE_0X, XM_PERMUTE_0Z, XM_PERMUTE_1X, XM_PERMUTE_1Z>(MT.r[2], MT.r[0]);
			V1[2] = VectorPermute<XM_PERMUTE_0Y, XM_PERMUTE_0W, XM_PERMUTE_1Y, XM_PERMUTE_1W>(MT.r[3], MT.r[1]);

			Vector D0 = VectorMultiply(V0[0], V1[0]);
			Vector D1 = VectorMultiply(V0[1], V1[1]);
			Vector D2 = VectorMultiply(V0[2], V1[2]);

			V0[0] = VectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_W>(MT.r[2]);
			V1[0] = VectorSwizzle<XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(MT.r[3]);
			V0[1] = VectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_W>(MT.r[0]);
			V1[1] = VectorSwizzle<XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(MT.r[1]);
			V0[2] = VectorPermute<XM_PERMUTE_0Y, XM_PERMUTE_0W, XM_PERMUTE_1Y, XM_PERMUTE_1W>(MT.r[2], MT.r[0]);
			V1[2] = VectorPermute<XM_PERMUTE_0X, XM_PERMUTE_0Z, XM_PERMUTE_1X, XM_PERMUTE_1Z>(MT.r[3], MT.r[1]);

			D0 = VectorNegativeMultiplySubtract(V0[0], V1[0], D0);
			D1 = VectorNegativeMultiplySubtract(V0[1], V1[1], D1);
			D2 = VectorNegativeMultiplySubtract(V0[2], V1[2], D2);

			V0[0] = VectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_Z, XM_SWIZZLE_X, XM_SWIZZLE_Y>(MT.r[1]);
			V1[0] = VectorPermute<XM_PERMUTE_1Y, XM_PERMUTE_0Y, XM_PERMUTE_0W, XM_PERMUTE_0X>(D0, D2);
			V0[1] = VectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_X>(MT.r[0]);
			V1[1] = VectorPermute<XM_PERMUTE_0W, XM_PERMUTE_1Y, XM_PERMUTE_0Y, XM_PERMUTE_0Z>(D0, D2);
			V0[2] = VectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_Z, XM_SWIZZLE_X, XM_SWIZZLE_Y>(MT.r[3]);
			V1[2] = VectorPermute<XM_PERMUTE_1W, XM_PERMUTE_0Y, XM_PERMUTE_0W, XM_PERMUTE_0X>(D1, D2);
			V0[3] = VectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_X>(MT.r[2]);
			V1[3] = VectorPermute<XM_PERMUTE_0W, XM_PERMUTE_1W, XM_PERMUTE_0Y, XM_PERMUTE_0Z>(D1, D2);

			Vector C0 = VectorMultiply(V0[0], V1[0]);
			Vector C2 = VectorMultiply(V0[1], V1[1]);
			Vector C4 = VectorMultiply(V0[2], V1[2]);
			Vector C6 = VectorMultiply(V0[3], V1[3]);

			V0[0] = VectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Y, XM_SWIZZLE_Z>(MT.r[1]);
			V1[0] = VectorPermute<XM_PERMUTE_0W, XM_PERMUTE_0X, XM_PERMUTE_0Y, XM_PERMUTE_1X>(D0, D2);
			V0[1] = VectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Y>(MT.r[0]);
			V1[1] = VectorPermute<XM_PERMUTE_0Z, XM_PERMUTE_0Y, XM_PERMUTE_1X, XM_PERMUTE_0X>(D0, D2);
			V0[2] = VectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Y, XM_SWIZZLE_Z>(MT.r[3]);
			V1[2] = VectorPermute<XM_PERMUTE_0W, XM_PERMUTE_0X, XM_PERMUTE_0Y, XM_PERMUTE_1Z>(D1, D2);
			V0[3] = VectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Y>(MT.r[2]);
			V1[3] = VectorPermute<XM_PERMUTE_0Z, XM_PERMUTE_0Y, XM_PERMUTE_1Z, XM_PERMUTE_0X>(D1, D2);

			C0 = VectorNegativeMultiplySubtract(V0[0], V1[0], C0);
			C2 = VectorNegativeMultiplySubtract(V0[1], V1[1], C2);
			C4 = VectorNegativeMultiplySubtract(V0[2], V1[2], C4);
			C6 = VectorNegativeMultiplySubtract(V0[3], V1[3], C6);

			V0[0] = VectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_X>(MT.r[1]);
			V1[0] = VectorPermute<XM_PERMUTE_0Z, XM_PERMUTE_1Y, XM_PERMUTE_1X, XM_PERMUTE_0Z>(D0, D2);
			V0[1] = VectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Z>(MT.r[0]);
			V1[1] = VectorPermute<XM_PERMUTE_1Y, XM_PERMUTE_0X, XM_PERMUTE_0W, XM_PERMUTE_1X>(D0, D2);
			V0[2] = VectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_X>(MT.r[3]);
			V1[2] = VectorPermute<XM_PERMUTE_0Z, XM_PERMUTE_1W, XM_PERMUTE_1Z, XM_PERMUTE_0Z>(D1, D2);
			V0[3] = VectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Z>(MT.r[2]);
			V1[3] = VectorPermute<XM_PERMUTE_1W, XM_PERMUTE_0X, XM_PERMUTE_0W, XM_PERMUTE_1Z>(D1, D2);

			Vector C1 = VectorNegativeMultiplySubtract(V0[0], V1[0], C0);
			C0 = VectorMultiplyAdd(V0[0], V1[0], C0);
			Vector C3 = VectorMultiplyAdd(V0[1], V1[1], C2);
			C2 = VectorNegativeMultiplySubtract(V0[1], V1[1], C2);
			Vector C5 = VectorNegativeMultiplySubtract(V0[2], V1[2], C4);
			C4 = VectorMultiplyAdd(V0[2], V1[2], C4);
			Vector C7 = VectorMultiplyAdd(V0[3], V1[3], C6);
			C6 = VectorNegativeMultiplySubtract(V0[3], V1[3], C6);

			Matrix R;
			R.r[0] = VectorSelect(C0, C1, g_XMSelect0101.v);
			R.r[1] = VectorSelect(C2, C3, g_XMSelect0101.v);
			R.r[2] = VectorSelect(C4, C5, g_XMSelect0101.v);
			R.r[3] = VectorSelect(C6, C7, g_XMSelect0101.v);

			Vector Determinant = Vector4Dot(R.r[0], MT.r[0]);

			if (pDeterminant != nullptr)
				*pDeterminant = Determinant;

			Vector Reciprocal = VectorReciprocal(Determinant);

			Matrix Result;
			Result.r[0] = VectorMultiply(R.r[0], Reciprocal);
			Result.r[1] = VectorMultiply(R.r[1], Reciprocal);
			Result.r[2] = VectorMultiply(R.r[2], Reciprocal);
			Result.r[3] = VectorMultiply(R.r[3], Reciprocal);
			return Result;

#elif defined(_XM_SSE_INTRINSICS_)
			Matrix MT = MatrixTranspose(M);
			Vector V00 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(1, 1, 0, 0));
			Vector V10 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(3, 2, 3, 2));
			Vector V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(1, 1, 0, 0));
			Vector V11 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(3, 2, 3, 2));
			Vector V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(2, 0, 2, 0));
			Vector V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(3, 1, 3, 1));

			Vector D0 = _mm_mul_ps(V00, V10);
			Vector D1 = _mm_mul_ps(V01, V11);
			Vector D2 = _mm_mul_ps(V02, V12);

			V00 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(3, 2, 3, 2));
			V10 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(1, 1, 0, 0));
			V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(3, 2, 3, 2));
			V11 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(1, 1, 0, 0));
			V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(3, 1, 3, 1));
			V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(2, 0, 2, 0));

			V00 = _mm_mul_ps(V00, V10);
			V01 = _mm_mul_ps(V01, V11);
			V02 = _mm_mul_ps(V02, V12);
			D0 = _mm_sub_ps(D0, V00);
			D1 = _mm_sub_ps(D1, V01);
			D2 = _mm_sub_ps(D2, V02);
			// V11 = D0Y,D0W,D2Y,D2Y
			V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
			V00 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(1, 0, 2, 1));
			V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
			V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(0, 1, 0, 2));
			V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));
			// V13 = D1Y,D1W,D2W,D2W
			Vector V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
			V02 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(1, 0, 2, 1));
			V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
			Vector V03 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(0, 1, 0, 2));
			V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

			Vector C0 = _mm_mul_ps(V00, V10);
			Vector C2 = _mm_mul_ps(V01, V11);
			Vector C4 = _mm_mul_ps(V02, V12);
			Vector C6 = _mm_mul_ps(V03, V13);

			// V11 = D0X,D0Y,D2X,D2X
			V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
			V00 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(2, 1, 3, 2));
			V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
			V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(1, 3, 2, 3));
			V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));
			// V13 = D1X,D1Y,D2Z,D2Z
			V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
			V02 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(2, 1, 3, 2));
			V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
			V03 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(1, 3, 2, 3));
			V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

			V00 = _mm_mul_ps(V00, V10);
			V01 = _mm_mul_ps(V01, V11);
			V02 = _mm_mul_ps(V02, V12);
			V03 = _mm_mul_ps(V03, V13);
			C0 = _mm_sub_ps(C0, V00);
			C2 = _mm_sub_ps(C2, V01);
			C4 = _mm_sub_ps(C4, V02);
			C6 = _mm_sub_ps(C6, V03);

			V00 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(0, 3, 0, 3));
			// V10 = D0Z,D0Z,D2X,D2Y
			V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
			V10 = XM_PERMUTE_PS(V10, _MM_SHUFFLE(0, 2, 3, 0));
			V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(2, 0, 3, 1));
			// V11 = D0X,D0W,D2X,D2Y
			V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
			V11 = XM_PERMUTE_PS(V11, _MM_SHUFFLE(2, 1, 0, 3));
			V02 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(0, 3, 0, 3));
			// V12 = D1Z,D1Z,D2Z,D2W
			V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
			V12 = XM_PERMUTE_PS(V12, _MM_SHUFFLE(0, 2, 3, 0));
			V03 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(2, 0, 3, 1));
			// V13 = D1X,D1W,D2Z,D2W
			V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
			V13 = XM_PERMUTE_PS(V13, _MM_SHUFFLE(2, 1, 0, 3));

			V00 = _mm_mul_ps(V00, V10);
			V01 = _mm_mul_ps(V01, V11);
			V02 = _mm_mul_ps(V02, V12);
			V03 = _mm_mul_ps(V03, V13);
			Vector C1 = _mm_sub_ps(C0, V00);
			C0 = _mm_add_ps(C0, V00);
			Vector C3 = _mm_add_ps(C2, V01);
			C2 = _mm_sub_ps(C2, V01);
			Vector C5 = _mm_sub_ps(C4, V02);
			C4 = _mm_add_ps(C4, V02);
			Vector C7 = _mm_add_ps(C6, V03);
			C6 = _mm_sub_ps(C6, V03);

			C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
			C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
			C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
			C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
			C0 = XM_PERMUTE_PS(C0, _MM_SHUFFLE(3, 1, 2, 0));
			C2 = XM_PERMUTE_PS(C2, _MM_SHUFFLE(3, 1, 2, 0));
			C4 = XM_PERMUTE_PS(C4, _MM_SHUFFLE(3, 1, 2, 0));
			C6 = XM_PERMUTE_PS(C6, _MM_SHUFFLE(3, 1, 2, 0));
			// Get the determinate
			Vector vTemp = Vector4Dot(C0, MT.r[0]);
			if (pDeterminant != nullptr)
				*pDeterminant = vTemp;
			vTemp = _mm_div_ps(g_XMOne, vTemp);
			Matrix mResult;
			mResult.r[0] = _mm_mul_ps(C0, vTemp);
			mResult.r[1] = _mm_mul_ps(C2, vTemp);
			mResult.r[2] = _mm_mul_ps(C4, vTemp);
			mResult.r[3] = _mm_mul_ps(C6, vTemp);
			return mResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV MatrixDeterminant
		(
			FMatrix M
		)
		{
			static const VectorF32 Sign = { { { 1.0f, -1.0f, 1.0f, -1.0f } } };

			Vector V0 = VectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_X>(M.r[2]);
			Vector V1 = VectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(M.r[3]);
			Vector V2 = VectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_X>(M.r[2]);
			Vector V3 = VectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_Z>(M.r[3]);
			Vector V4 = VectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(M.r[2]);
			Vector V5 = VectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_Z>(M.r[3]);

			Vector P0 = VectorMultiply(V0, V1);
			Vector P1 = VectorMultiply(V2, V3);
			Vector P2 = VectorMultiply(V4, V5);

			V0 = VectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(M.r[2]);
			V1 = VectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_X>(M.r[3]);
			V2 = VectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_Z>(M.r[2]);
			V3 = VectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_X>(M.r[3]);
			V4 = VectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_Z>(M.r[2]);
			V5 = VectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(M.r[3]);

			P0 = VectorNegativeMultiplySubtract(V0, V1, P0);
			P1 = VectorNegativeMultiplySubtract(V2, V3, P1);
			P2 = VectorNegativeMultiplySubtract(V4, V5, P2);

			V0 = VectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_Z>(M.r[1]);
			V1 = VectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(M.r[1]);
			V2 = VectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_X>(M.r[1]);

			Vector S = VectorMultiply(M.r[0], Sign.v);
			Vector R = VectorMultiply(V0, P0);
			R = VectorNegativeMultiplySubtract(V1, P1, R);
			R = VectorMultiplyAdd(V2, P2, R);

			return Vector4Dot(S, R);
		}

#define XM3RANKDECOMPOSE(a, b, c, x, y, z)      \
    if((x) < (y))                   \
    {                               \
        if((y) < (z))               \
        {                           \
            (a) = 2;                \
            (b) = 1;                \
            (c) = 0;                \
        }                           \
        else                        \
        {                           \
            (a) = 1;                \
                                    \
            if((x) < (z))           \
            {                       \
                (b) = 2;            \
                (c) = 0;            \
            }                       \
            else                    \
            {                       \
                (b) = 0;            \
                (c) = 2;            \
            }                       \
        }                           \
    }                               \
    else                            \
    {                               \
        if((x) < (z))               \
        {                           \
            (a) = 2;                \
            (b) = 0;                \
            (c) = 1;                \
        }                           \
        else                        \
        {                           \
            (a) = 0;                \
                                    \
            if((y) < (z))           \
            {                       \
                (b) = 2;            \
                (c) = 1;            \
            }                       \
            else                    \
            {                       \
                (b) = 1;            \
                (c) = 2;            \
            }                       \
        }                           \
    }

#define XM3_DECOMP_EPSILON 0.0001f


		inline bool XM_CALLCONV MatrixDecompose
		(
			Vector* outScale,
			Vector* outRotQuat,
			Vector* outTrans,
			FMatrix M
		)
		{
			static const Vector* pvCanonicalBasis[3] = {
				&g_XMIdentityR0.v,
				&g_XMIdentityR1.v,
				&g_XMIdentityR2.v
			};

			assert(outScale != nullptr);
			assert(outRotQuat != nullptr);
			assert(outTrans != nullptr);

			// Get the translation
			outTrans[0] = M.r[3];

			Vector* ppvBasis[3];
			Matrix matTemp;
			ppvBasis[0] = &matTemp.r[0];
			ppvBasis[1] = &matTemp.r[1];
			ppvBasis[2] = &matTemp.r[2];

			matTemp.r[0] = M.r[0];
			matTemp.r[1] = M.r[1];
			matTemp.r[2] = M.r[2];
			matTemp.r[3] = g_XMIdentityR3.v;

			auto pfScales = reinterpret_cast<float*>(outScale);

			size_t a, b, c;
			VectorGetXPtr(&pfScales[0], Vector3Length(ppvBasis[0][0]));
			VectorGetXPtr(&pfScales[1], Vector3Length(ppvBasis[1][0]));
			VectorGetXPtr(&pfScales[2], Vector3Length(ppvBasis[2][0]));
			pfScales[3] = 0.f;

			XM3RANKDECOMPOSE(a, b, c, pfScales[0], pfScales[1], pfScales[2])

				if (pfScales[a] < XM3_DECOMP_EPSILON)
				{
					ppvBasis[a][0] = pvCanonicalBasis[a][0];
				}
			ppvBasis[a][0] = Vector3Normalize(ppvBasis[a][0]);

			if (pfScales[b] < XM3_DECOMP_EPSILON)
			{
				size_t aa, bb, cc;
				float fAbsX, fAbsY, fAbsZ;

				fAbsX = fabsf(VectorGetX(ppvBasis[a][0]));
				fAbsY = fabsf(VectorGetY(ppvBasis[a][0]));
				fAbsZ = fabsf(VectorGetZ(ppvBasis[a][0]));

				XM3RANKDECOMPOSE(aa, bb, cc, fAbsX, fAbsY, fAbsZ)

					ppvBasis[b][0] = Vector3Cross(ppvBasis[a][0], pvCanonicalBasis[cc][0]);
			}

			ppvBasis[b][0] = Vector3Normalize(ppvBasis[b][0]);

			if (pfScales[c] < XM3_DECOMP_EPSILON)
			{
				ppvBasis[c][0] = Vector3Cross(ppvBasis[a][0], ppvBasis[b][0]);
			}

			ppvBasis[c][0] = Vector3Normalize(ppvBasis[c][0]);

			float fDet = VectorGetX(MatrixDeterminant(matTemp));

			// use Kramer's rule to check for handedness of coordinate system
			if (fDet < 0.0f)
			{
				// switch coordinate system by negating the scale and inverting the basis vector on the x-axis
				pfScales[a] = -pfScales[a];
				ppvBasis[a][0] = VectorNegate(ppvBasis[a][0]);

				fDet = -fDet;
			}

			fDet -= 1.0f;
			fDet *= fDet;

			if (XM3_DECOMP_EPSILON < fDet)
			{
				// Non-SRT matrix encountered
				return false;
			}

			// generate the quaternion from the matrix
			outRotQuat[0] = QuaternionRotationMatrix(matTemp);
			return true;
		}

#undef XM3_DECOMP_EPSILON
#undef XM3RANKDECOMPOSE

		//------------------------------------------------------------------------------
		// Transformation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixIdentity()
		{
			Matrix M;
			M.r[0] = g_XMIdentityR0.v;
			M.r[1] = g_XMIdentityR1.v;
			M.r[2] = g_XMIdentityR2.v;
			M.r[3] = g_XMIdentityR3.v;
			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixSet
		(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
		)
		{
			Matrix M;
#if defined(_XM_NO_INTRINSICS_)
			M.m[0][0] = m00; M.m[0][1] = m01; M.m[0][2] = m02; M.m[0][3] = m03;
			M.m[1][0] = m10; M.m[1][1] = m11; M.m[1][2] = m12; M.m[1][3] = m13;
			M.m[2][0] = m20; M.m[2][1] = m21; M.m[2][2] = m22; M.m[2][3] = m23;
			M.m[3][0] = m30; M.m[3][1] = m31; M.m[3][2] = m32; M.m[3][3] = m33;
#else
			M.r[0] = VectorSet(m00, m01, m02, m03);
			M.r[1] = VectorSet(m10, m11, m12, m13);
			M.r[2] = VectorSet(m20, m21, m22, m23);
			M.r[3] = VectorSet(m30, m31, m32, m33);
#endif
			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixTranslation
		(
			float OffsetX,
			float OffsetY,
			float OffsetZ
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Matrix M;
			M.m[0][0] = 1.0f;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = 1.0f;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = 1.0f;
			M.m[2][3] = 0.0f;

			M.m[3][0] = OffsetX;
			M.m[3][1] = OffsetY;
			M.m[3][2] = OffsetZ;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
			Matrix M;
			M.r[0] = g_XMIdentityR0.v;
			M.r[1] = g_XMIdentityR1.v;
			M.r[2] = g_XMIdentityR2.v;
			M.r[3] = VectorSet(OffsetX, OffsetY, OffsetZ, 1.f);
			return M;
#endif
		}


		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixTranslationFromVector
		(
			FVector Offset
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Matrix M;
			M.m[0][0] = 1.0f;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = 1.0f;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = 1.0f;
			M.m[2][3] = 0.0f;

			M.m[3][0] = Offset.vector4_f32[0];
			M.m[3][1] = Offset.vector4_f32[1];
			M.m[3][2] = Offset.vector4_f32[2];
			M.m[3][3] = 1.0f;
			return M;

#elif defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
			Matrix M;
			M.r[0] = g_XMIdentityR0.v;
			M.r[1] = g_XMIdentityR1.v;
			M.r[2] = g_XMIdentityR2.v;
			M.r[3] = VectorSelect(g_XMIdentityR3.v, Offset, g_XMSelect1110.v);
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixScaling
		(
			float ScaleX,
			float ScaleY,
			float ScaleZ
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Matrix M;
			M.m[0][0] = ScaleX;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = ScaleY;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = ScaleZ;
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = 0.0f;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			const Vector Zero = vdupq_n_f32(0);
			Matrix M;
			M.r[0] = vsetq_lane_f32(ScaleX, Zero, 0);
			M.r[1] = vsetq_lane_f32(ScaleY, Zero, 1);
			M.r[2] = vsetq_lane_f32(ScaleZ, Zero, 2);
			M.r[3] = g_XMIdentityR3.v;
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			Matrix M;
			M.r[0] = _mm_set_ps(0, 0, 0, ScaleX);
			M.r[1] = _mm_set_ps(0, 0, ScaleY, 0);
			M.r[2] = _mm_set_ps(0, ScaleZ, 0, 0);
			M.r[3] = g_XMIdentityR3.v;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixScalingFromVector
		(
			FVector Scale
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			Matrix M;
			M.m[0][0] = Scale.vector4_f32[0];
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = Scale.vector4_f32[1];
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = Scale.vector4_f32[2];
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = 0.0f;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Matrix M;
			M.r[0] = vandq_u32(Scale, g_XMMaskX);
			M.r[1] = vandq_u32(Scale, g_XMMaskY);
			M.r[2] = vandq_u32(Scale, g_XMMaskZ);
			M.r[3] = g_XMIdentityR3.v;
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			Matrix M;
			M.r[0] = _mm_and_ps(Scale, g_XMMaskX);
			M.r[1] = _mm_and_ps(Scale, g_XMMaskY);
			M.r[2] = _mm_and_ps(Scale, g_XMMaskZ);
			M.r[3] = g_XMIdentityR3.v;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixRotationX
		(
			float Angle
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			Matrix M;
			M.m[0][0] = 1.0f;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = fCosAngle;
			M.m[1][2] = fSinAngle;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = -fSinAngle;
			M.m[2][2] = fCosAngle;
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = 0.0f;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			const Vector Zero = vdupq_n_f32(0);

			Vector T1 = vsetq_lane_f32(fCosAngle, Zero, 1);
			T1 = vsetq_lane_f32(fSinAngle, T1, 2);

			Vector T2 = vsetq_lane_f32(-fSinAngle, Zero, 1);
			T2 = vsetq_lane_f32(fCosAngle, T2, 2);

			Matrix M;
			M.r[0] = g_XMIdentityR0.v;
			M.r[1] = T1;
			M.r[2] = T2;
			M.r[3] = g_XMIdentityR3.v;
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			float    SinAngle;
			float    CosAngle;
			ScalarSinCos(&SinAngle, &CosAngle, Angle);

			Vector vSin = _mm_set_ss(SinAngle);
			Vector vCos = _mm_set_ss(CosAngle);
			// x = 0,y = cos,z = sin, w = 0
			vCos = _mm_shuffle_ps(vCos, vSin, _MM_SHUFFLE(3, 0, 0, 3));
			Matrix M;
			M.r[0] = g_XMIdentityR0;
			M.r[1] = vCos;
			// x = 0,y = sin,z = cos, w = 0
			vCos = XM_PERMUTE_PS(vCos, _MM_SHUFFLE(3, 1, 2, 0));
			// x = 0,y = -sin,z = cos, w = 0
			vCos = _mm_mul_ps(vCos, g_XMNegateY);
			M.r[2] = vCos;
			M.r[3] = g_XMIdentityR3;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixRotationY
		(
			float Angle
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			Matrix M;
			M.m[0][0] = fCosAngle;
			M.m[0][1] = 0.0f;
			M.m[0][2] = -fSinAngle;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = 1.0f;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = fSinAngle;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fCosAngle;
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = 0.0f;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			const Vector Zero = vdupq_n_f32(0);

			Vector T0 = vsetq_lane_f32(fCosAngle, Zero, 0);
			T0 = vsetq_lane_f32(-fSinAngle, T0, 2);

			Vector T2 = vsetq_lane_f32(fSinAngle, Zero, 0);
			T2 = vsetq_lane_f32(fCosAngle, T2, 2);

			Matrix M;
			M.r[0] = T0;
			M.r[1] = g_XMIdentityR1.v;
			M.r[2] = T2;
			M.r[3] = g_XMIdentityR3.v;
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			float    SinAngle;
			float    CosAngle;
			ScalarSinCos(&SinAngle, &CosAngle, Angle);

			Vector vSin = _mm_set_ss(SinAngle);
			Vector vCos = _mm_set_ss(CosAngle);
			// x = sin,y = 0,z = cos, w = 0
			vSin = _mm_shuffle_ps(vSin, vCos, _MM_SHUFFLE(3, 0, 3, 0));
			Matrix M;
			M.r[2] = vSin;
			M.r[1] = g_XMIdentityR1;
			// x = cos,y = 0,z = sin, w = 0
			vSin = XM_PERMUTE_PS(vSin, _MM_SHUFFLE(3, 0, 1, 2));
			// x = cos,y = 0,z = -sin, w = 0
			vSin = _mm_mul_ps(vSin, g_XMNegateZ);
			M.r[0] = vSin;
			M.r[3] = g_XMIdentityR3;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixRotationZ
		(
			float Angle
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			Matrix M;
			M.m[0][0] = fCosAngle;
			M.m[0][1] = fSinAngle;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = -fSinAngle;
			M.m[1][1] = fCosAngle;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = 1.0f;
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = 0.0f;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			const Vector Zero = vdupq_n_f32(0);

			Vector T0 = vsetq_lane_f32(fCosAngle, Zero, 0);
			T0 = vsetq_lane_f32(fSinAngle, T0, 1);

			Vector T1 = vsetq_lane_f32(-fSinAngle, Zero, 0);
			T1 = vsetq_lane_f32(fCosAngle, T1, 1);

			Matrix M;
			M.r[0] = T0;
			M.r[1] = T1;
			M.r[2] = g_XMIdentityR2.v;
			M.r[3] = g_XMIdentityR3.v;
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			float    SinAngle;
			float    CosAngle;
			ScalarSinCos(&SinAngle, &CosAngle, Angle);

			Vector vSin = _mm_set_ss(SinAngle);
			Vector vCos = _mm_set_ss(CosAngle);
			// x = cos,y = sin,z = 0, w = 0
			vCos = _mm_unpacklo_ps(vCos, vSin);
			Matrix M;
			M.r[0] = vCos;
			// x = sin,y = cos,z = 0, w = 0
			vCos = XM_PERMUTE_PS(vCos, _MM_SHUFFLE(3, 2, 0, 1));
			// x = cos,y = -sin,z = 0, w = 0
			vCos = _mm_mul_ps(vCos, g_XMNegateX);
			M.r[1] = vCos;
			M.r[2] = g_XMIdentityR2;
			M.r[3] = g_XMIdentityR3;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixRotationRollPitchYaw
		(
			float Pitch,
			float Yaw,
			float Roll
		)
		{
			Vector Angles = VectorSet(Pitch, Yaw, Roll, 0.0f);
			return MatrixRotationRollPitchYawFromVector(Angles);
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixRotationRollPitchYawFromVector
		(
			FVector Angles // <Pitch, Yaw, Roll, undefined>
		)
		{
			Vector Q = QuaternionRotationRollPitchYawFromVector(Angles);
			return MatrixRotationQuaternion(Q);
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixRotationNormal
		(
			FVector NormalAxis,
			float     Angle
		)
		{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)

			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			Vector A = VectorSet(fSinAngle, fCosAngle, 1.0f - fCosAngle, 0.0f);

			Vector C2 = VectorSplatZ(A);
			Vector C1 = VectorSplatY(A);
			Vector C0 = VectorSplatX(A);

			Vector N0 = VectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_Z, XM_SWIZZLE_X, XM_SWIZZLE_W>(NormalAxis);
			Vector N1 = VectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_W>(NormalAxis);

			Vector V0 = VectorMultiply(C2, N0);
			V0 = VectorMultiply(V0, N1);

			Vector R0 = VectorMultiply(C2, NormalAxis);
			R0 = VectorMultiplyAdd(R0, NormalAxis, C1);

			Vector R1 = VectorMultiplyAdd(C0, NormalAxis, V0);
			Vector R2 = VectorNegativeMultiplySubtract(C0, NormalAxis, V0);

			V0 = VectorSelect(A, R0, g_XMSelect1110.v);
			Vector V1 = VectorPermute<XM_PERMUTE_0Z, XM_PERMUTE_1Y, XM_PERMUTE_1Z, XM_PERMUTE_0X>(R1, R2);
			Vector V2 = VectorPermute<XM_PERMUTE_0Y, XM_PERMUTE_1X, XM_PERMUTE_0Y, XM_PERMUTE_1X>(R1, R2);

			Matrix M;
			M.r[0] = VectorPermute<XM_PERMUTE_0X, XM_PERMUTE_1X, XM_PERMUTE_1Y, XM_PERMUTE_0W>(V0, V1);
			M.r[1] = VectorPermute<XM_PERMUTE_1Z, XM_PERMUTE_0Y, XM_PERMUTE_1W, XM_PERMUTE_0W>(V0, V1);
			M.r[2] = VectorPermute<XM_PERMUTE_1X, XM_PERMUTE_1Y, XM_PERMUTE_0Z, XM_PERMUTE_0W>(V0, V2);
			M.r[3] = g_XMIdentityR3.v;
			return M;

#elif defined(_XM_SSE_INTRINSICS_)
			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			Vector C2 = _mm_set_ps1(1.0f - fCosAngle);
			Vector C1 = _mm_set_ps1(fCosAngle);
			Vector C0 = _mm_set_ps1(fSinAngle);

			Vector N0 = XM_PERMUTE_PS(NormalAxis, _MM_SHUFFLE(3, 0, 2, 1));
			Vector N1 = XM_PERMUTE_PS(NormalAxis, _MM_SHUFFLE(3, 1, 0, 2));

			Vector V0 = _mm_mul_ps(C2, N0);
			V0 = _mm_mul_ps(V0, N1);

			Vector R0 = _mm_mul_ps(C2, NormalAxis);
			R0 = _mm_mul_ps(R0, NormalAxis);
			R0 = _mm_add_ps(R0, C1);

			Vector R1 = _mm_mul_ps(C0, NormalAxis);
			R1 = _mm_add_ps(R1, V0);
			Vector R2 = _mm_mul_ps(C0, NormalAxis);
			R2 = _mm_sub_ps(V0, R2);

			V0 = _mm_and_ps(R0, g_XMMask3);
			Vector V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 1, 2, 0));
			V1 = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 3, 2, 1));
			Vector V2 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(0, 0, 1, 1));
			V2 = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 0, 2, 0));

			R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(1, 0, 3, 0));
			R2 = XM_PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 2, 0));

			Matrix M;
			M.r[0] = R2;

			R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(3, 2, 3, 1));
			R2 = XM_PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 0, 2));
			M.r[1] = R2;

			V2 = _mm_shuffle_ps(V2, V0, _MM_SHUFFLE(3, 2, 1, 0));
			M.r[2] = V2;
			M.r[3] = g_XMIdentityR3.v;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixRotationAxis
		(
			FVector Axis,
			float     Angle
		)
		{
			assert(!Vector3Equal(Axis, VectorZero()));
			assert(!Vector3IsInfinite(Axis));

			Vector Normal = Vector3Normalize(Axis);
			return MatrixRotationNormal(Normal, Angle);
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixRotationQuaternion
		(
			FVector Quaternion
		)
		{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)

			static const VectorF32 Constant1110 = { { { 1.0f, 1.0f, 1.0f, 0.0f } } };

			Vector Q0 = VectorAdd(Quaternion, Quaternion);
			Vector Q1 = VectorMultiply(Quaternion, Q0);

			Vector V0 = VectorPermute<XM_PERMUTE_0Y, XM_PERMUTE_0X, XM_PERMUTE_0X, XM_PERMUTE_1W>(Q1, Constant1110.v);
			Vector V1 = VectorPermute<XM_PERMUTE_0Z, XM_PERMUTE_0Z, XM_PERMUTE_0Y, XM_PERMUTE_1W>(Q1, Constant1110.v);
			Vector R0 = VectorSubtract(Constant1110, V0);
			R0 = VectorSubtract(R0, V1);

			V0 = VectorSwizzle<XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_W>(Quaternion);
			V1 = VectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_Z, XM_SWIZZLE_W>(Q0);
			V0 = VectorMultiply(V0, V1);

			V1 = VectorSplatW(Quaternion);
			Vector V2 = VectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_Z, XM_SWIZZLE_X, XM_SWIZZLE_W>(Q0);
			V1 = VectorMultiply(V1, V2);

			Vector R1 = VectorAdd(V0, V1);
			Vector R2 = VectorSubtract(V0, V1);

			V0 = VectorPermute<XM_PERMUTE_0Y, XM_PERMUTE_1X, XM_PERMUTE_1Y, XM_PERMUTE_0Z>(R1, R2);
			V1 = VectorPermute<XM_PERMUTE_0X, XM_PERMUTE_1Z, XM_PERMUTE_0X, XM_PERMUTE_1Z>(R1, R2);

			Matrix M;
			M.r[0] = VectorPermute<XM_PERMUTE_0X, XM_PERMUTE_1X, XM_PERMUTE_1Y, XM_PERMUTE_0W>(R0, V0);
			M.r[1] = VectorPermute<XM_PERMUTE_1Z, XM_PERMUTE_0Y, XM_PERMUTE_1W, XM_PERMUTE_0W>(R0, V0);
			M.r[2] = VectorPermute<XM_PERMUTE_1X, XM_PERMUTE_1Y, XM_PERMUTE_0Z, XM_PERMUTE_0W>(R0, V1);
			M.r[3] = g_XMIdentityR3.v;
			return M;

#elif defined(_XM_SSE_INTRINSICS_)
			static const VectorF32  Constant1110 = { { { 1.0f, 1.0f, 1.0f, 0.0f } } };

			Vector Q0 = _mm_add_ps(Quaternion, Quaternion);
			Vector Q1 = _mm_mul_ps(Quaternion, Q0);

			Vector V0 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(3, 0, 0, 1));
			V0 = _mm_and_ps(V0, g_XMMask3);
			Vector V1 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(3, 1, 2, 2));
			V1 = _mm_and_ps(V1, g_XMMask3);
			Vector R0 = _mm_sub_ps(Constant1110, V0);
			R0 = _mm_sub_ps(R0, V1);

			V0 = XM_PERMUTE_PS(Quaternion, _MM_SHUFFLE(3, 1, 0, 0));
			V1 = XM_PERMUTE_PS(Q0, _MM_SHUFFLE(3, 2, 1, 2));
			V0 = _mm_mul_ps(V0, V1);

			V1 = XM_PERMUTE_PS(Quaternion, _MM_SHUFFLE(3, 3, 3, 3));
			Vector V2 = XM_PERMUTE_PS(Q0, _MM_SHUFFLE(3, 0, 2, 1));
			V1 = _mm_mul_ps(V1, V2);

			Vector R1 = _mm_add_ps(V0, V1);
			Vector R2 = _mm_sub_ps(V0, V1);

			V0 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(1, 0, 2, 1));
			V0 = XM_PERMUTE_PS(V0, _MM_SHUFFLE(1, 3, 2, 0));
			V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 2, 0, 0));
			V1 = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 0, 2, 0));

			Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(1, 0, 3, 0));
			Q1 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 2, 0));

			Matrix M;
			M.r[0] = Q1;

			Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(3, 2, 3, 1));
			Q1 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 0, 2));
			M.r[1] = Q1;

			Q1 = _mm_shuffle_ps(V1, R0, _MM_SHUFFLE(3, 2, 1, 0));
			M.r[2] = Q1;
			M.r[3] = g_XMIdentityR3;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixTransformation2D
		(
			FVector ScalingOrigin,
			float     ScalingOrientation,
			FVector Scaling,
			FVector RotationOrigin,
			float     Rotation,
			GVector Translation
		)
		{
			// M = Inverse(MScalingOrigin) * Transpose(MScalingOrientation) * MScaling * MScalingOrientation *
			//         MScalingOrigin * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

			Vector VScalingOrigin = VectorSelect(g_XMSelect1100.v, ScalingOrigin, g_XMSelect1100.v);
			Vector NegScalingOrigin = VectorNegate(VScalingOrigin);

			Matrix MScalingOriginI = MatrixTranslationFromVector(NegScalingOrigin);
			Matrix MScalingOrientation = MatrixRotationZ(ScalingOrientation);
			Matrix MScalingOrientationT = MatrixTranspose(MScalingOrientation);
			Vector VScaling = VectorSelect(g_XMOne.v, Scaling, g_XMSelect1100.v);
			Matrix MScaling = MatrixScalingFromVector(VScaling);
			Vector VRotationOrigin = VectorSelect(g_XMSelect1100.v, RotationOrigin, g_XMSelect1100.v);
			Matrix MRotation = MatrixRotationZ(Rotation);
			Vector VTranslation = VectorSelect(g_XMSelect1100.v, Translation, g_XMSelect1100.v);

			Matrix M = MatrixMultiply(MScalingOriginI, MScalingOrientationT);
			M = MatrixMultiply(M, MScaling);
			M = MatrixMultiply(M, MScalingOrientation);
			M.r[3] = VectorAdd(M.r[3], VScalingOrigin);
			M.r[3] = VectorSubtract(M.r[3], VRotationOrigin);
			M = MatrixMultiply(M, MRotation);
			M.r[3] = VectorAdd(M.r[3], VRotationOrigin);
			M.r[3] = VectorAdd(M.r[3], VTranslation);

			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixTransformation
		(
			FVector ScalingOrigin,
			FVector ScalingOrientationQuaternion,
			FVector Scaling,
			GVector RotationOrigin,
			HVector RotationQuaternion,
			HVector Translation
		)
		{
			// M = Inverse(MScalingOrigin) * Transpose(MScalingOrientation) * MScaling * MScalingOrientation *
			//         MScalingOrigin * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

			Vector VScalingOrigin = VectorSelect(g_XMSelect1110.v, ScalingOrigin, g_XMSelect1110.v);
			Vector NegScalingOrigin = VectorNegate(ScalingOrigin);

			Matrix MScalingOriginI = MatrixTranslationFromVector(NegScalingOrigin);
			Matrix MScalingOrientation = MatrixRotationQuaternion(ScalingOrientationQuaternion);
			Matrix MScalingOrientationT = MatrixTranspose(MScalingOrientation);
			Matrix MScaling = MatrixScalingFromVector(Scaling);
			Vector VRotationOrigin = VectorSelect(g_XMSelect1110.v, RotationOrigin, g_XMSelect1110.v);
			Matrix MRotation = MatrixRotationQuaternion(RotationQuaternion);
			Vector VTranslation = VectorSelect(g_XMSelect1110.v, Translation, g_XMSelect1110.v);

			Matrix M;
			M = MatrixMultiply(MScalingOriginI, MScalingOrientationT);
			M = MatrixMultiply(M, MScaling);
			M = MatrixMultiply(M, MScalingOrientation);
			M.r[3] = VectorAdd(M.r[3], VScalingOrigin);
			M.r[3] = VectorSubtract(M.r[3], VRotationOrigin);
			M = MatrixMultiply(M, MRotation);
			M.r[3] = VectorAdd(M.r[3], VRotationOrigin);
			M.r[3] = VectorAdd(M.r[3], VTranslation);
			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixAffineTransformation2D
		(
			FVector Scaling,
			FVector RotationOrigin,
			float     Rotation,
			FVector Translation
		)
		{
			// M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

			Vector VScaling = VectorSelect(g_XMOne.v, Scaling, g_XMSelect1100.v);
			Matrix MScaling = MatrixScalingFromVector(VScaling);
			Vector VRotationOrigin = VectorSelect(g_XMSelect1100.v, RotationOrigin, g_XMSelect1100.v);
			Matrix MRotation = MatrixRotationZ(Rotation);
			Vector VTranslation = VectorSelect(g_XMSelect1100.v, Translation, g_XMSelect1100.v);

			Matrix M;
			M = MScaling;
			M.r[3] = VectorSubtract(M.r[3], VRotationOrigin);
			M = MatrixMultiply(M, MRotation);
			M.r[3] = VectorAdd(M.r[3], VRotationOrigin);
			M.r[3] = VectorAdd(M.r[3], VTranslation);
			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixAffineTransformation
		(
			FVector Scaling,
			FVector RotationOrigin,
			FVector RotationQuaternion,
			GVector Translation
		)
		{
			// M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

			Matrix MScaling = MatrixScalingFromVector(Scaling);
			Vector VRotationOrigin = VectorSelect(g_XMSelect1110.v, RotationOrigin, g_XMSelect1110.v);
			Matrix MRotation = MatrixRotationQuaternion(RotationQuaternion);
			Vector VTranslation = VectorSelect(g_XMSelect1110.v, Translation, g_XMSelect1110.v);

			Matrix M;
			M = MScaling;
			M.r[3] = VectorSubtract(M.r[3], VRotationOrigin);
			M = MatrixMultiply(M, MRotation);
			M.r[3] = VectorAdd(M.r[3], VRotationOrigin);
			M.r[3] = VectorAdd(M.r[3], VTranslation);
			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixReflect
		(
			FVector ReflectionPlane
		)
		{
			assert(!Vector3Equal(ReflectionPlane, VectorZero()));
			assert(!PlaneIsInfinite(ReflectionPlane));

			static const VectorF32 NegativeTwo = { { { -2.0f, -2.0f, -2.0f, 0.0f } } };

			Vector P = PlaneNormalize(ReflectionPlane);
			Vector S = VectorMultiply(P, NegativeTwo);

			Vector A = VectorSplatX(P);
			Vector B = VectorSplatY(P);
			Vector C = VectorSplatZ(P);
			Vector D = VectorSplatW(P);

			Matrix M;
			M.r[0] = VectorMultiplyAdd(A, S, g_XMIdentityR0.v);
			M.r[1] = VectorMultiplyAdd(B, S, g_XMIdentityR1.v);
			M.r[2] = VectorMultiplyAdd(C, S, g_XMIdentityR2.v);
			M.r[3] = VectorMultiplyAdd(D, S, g_XMIdentityR3.v);
			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixShadow
		(
			FVector ShadowPlane,
			FVector LightPosition
		)
		{
			static const VectorU32 Select0001 = { { { XM_SELECT_0, XM_SELECT_0, XM_SELECT_0, XM_SELECT_1 } } };

			assert(!Vector3Equal(ShadowPlane, VectorZero()));
			assert(!PlaneIsInfinite(ShadowPlane));

			Vector P = PlaneNormalize(ShadowPlane);
			Vector Dot = PlaneDot(P, LightPosition);
			P = VectorNegate(P);
			Vector D = VectorSplatW(P);
			Vector C = VectorSplatZ(P);
			Vector B = VectorSplatY(P);
			Vector A = VectorSplatX(P);
			Dot = VectorSelect(Select0001.v, Dot, Select0001.v);

			Matrix M;
			M.r[3] = VectorMultiplyAdd(D, LightPosition, Dot);
			Dot = VectorRotateLeft(Dot, 1);
			M.r[2] = VectorMultiplyAdd(C, LightPosition, Dot);
			Dot = VectorRotateLeft(Dot, 1);
			M.r[1] = VectorMultiplyAdd(B, LightPosition, Dot);
			Dot = VectorRotateLeft(Dot, 1);
			M.r[0] = VectorMultiplyAdd(A, LightPosition, Dot);
			return M;
		}

		//------------------------------------------------------------------------------
		// View and projection initialization operations
		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixLookAtLH
		(
			FVector EyePosition,
			FVector FocusPosition,
			FVector UpDirection
		)
		{
			Vector EyeDirection = VectorSubtract(FocusPosition, EyePosition);
			return MatrixLookToLH(EyePosition, EyeDirection, UpDirection);
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixLookAtRH
		(
			FVector EyePosition,
			FVector FocusPosition,
			FVector UpDirection
		)
		{
			Vector NegEyeDirection = VectorSubtract(EyePosition, FocusPosition);
			return MatrixLookToLH(EyePosition, NegEyeDirection, UpDirection);
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixLookToLH
		(
			FVector EyePosition,
			FVector EyeDirection,
			FVector UpDirection
		)
		{
			assert(!Vector3Equal(EyeDirection, VectorZero()));
			assert(!Vector3IsInfinite(EyeDirection));
			assert(!Vector3Equal(UpDirection, VectorZero()));
			assert(!Vector3IsInfinite(UpDirection));

			Vector R2 = Vector3Normalize(EyeDirection);

			Vector R0 = Vector3Cross(UpDirection, R2);
			R0 = Vector3Normalize(R0);

			Vector R1 = Vector3Cross(R2, R0);

			Vector NegEyePosition = VectorNegate(EyePosition);

			Vector D0 = Vector3Dot(R0, NegEyePosition);
			Vector D1 = Vector3Dot(R1, NegEyePosition);
			Vector D2 = Vector3Dot(R2, NegEyePosition);

			Matrix M;
			M.r[0] = VectorSelect(D0, R0, g_XMSelect1110.v);
			M.r[1] = VectorSelect(D1, R1, g_XMSelect1110.v);
			M.r[2] = VectorSelect(D2, R2, g_XMSelect1110.v);
			M.r[3] = g_XMIdentityR3.v;

			M = MatrixTranspose(M);

			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixLookToRH
		(
			FVector EyePosition,
			FVector EyeDirection,
			FVector UpDirection
		)
		{
			Vector NegEyeDirection = VectorNegate(EyeDirection);
			return MatrixLookToLH(EyePosition, NegEyeDirection, UpDirection);
		}

		//------------------------------------------------------------------------------

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable:28931, "PREfast noise: Esp:1266")
#endif

		inline Matrix XM_CALLCONV MatrixPerspectiveLH
		(
			float ViewWidth,
			float ViewHeight,
			float NearZ,
			float FarZ
		)
		{
			assert(NearZ > 0.f && FarZ > 0.f);
			assert(!ScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (FarZ - NearZ);

			Matrix M;
			M.m[0][0] = TwoNearZ / ViewWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = TwoNearZ / ViewHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = 1.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = -fRange * NearZ;
			M.m[3][3] = 0.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (FarZ - NearZ);
			const Vector Zero = vdupq_n_f32(0);
			Matrix M;
			M.r[0] = vsetq_lane_f32(TwoNearZ / ViewWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(TwoNearZ / ViewHeight, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, g_XMIdentityR3.v, 2);
			M.r[3] = vsetq_lane_f32(-fRange * NearZ, Zero, 2);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			Matrix M;
			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (FarZ - NearZ);
			// Note: This is recorded on the stack
			Vector rMem = {
				TwoNearZ / ViewWidth,
				TwoNearZ / ViewHeight,
				fRange,
				-fRange * NearZ
			};
			// Copy from memory to SSE register
			Vector vValues = rMem;
			Vector vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// TwoNearZ / ViewWidth,0,0,0
			M.r[0] = vTemp;
			// 0,TwoNearZ / ViewHeight,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_XMMaskY);
			M.r[1] = vTemp;
			// x=fRange,y=-fRange * NearZ,0,1.0f
			vValues = _mm_shuffle_ps(vValues, g_XMIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
			// 0,0,fRange,1.0f
			vTemp = _mm_setzero_ps();
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
			M.r[2] = vTemp;
			// 0,0,-fRange * NearZ,0
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
			M.r[3] = vTemp;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixPerspectiveRH
		(
			float ViewWidth,
			float ViewHeight,
			float NearZ,
			float FarZ
		)
		{
			assert(NearZ > 0.f && FarZ > 0.f);
			assert(!ScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (NearZ - FarZ);

			Matrix M;
			M.m[0][0] = TwoNearZ / ViewWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = TwoNearZ / ViewHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = -1.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = fRange * NearZ;
			M.m[3][3] = 0.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (NearZ - FarZ);
			const Vector Zero = vdupq_n_f32(0);

			Matrix M;
			M.r[0] = vsetq_lane_f32(TwoNearZ / ViewWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(TwoNearZ / ViewHeight, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, g_XMNegIdentityR3.v, 2);
			M.r[3] = vsetq_lane_f32(fRange * NearZ, Zero, 2);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			Matrix M;
			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (NearZ - FarZ);
			// Note: This is recorded on the stack
			Vector rMem = {
				TwoNearZ / ViewWidth,
				TwoNearZ / ViewHeight,
				fRange,
				fRange * NearZ
			};
			// Copy from memory to SSE register
			Vector vValues = rMem;
			Vector vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// TwoNearZ / ViewWidth,0,0,0
			M.r[0] = vTemp;
			// 0,TwoNearZ / ViewHeight,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_XMMaskY);
			M.r[1] = vTemp;
			// x=fRange,y=-fRange * NearZ,0,-1.0f
			vValues = _mm_shuffle_ps(vValues, g_XMNegIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
			// 0,0,fRange,-1.0f
			vTemp = _mm_setzero_ps();
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
			M.r[2] = vTemp;
			// 0,0,-fRange * NearZ,0
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
			M.r[3] = vTemp;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixPerspectiveFovLH
		(
			float FovAngleY,
			float AspectRatio,
			float NearZ,
			float FarZ
		)
		{
			assert(NearZ > 0.f && FarZ > 0.f);
			assert(!ScalarNearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
			assert(!ScalarNearEqual(AspectRatio, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

			float    SinFov;
			float    CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

			float Height = CosFov / SinFov;
			float Width = Height / AspectRatio;
			float fRange = FarZ / (FarZ - NearZ);

			Matrix M;
			M.m[0][0] = Width;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = Height;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = 1.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = -fRange * NearZ;
			M.m[3][3] = 0.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float    SinFov;
			float    CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

			float fRange = FarZ / (FarZ - NearZ);
			float Height = CosFov / SinFov;
			float Width = Height / AspectRatio;
			const Vector Zero = vdupq_n_f32(0);

			Matrix M;
			M.r[0] = vsetq_lane_f32(Width, Zero, 0);
			M.r[1] = vsetq_lane_f32(Height, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, g_XMIdentityR3.v, 2);
			M.r[3] = vsetq_lane_f32(-fRange * NearZ, Zero, 2);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			float    SinFov;
			float    CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

			float fRange = FarZ / (FarZ - NearZ);
			// Note: This is recorded on the stack
			float Height = CosFov / SinFov;
			Vector rMem = {
				Height / AspectRatio,
				Height,
				fRange,
				-fRange * NearZ
			};
			// Copy from memory to SSE register
			Vector vValues = rMem;
			Vector vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// CosFov / SinFov,0,0,0
			Matrix M;
			M.r[0] = vTemp;
			// 0,Height / AspectRatio,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_XMMaskY);
			M.r[1] = vTemp;
			// x=fRange,y=-fRange * NearZ,0,1.0f
			vTemp = _mm_setzero_ps();
			vValues = _mm_shuffle_ps(vValues, g_XMIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
			// 0,0,fRange,1.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
			M.r[2] = vTemp;
			// 0,0,-fRange * NearZ,0.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
			M.r[3] = vTemp;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixPerspectiveFovRH
		(
			float FovAngleY,
			float AspectRatio,
			float NearZ,
			float FarZ
		)
		{
			assert(NearZ > 0.f && FarZ > 0.f);
			assert(!ScalarNearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
			assert(!ScalarNearEqual(AspectRatio, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

			float    SinFov;
			float    CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

			float Height = CosFov / SinFov;
			float Width = Height / AspectRatio;
			float fRange = FarZ / (NearZ - FarZ);

			Matrix M;
			M.m[0][0] = Width;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = Height;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = -1.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = fRange * NearZ;
			M.m[3][3] = 0.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float    SinFov;
			float    CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);
			float fRange = FarZ / (NearZ - FarZ);
			float Height = CosFov / SinFov;
			float Width = Height / AspectRatio;
			const Vector Zero = vdupq_n_f32(0);

			Matrix M;
			M.r[0] = vsetq_lane_f32(Width, Zero, 0);
			M.r[1] = vsetq_lane_f32(Height, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, g_XMNegIdentityR3.v, 2);
			M.r[3] = vsetq_lane_f32(fRange * NearZ, Zero, 2);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			float    SinFov;
			float    CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);
			float fRange = FarZ / (NearZ - FarZ);
			// Note: This is recorded on the stack
			float Height = CosFov / SinFov;
			Vector rMem = {
				Height / AspectRatio,
				Height,
				fRange,
				fRange * NearZ
			};
			// Copy from memory to SSE register
			Vector vValues = rMem;
			Vector vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// CosFov / SinFov,0,0,0
			Matrix M;
			M.r[0] = vTemp;
			// 0,Height / AspectRatio,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_XMMaskY);
			M.r[1] = vTemp;
			// x=fRange,y=-fRange * NearZ,0,-1.0f
			vTemp = _mm_setzero_ps();
			vValues = _mm_shuffle_ps(vValues, g_XMNegIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
			// 0,0,fRange,-1.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
			M.r[2] = vTemp;
			// 0,0,fRange * NearZ,0.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
			M.r[3] = vTemp;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixPerspectiveOffCenterLH
		(
			float ViewLeft,
			float ViewRight,
			float ViewBottom,
			float ViewTop,
			float NearZ,
			float FarZ
		)
		{
			assert(NearZ > 0.f && FarZ > 0.f);
			assert(!ScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
			assert(!ScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (FarZ - NearZ);

			Matrix M;
			M.m[0][0] = TwoNearZ * ReciprocalWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = TwoNearZ * ReciprocalHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = -(ViewLeft + ViewRight) * ReciprocalWidth;
			M.m[2][1] = -(ViewTop + ViewBottom) * ReciprocalHeight;
			M.m[2][2] = fRange;
			M.m[2][3] = 1.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = -fRange * NearZ;
			M.m[3][3] = 0.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (FarZ - NearZ);
			const Vector Zero = vdupq_n_f32(0);

			Matrix M;
			M.r[0] = vsetq_lane_f32(TwoNearZ * ReciprocalWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(TwoNearZ * ReciprocalHeight, Zero, 1);
			M.r[2] = VectorSet(-(ViewLeft + ViewRight) * ReciprocalWidth,
				-(ViewTop + ViewBottom) * ReciprocalHeight,
				fRange,
				1.0f);
			M.r[3] = vsetq_lane_f32(-fRange * NearZ, Zero, 2);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			Matrix M;
			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (FarZ - NearZ);
			// Note: This is recorded on the stack
			Vector rMem = {
				TwoNearZ * ReciprocalWidth,
				TwoNearZ * ReciprocalHeight,
				-fRange * NearZ,
				0
			};
			// Copy from memory to SSE register
			Vector vValues = rMem;
			Vector vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// TwoNearZ*ReciprocalWidth,0,0,0
			M.r[0] = vTemp;
			// 0,TwoNearZ*ReciprocalHeight,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_XMMaskY);
			M.r[1] = vTemp;
			// 0,0,fRange,1.0f
			M.r[2] = VectorSet(-(ViewLeft + ViewRight) * ReciprocalWidth,
				-(ViewTop + ViewBottom) * ReciprocalHeight,
				fRange,
				1.0f);
			// 0,0,-fRange * NearZ,0.0f
			vValues = _mm_and_ps(vValues, g_XMMaskZ);
			M.r[3] = vValues;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixPerspectiveOffCenterRH
		(
			float ViewLeft,
			float ViewRight,
			float ViewBottom,
			float ViewTop,
			float NearZ,
			float FarZ
		)
		{
			assert(NearZ > 0.f && FarZ > 0.f);
			assert(!ScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
			assert(!ScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (NearZ - FarZ);

			Matrix M;
			M.m[0][0] = TwoNearZ * ReciprocalWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = TwoNearZ * ReciprocalHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = (ViewLeft + ViewRight) * ReciprocalWidth;
			M.m[2][1] = (ViewTop + ViewBottom) * ReciprocalHeight;
			M.m[2][2] = fRange;
			M.m[2][3] = -1.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = fRange * NearZ;
			M.m[3][3] = 0.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (NearZ - FarZ);
			const Vector Zero = vdupq_n_f32(0);

			Matrix M;
			M.r[0] = vsetq_lane_f32(TwoNearZ * ReciprocalWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(TwoNearZ * ReciprocalHeight, Zero, 1);
			M.r[2] = VectorSet((ViewLeft + ViewRight) * ReciprocalWidth,
				(ViewTop + ViewBottom) * ReciprocalHeight,
				fRange,
				-1.0f);
			M.r[3] = vsetq_lane_f32(fRange * NearZ, Zero, 2);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			Matrix M;
			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (NearZ - FarZ);
			// Note: This is recorded on the stack
			Vector rMem = {
				TwoNearZ * ReciprocalWidth,
				TwoNearZ * ReciprocalHeight,
				fRange * NearZ,
				0
			};
			// Copy from memory to SSE register
			Vector vValues = rMem;
			Vector vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// TwoNearZ*ReciprocalWidth,0,0,0
			M.r[0] = vTemp;
			// 0,TwoNearZ*ReciprocalHeight,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_XMMaskY);
			M.r[1] = vTemp;
			// 0,0,fRange,1.0f
			M.r[2] = VectorSet((ViewLeft + ViewRight) * ReciprocalWidth,
				(ViewTop + ViewBottom) * ReciprocalHeight,
				fRange,
				-1.0f);
			// 0,0,-fRange * NearZ,0.0f
			vValues = _mm_and_ps(vValues, g_XMMaskZ);
			M.r[3] = vValues;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixOrthographicLH
		(
			float ViewWidth,
			float ViewHeight,
			float NearZ,
			float FarZ
		)
		{
			assert(!ScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

			float fRange = 1.0f / (FarZ - NearZ);

			Matrix M;
			M.m[0][0] = 2.0f / ViewWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = 2.0f / ViewHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = -fRange * NearZ;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float fRange = 1.0f / (FarZ - NearZ);

			const Vector Zero = vdupq_n_f32(0);
			Matrix M;
			M.r[0] = vsetq_lane_f32(2.0f / ViewWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(2.0f / ViewHeight, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, Zero, 2);
			M.r[3] = vsetq_lane_f32(-fRange * NearZ, g_XMIdentityR3.v, 2);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			Matrix M;
			float fRange = 1.0f / (FarZ - NearZ);
			// Note: This is recorded on the stack
			Vector rMem = {
				2.0f / ViewWidth,
				2.0f / ViewHeight,
				fRange,
				-fRange * NearZ
			};
			// Copy from memory to SSE register
			Vector vValues = rMem;
			Vector vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// 2.0f / ViewWidth,0,0,0
			M.r[0] = vTemp;
			// 0,2.0f / ViewHeight,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_XMMaskY);
			M.r[1] = vTemp;
			// x=fRange,y=-fRange * NearZ,0,1.0f
			vTemp = _mm_setzero_ps();
			vValues = _mm_shuffle_ps(vValues, g_XMIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
			// 0,0,fRange,0.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
			M.r[2] = vTemp;
			// 0,0,-fRange * NearZ,1.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
			M.r[3] = vTemp;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixOrthographicRH
		(
			float ViewWidth,
			float ViewHeight,
			float NearZ,
			float FarZ
		)
		{
			assert(!ScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

			float fRange = 1.0f / (NearZ - FarZ);

			Matrix M;
			M.m[0][0] = 2.0f / ViewWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = 2.0f / ViewHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = fRange * NearZ;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float fRange = 1.0f / (NearZ - FarZ);

			const Vector Zero = vdupq_n_f32(0);
			Matrix M;
			M.r[0] = vsetq_lane_f32(2.0f / ViewWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(2.0f / ViewHeight, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, Zero, 2);
			M.r[3] = vsetq_lane_f32(fRange * NearZ, g_XMIdentityR3.v, 2);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			Matrix M;
			float fRange = 1.0f / (NearZ - FarZ);
			// Note: This is recorded on the stack
			Vector rMem = {
				2.0f / ViewWidth,
				2.0f / ViewHeight,
				fRange,
				fRange * NearZ
			};
			// Copy from memory to SSE register
			Vector vValues = rMem;
			Vector vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// 2.0f / ViewWidth,0,0,0
			M.r[0] = vTemp;
			// 0,2.0f / ViewHeight,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_XMMaskY);
			M.r[1] = vTemp;
			// x=fRange,y=fRange * NearZ,0,1.0f
			vTemp = _mm_setzero_ps();
			vValues = _mm_shuffle_ps(vValues, g_XMIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
			// 0,0,fRange,0.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
			M.r[2] = vTemp;
			// 0,0,fRange * NearZ,1.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
			M.r[3] = vTemp;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixOrthographicOffCenterLH
		(
			float ViewLeft,
			float ViewRight,
			float ViewBottom,
			float ViewTop,
			float NearZ,
			float FarZ
		)
		{
			assert(!ScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
			assert(!ScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (FarZ - NearZ);

			Matrix M;
			M.m[0][0] = ReciprocalWidth + ReciprocalWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = ReciprocalHeight + ReciprocalHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = 0.0f;

			M.m[3][0] = -(ViewLeft + ViewRight) * ReciprocalWidth;
			M.m[3][1] = -(ViewTop + ViewBottom) * ReciprocalHeight;
			M.m[3][2] = -fRange * NearZ;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (FarZ - NearZ);
			const Vector Zero = vdupq_n_f32(0);
			Matrix M;
			M.r[0] = vsetq_lane_f32(ReciprocalWidth + ReciprocalWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(ReciprocalHeight + ReciprocalHeight, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, Zero, 2);
			M.r[3] = VectorSet(-(ViewLeft + ViewRight) * ReciprocalWidth,
				-(ViewTop + ViewBottom) * ReciprocalHeight,
				-fRange * NearZ,
				1.0f);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			Matrix M;
			float fReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float fReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (FarZ - NearZ);
			// Note: This is recorded on the stack
			Vector rMem = {
				fReciprocalWidth,
				fReciprocalHeight,
				fRange,
				1.0f
			};
			Vector rMem2 = {
				-(ViewLeft + ViewRight),
				-(ViewTop + ViewBottom),
				-NearZ,
				1.0f
			};
			// Copy from memory to SSE register
			Vector vValues = rMem;
			Vector vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// fReciprocalWidth*2,0,0,0
			vTemp = _mm_add_ss(vTemp, vTemp);
			M.r[0] = vTemp;
			// 0,fReciprocalHeight*2,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_XMMaskY);
			vTemp = _mm_add_ps(vTemp, vTemp);
			M.r[1] = vTemp;
			// 0,0,fRange,0.0f
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_XMMaskZ);
			M.r[2] = vTemp;
			// -(ViewLeft + ViewRight)*fReciprocalWidth,-(ViewTop + ViewBottom)*fReciprocalHeight,fRange*-NearZ,1.0f
			vValues = _mm_mul_ps(vValues, rMem2);
			M.r[3] = vValues;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV MatrixOrthographicOffCenterRH
		(
			float ViewLeft,
			float ViewRight,
			float ViewBottom,
			float ViewTop,
			float NearZ,
			float FarZ
		)
		{
			assert(!ScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
			assert(!ScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (NearZ - FarZ);

			Matrix M;
			M.m[0][0] = ReciprocalWidth + ReciprocalWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = ReciprocalHeight + ReciprocalHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = 0.0f;

			M.r[3] = VectorSet(-(ViewLeft + ViewRight) * ReciprocalWidth,
				-(ViewTop + ViewBottom) * ReciprocalHeight,
				fRange * NearZ,
				1.0f);
			return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (NearZ - FarZ);
			const Vector Zero = vdupq_n_f32(0);
			Matrix M;
			M.r[0] = vsetq_lane_f32(ReciprocalWidth + ReciprocalWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(ReciprocalHeight + ReciprocalHeight, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, Zero, 2);
			M.r[3] = VectorSet(-(ViewLeft + ViewRight) * ReciprocalWidth,
				-(ViewTop + ViewBottom) * ReciprocalHeight,
				fRange * NearZ,
				1.0f);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			Matrix M;
			float fReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float fReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (NearZ - FarZ);
			// Note: This is recorded on the stack
			Vector rMem = {
				fReciprocalWidth,
				fReciprocalHeight,
				fRange,
				1.0f
			};
			Vector rMem2 = {
				-(ViewLeft + ViewRight),
				-(ViewTop + ViewBottom),
				NearZ,
				1.0f
			};
			// Copy from memory to SSE register
			Vector vValues = rMem;
			Vector vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// fReciprocalWidth*2,0,0,0
			vTemp = _mm_add_ss(vTemp, vTemp);
			M.r[0] = vTemp;
			// 0,fReciprocalHeight*2,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_XMMaskY);
			vTemp = _mm_add_ps(vTemp, vTemp);
			M.r[1] = vTemp;
			// 0,0,fRange,0.0f
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_XMMaskZ);
			M.r[2] = vTemp;
			// -(ViewLeft + ViewRight)*fReciprocalWidth,-(ViewTop + ViewBottom)*fReciprocalHeight,fRange*-NearZ,1.0f
			vValues = _mm_mul_ps(vValues, rMem2);
			M.r[3] = vValues;
			return M;
#endif
		}

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

		/****************************************************************************
		 *
		 * Matrix operators and methods
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------

		inline Matrix::Matrix
		(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
		)
		{
			r[0] = VectorSet(m00, m01, m02, m03);
			r[1] = VectorSet(m10, m11, m12, m13);
			r[2] = VectorSet(m20, m21, m22, m23);
			r[3] = VectorSet(m30, m31, m32, m33);
		}

		//------------------------------------------------------------------------------

		inline Matrix::Matrix
		(
			const float* pArray
		)
		{
			assert(pArray != nullptr);
			r[0] = LoadFloat4(reinterpret_cast<const Float4*>(pArray));
			r[1] = LoadFloat4(reinterpret_cast<const Float4*>(pArray + 4));
			r[2] = LoadFloat4(reinterpret_cast<const Float4*>(pArray + 8));
			r[3] = LoadFloat4(reinterpret_cast<const Float4*>(pArray + 12));
		}

		//------------------------------------------------------------------------------

		inline Matrix Matrix::operator- () const
		{
			Matrix R;
			R.r[0] = VectorNegate(r[0]);
			R.r[1] = VectorNegate(r[1]);
			R.r[2] = VectorNegate(r[2]);
			R.r[3] = VectorNegate(r[3]);
			return R;
		}

		//------------------------------------------------------------------------------

		inline Matrix& XM_CALLCONV Matrix::operator+= (FMatrix M)
		{
			r[0] = VectorAdd(r[0], M.r[0]);
			r[1] = VectorAdd(r[1], M.r[1]);
			r[2] = VectorAdd(r[2], M.r[2]);
			r[3] = VectorAdd(r[3], M.r[3]);
			return *this;
		}

		//------------------------------------------------------------------------------

		inline Matrix& XM_CALLCONV Matrix::operator-= (FMatrix M)
		{
			r[0] = VectorSubtract(r[0], M.r[0]);
			r[1] = VectorSubtract(r[1], M.r[1]);
			r[2] = VectorSubtract(r[2], M.r[2]);
			r[3] = VectorSubtract(r[3], M.r[3]);
			return *this;
		}

		//------------------------------------------------------------------------------

		inline Matrix& XM_CALLCONV Matrix::operator*=(FMatrix M)
		{
			*this = MatrixMultiply(*this, M);
			return *this;
		}

		//------------------------------------------------------------------------------

		inline Matrix& Matrix::operator*= (float S)
		{
			r[0] = VectorScale(r[0], S);
			r[1] = VectorScale(r[1], S);
			r[2] = VectorScale(r[2], S);
			r[3] = VectorScale(r[3], S);
			return *this;
		}

		//------------------------------------------------------------------------------

		inline Matrix& Matrix::operator/= (float S)
		{
#if defined(_XM_NO_INTRINSICS_)
			Vector vS = VectorReplicate(S);
			r[0] = VectorDivide(r[0], vS);
			r[1] = VectorDivide(r[1], vS);
			r[2] = VectorDivide(r[2], vS);
			r[3] = VectorDivide(r[3], vS);
			return *this;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			float32x4_t vS = vdupq_n_f32(S);
			r[0] = vdivq_f32(r[0], vS);
			r[1] = vdivq_f32(r[1], vS);
			r[2] = vdivq_f32(r[2], vS);
			r[3] = vdivq_f32(r[3], vS);
#else
			// 2 iterations of Newton-Raphson refinement of reciprocal
			float32x2_t vS = vdup_n_f32(S);
			float32x2_t R0 = vrecpe_f32(vS);
			float32x2_t S0 = vrecps_f32(R0, vS);
			R0 = vmul_f32(S0, R0);
			S0 = vrecps_f32(R0, vS);
			R0 = vmul_f32(S0, R0);
			float32x4_t Reciprocal = vcombine_u32(R0, R0);
			r[0] = vmulq_f32(r[0], Reciprocal);
			r[1] = vmulq_f32(r[1], Reciprocal);
			r[2] = vmulq_f32(r[2], Reciprocal);
			r[3] = vmulq_f32(r[3], Reciprocal);
#endif
			return *this;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 vS = _mm_set_ps1(S);
			r[0] = _mm_div_ps(r[0], vS);
			r[1] = _mm_div_ps(r[1], vS);
			r[2] = _mm_div_ps(r[2], vS);
			r[3] = _mm_div_ps(r[3], vS);
			return *this;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV Matrix::operator+ (FMatrix M) const
		{
			Matrix R;
			R.r[0] = VectorAdd(r[0], M.r[0]);
			R.r[1] = VectorAdd(r[1], M.r[1]);
			R.r[2] = VectorAdd(r[2], M.r[2]);
			R.r[3] = VectorAdd(r[3], M.r[3]);
			return R;
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV Matrix::operator- (FMatrix M) const
		{
			Matrix R;
			R.r[0] = VectorSubtract(r[0], M.r[0]);
			R.r[1] = VectorSubtract(r[1], M.r[1]);
			R.r[2] = VectorSubtract(r[2], M.r[2]);
			R.r[3] = VectorSubtract(r[3], M.r[3]);
			return R;
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV Matrix::operator*(FMatrix M) const
		{
			return MatrixMultiply(*this, M);
		}

		//------------------------------------------------------------------------------

		inline Matrix Matrix::operator* (float S) const
		{
			Matrix R;
			R.r[0] = VectorScale(r[0], S);
			R.r[1] = VectorScale(r[1], S);
			R.r[2] = VectorScale(r[2], S);
			R.r[3] = VectorScale(r[3], S);
			return R;
		}

		//------------------------------------------------------------------------------

		inline Matrix Matrix::operator/ (float S) const
		{
#if defined(_XM_NO_INTRINSICS_)
			Vector vS = VectorReplicate(S);
			Matrix R;
			R.r[0] = VectorDivide(r[0], vS);
			R.r[1] = VectorDivide(r[1], vS);
			R.r[2] = VectorDivide(r[2], vS);
			R.r[3] = VectorDivide(r[3], vS);
			return R;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			float32x4_t vS = vdupq_n_f32(S);
			Matrix R;
			R.r[0] = vdivq_f32(r[0], vS);
			R.r[1] = vdivq_f32(r[1], vS);
			R.r[2] = vdivq_f32(r[2], vS);
			R.r[3] = vdivq_f32(r[3], vS);
#else
			// 2 iterations of Newton-Raphson refinement of reciprocal
			float32x2_t vS = vdup_n_f32(S);
			float32x2_t R0 = vrecpe_f32(vS);
			float32x2_t S0 = vrecps_f32(R0, vS);
			R0 = vmul_f32(S0, R0);
			S0 = vrecps_f32(R0, vS);
			R0 = vmul_f32(S0, R0);
			float32x4_t Reciprocal = vcombine_u32(R0, R0);
			Matrix R;
			R.r[0] = vmulq_f32(r[0], Reciprocal);
			R.r[1] = vmulq_f32(r[1], Reciprocal);
			R.r[2] = vmulq_f32(r[2], Reciprocal);
			R.r[3] = vmulq_f32(r[3], Reciprocal);
#endif
			return R;
#elif defined(_XM_SSE_INTRINSICS_)
			__m128 vS = _mm_set_ps1(S);
			Matrix R;
			R.r[0] = _mm_div_ps(r[0], vS);
			R.r[1] = _mm_div_ps(r[1], vS);
			R.r[2] = _mm_div_ps(r[2], vS);
			R.r[3] = _mm_div_ps(r[3], vS);
			return R;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix XM_CALLCONV operator*
			(
				float S,
				FMatrix M
				)
		{
			Matrix R;
			R.r[0] = VectorScale(M.r[0], S);
			R.r[1] = VectorScale(M.r[1], S);
			R.r[2] = VectorScale(M.r[2], S);
			R.r[3] = VectorScale(M.r[3], S);
			return R;
		}

		/****************************************************************************
		 *
		 * Float3X3 operators
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------

		inline Float3X3::Float3X3
		(
			const float* pArray
		)
		{
			assert(pArray != nullptr);
			for (size_t Row = 0; Row < 3; Row++)
			{
				for (size_t Column = 0; Column < 3; Column++)
				{
					m[Row][Column] = pArray[Row * 3 + Column];
				}
			}
		}

		/****************************************************************************
		 *
		 * Float4X3 operators
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------

		inline Float4X3::Float4X3
		(
			const float* pArray
		)
		{
			assert(pArray != nullptr);

			m[0][0] = pArray[0];
			m[0][1] = pArray[1];
			m[0][2] = pArray[2];

			m[1][0] = pArray[3];
			m[1][1] = pArray[4];
			m[1][2] = pArray[5];

			m[2][0] = pArray[6];
			m[2][1] = pArray[7];
			m[2][2] = pArray[8];

			m[3][0] = pArray[9];
			m[3][1] = pArray[10];
			m[3][2] = pArray[11];
		}

		/****************************************************************************
		*
		* Float3X4 operators
		*
		****************************************************************************/

		//------------------------------------------------------------------------------

		inline Float3X4::Float3X4
		(
			const float* pArray
		)
		{
			assert(pArray != nullptr);

			m[0][0] = pArray[0];
			m[0][1] = pArray[1];
			m[0][2] = pArray[2];
			m[0][3] = pArray[3];

			m[1][0] = pArray[4];
			m[1][1] = pArray[5];
			m[1][2] = pArray[6];
			m[1][3] = pArray[7];

			m[2][0] = pArray[8];
			m[2][1] = pArray[9];
			m[2][2] = pArray[10];
			m[2][3] = pArray[11];
		}

		/****************************************************************************
		 *
		 * Float4X4 operators
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------

		inline Float4X4::Float4X4
		(
			const float* pArray
		)
		{
			assert(pArray != nullptr);

			m[0][0] = pArray[0];
			m[0][1] = pArray[1];
			m[0][2] = pArray[2];
			m[0][3] = pArray[3];

			m[1][0] = pArray[4];
			m[1][1] = pArray[5];
			m[1][2] = pArray[6];
			m[1][3] = pArray[7];

			m[2][0] = pArray[8];
			m[2][1] = pArray[9];
			m[2][2] = pArray[10];
			m[2][3] = pArray[11];

			m[3][0] = pArray[12];
			m[3][1] = pArray[13];
			m[3][2] = pArray[14];
			m[3][3] = pArray[15];
		}


		//-------------------------------------------------------------------------------------
		// DirectXMathMisc.inl -- SIMD C++ Math library
		//
		// Copyright (c) Microsoft Corporation. All rights reserved.
		// Licensed under the MIT License.
		//
		// http://go.microsoft.com/fwlink/?LinkID=615560
		//-------------------------------------------------------------------------------------


		/****************************************************************************
		 *
		 * Quaternion
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------
		 // Comparison operations
		 //------------------------------------------------------------------------------

		 //------------------------------------------------------------------------------

		inline bool XM_CALLCONV QuaternionEqual
		(
			FVector Q1,
			FVector Q2
		)
		{
			return Vector4Equal(Q1, Q2);
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV QuaternionNotEqual
		(
			FVector Q1,
			FVector Q2
		)
		{
			return Vector4NotEqual(Q1, Q2);
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV QuaternionIsNaN
		(
			FVector Q
		)
		{
			return Vector4IsNaN(Q);
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV QuaternionIsInfinite
		(
			FVector Q
		)
		{
			return Vector4IsInfinite(Q);
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV QuaternionIsIdentity
		(
			FVector Q
		)
		{
			return Vector4Equal(Q, g_XMIdentityR3.v);
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionDot
		(
			FVector Q1,
			FVector Q2
		)
		{
			return Vector4Dot(Q1, Q2);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionMultiply
		(
			FVector Q1,
			FVector Q2
		)
		{
			// Returns the product Q2*Q1 (which is the concatenation of a rotation Q1 followed by the rotation Q2)

			// [ (Q2.w * Q1.x) + (Q2.x * Q1.w) + (Q2.y * Q1.z) - (Q2.z * Q1.y),
			//   (Q2.w * Q1.y) - (Q2.x * Q1.z) + (Q2.y * Q1.w) + (Q2.z * Q1.x),
			//   (Q2.w * Q1.z) + (Q2.x * Q1.y) - (Q2.y * Q1.x) + (Q2.z * Q1.w),
			//   (Q2.w * Q1.w) - (Q2.x * Q1.x) - (Q2.y * Q1.y) - (Q2.z * Q1.z) ]

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					(Q2.vector4_f32[3] * Q1.vector4_f32[0]) + (Q2.vector4_f32[0] * Q1.vector4_f32[3]) + (Q2.vector4_f32[1] * Q1.vector4_f32[2]) - (Q2.vector4_f32[2] * Q1.vector4_f32[1]),
					(Q2.vector4_f32[3] * Q1.vector4_f32[1]) - (Q2.vector4_f32[0] * Q1.vector4_f32[2]) + (Q2.vector4_f32[1] * Q1.vector4_f32[3]) + (Q2.vector4_f32[2] * Q1.vector4_f32[0]),
					(Q2.vector4_f32[3] * Q1.vector4_f32[2]) + (Q2.vector4_f32[0] * Q1.vector4_f32[1]) - (Q2.vector4_f32[1] * Q1.vector4_f32[0]) + (Q2.vector4_f32[2] * Q1.vector4_f32[3]),
					(Q2.vector4_f32[3] * Q1.vector4_f32[3]) - (Q2.vector4_f32[0] * Q1.vector4_f32[0]) - (Q2.vector4_f32[1] * Q1.vector4_f32[1]) - (Q2.vector4_f32[2] * Q1.vector4_f32[2])
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			static const VectorF32 ControlWZYX = { { { 1.0f, -1.0f, 1.0f, -1.0f } } };
			static const VectorF32 ControlZWXY = { { { 1.0f, 1.0f, -1.0f, -1.0f } } };
			static const VectorF32 ControlYXWZ = { { { -1.0f, 1.0f, 1.0f, -1.0f } } };

			float32x2_t Q2L = vget_low_f32(Q2);
			float32x2_t Q2H = vget_high_f32(Q2);

			float32x4_t Q2X = vdupq_lane_f32(Q2L, 0);
			float32x4_t Q2Y = vdupq_lane_f32(Q2L, 1);
			float32x4_t Q2Z = vdupq_lane_f32(Q2H, 0);
			Vector vResult = vmulq_lane_f32(Q1, Q2H, 1);

			// Mul by Q1WZYX
			float32x4_t vTemp = vrev64q_f32(Q1);
			vTemp = vcombine_f32(vget_high_f32(vTemp), vget_low_f32(vTemp));
			Q2X = vmulq_f32(Q2X, vTemp);
			vResult = vmlaq_f32(vResult, Q2X, ControlWZYX);

			// Mul by Q1ZWXY
			vTemp = vrev64q_u32(vTemp);
			Q2Y = vmulq_f32(Q2Y, vTemp);
			vResult = vmlaq_f32(vResult, Q2Y, ControlZWXY);

			// Mul by Q1YXWZ
			vTemp = vrev64q_u32(vTemp);
			vTemp = vcombine_f32(vget_high_f32(vTemp), vget_low_f32(vTemp));
			Q2Z = vmulq_f32(Q2Z, vTemp);
			vResult = vmlaq_f32(vResult, Q2Z, ControlYXWZ);
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			static const VectorF32 ControlWZYX = { { { 1.0f, -1.0f, 1.0f, -1.0f } } };
			static const VectorF32 ControlZWXY = { { { 1.0f, 1.0f, -1.0f, -1.0f } } };
			static const VectorF32 ControlYXWZ = { { { -1.0f, 1.0f, 1.0f, -1.0f } } };
			// Copy to SSE registers and use as few as possible for x86
			Vector Q2X = Q2;
			Vector Q2Y = Q2;
			Vector Q2Z = Q2;
			Vector vResult = Q2;
			// Splat with one instruction
			vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 3, 3, 3));
			Q2X = XM_PERMUTE_PS(Q2X, _MM_SHUFFLE(0, 0, 0, 0));
			Q2Y = XM_PERMUTE_PS(Q2Y, _MM_SHUFFLE(1, 1, 1, 1));
			Q2Z = XM_PERMUTE_PS(Q2Z, _MM_SHUFFLE(2, 2, 2, 2));
			// Retire Q1 and perform Q1*Q2W
			vResult = _mm_mul_ps(vResult, Q1);
			Vector Q1Shuffle = Q1;
			// Shuffle the copies of Q1
			Q1Shuffle = XM_PERMUTE_PS(Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
			// Mul by Q1WZYX
			Q2X = _mm_mul_ps(Q2X, Q1Shuffle);
			Q1Shuffle = XM_PERMUTE_PS(Q1Shuffle, _MM_SHUFFLE(2, 3, 0, 1));
			// Flip the signs on y and z
			Q2X = _mm_mul_ps(Q2X, ControlWZYX);
			// Mul by Q1ZWXY
			Q2Y = _mm_mul_ps(Q2Y, Q1Shuffle);
			Q1Shuffle = XM_PERMUTE_PS(Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
			// Flip the signs on z and w
			Q2Y = _mm_mul_ps(Q2Y, ControlZWXY);
			// Mul by Q1YXWZ
			Q2Z = _mm_mul_ps(Q2Z, Q1Shuffle);
			vResult = _mm_add_ps(vResult, Q2X);
			// Flip the signs on x and w
			Q2Z = _mm_mul_ps(Q2Z, ControlYXWZ);
			Q2Y = _mm_add_ps(Q2Y, Q2Z);
			vResult = _mm_add_ps(vResult, Q2Y);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionLengthSq
		(
			FVector Q
		)
		{
			return Vector4LengthSq(Q);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionReciprocalLength
		(
			FVector Q
		)
		{
			return Vector4ReciprocalLength(Q);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionLength
		(
			FVector Q
		)
		{
			return Vector4Length(Q);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionNormalizeEst
		(
			FVector Q
		)
		{
			return Vector4NormalizeEst(Q);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionNormalize
		(
			FVector Q
		)
		{
			return Vector4Normalize(Q);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionConjugate
		(
			FVector Q
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 Result = { { {
					-Q.vector4_f32[0],
					-Q.vector4_f32[1],
					-Q.vector4_f32[2],
					Q.vector4_f32[3]
				} } };
			return Result.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			static const VectorF32 NegativeOne3 = { { { -1.0f, -1.0f, -1.0f, 1.0f } } };
			return vmulq_f32(Q, NegativeOne3.v);
#elif defined(_XM_SSE_INTRINSICS_)
			static const VectorF32 NegativeOne3 = { { { -1.0f, -1.0f, -1.0f, 1.0f } } };
			return _mm_mul_ps(Q, NegativeOne3);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionInverse
		(
			FVector Q
		)
		{
			const Vector  Zero = VectorZero();

			Vector L = Vector4LengthSq(Q);
			Vector Conjugate = QuaternionConjugate(Q);

			Vector Control = VectorLessOrEqual(L, g_XMEpsilon.v);

			Vector Result = VectorDivide(Conjugate, L);

			Result = VectorSelect(Result, Zero, Control);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionLn
		(
			FVector Q
		)
		{
			static const VectorF32 OneMinusEpsilon = { { { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f } } };

			Vector QW = VectorSplatW(Q);
			Vector Q0 = VectorSelect(g_XMSelect1110.v, Q, g_XMSelect1110.v);

			Vector ControlW = VectorInBounds(QW, OneMinusEpsilon.v);

			Vector Theta = VectorACos(QW);
			Vector SinTheta = VectorSin(Theta);

			Vector S = VectorDivide(Theta, SinTheta);

			Vector Result = VectorMultiply(Q0, S);
			Result = VectorSelect(Q0, Result, ControlW);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionExp
		(
			FVector Q
		)
		{
			Vector Theta = Vector3Length(Q);

			Vector SinTheta, CosTheta;
			VectorSinCos(&SinTheta, &CosTheta, Theta);

			Vector S = VectorDivide(SinTheta, Theta);

			Vector Result = VectorMultiply(Q, S);

			const Vector Zero = VectorZero();
			Vector Control = VectorNearEqual(Theta, Zero, g_XMEpsilon.v);
			Result = VectorSelect(Result, Q, Control);

			Result = VectorSelect(CosTheta, Result, g_XMSelect1110.v);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionSlerp
		(
			FVector Q0,
			FVector Q1,
			float    t
		)
		{
			Vector T = VectorReplicate(t);
			return QuaternionSlerpV(Q0, Q1, T);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionSlerpV
		(
			FVector Q0,
			FVector Q1,
			FVector T
		)
		{
			assert((VectorGetY(T) == VectorGetX(T)) && (VectorGetZ(T) == VectorGetX(T)) && (VectorGetW(T) == VectorGetX(T)));

			// Result = Q0 * sin((1.0 - t) * Omega) / sin(Omega) + Q1 * sin(t * Omega) / sin(Omega)

#if defined(_XM_NO_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)

			const VectorF32 OneMinusEpsilon = { { { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f } } };

			Vector CosOmega = QuaternionDot(Q0, Q1);

			const Vector Zero = VectorZero();
			Vector Control = VectorLess(CosOmega, Zero);
			Vector Sign = VectorSelect(g_XMOne.v, g_XMNegativeOne.v, Control);

			CosOmega = VectorMultiply(CosOmega, Sign);

			Control = VectorLess(CosOmega, OneMinusEpsilon);

			Vector SinOmega = VectorNegativeMultiplySubtract(CosOmega, CosOmega, g_XMOne.v);
			SinOmega = VectorSqrt(SinOmega);

			Vector Omega = VectorATan2(SinOmega, CosOmega);

			Vector SignMask = VectorSplatSignMask();
			Vector V01 = VectorShiftLeft(T, Zero, 2);
			SignMask = VectorShiftLeft(SignMask, Zero, 3);
			V01 = VectorXorInt(V01, SignMask);
			V01 = VectorAdd(g_XMIdentityR0.v, V01);

			Vector InvSinOmega = VectorReciprocal(SinOmega);

			Vector S0 = VectorMultiply(V01, Omega);
			S0 = VectorSin(S0);
			S0 = VectorMultiply(S0, InvSinOmega);

			S0 = VectorSelect(V01, S0, Control);

			Vector S1 = VectorSplatY(S0);
			S0 = VectorSplatX(S0);

			S1 = VectorMultiply(S1, Sign);

			Vector Result = VectorMultiply(Q0, S0);
			Result = VectorMultiplyAdd(Q1, S1, Result);

			return Result;

#elif defined(_XM_SSE_INTRINSICS_)
			static const VectorF32 OneMinusEpsilon = { { { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f } } };
			static const VectorU32 SignMask2 = { { { 0x80000000, 0x00000000, 0x00000000, 0x00000000 } } };

			Vector CosOmega = QuaternionDot(Q0, Q1);

			const Vector Zero = VectorZero();
			Vector Control = VectorLess(CosOmega, Zero);
			Vector Sign = VectorSelect(g_XMOne, g_XMNegativeOne, Control);

			CosOmega = _mm_mul_ps(CosOmega, Sign);

			Control = VectorLess(CosOmega, OneMinusEpsilon);

			Vector SinOmega = _mm_mul_ps(CosOmega, CosOmega);
			SinOmega = _mm_sub_ps(g_XMOne, SinOmega);
			SinOmega = _mm_sqrt_ps(SinOmega);

			Vector Omega = VectorATan2(SinOmega, CosOmega);

			Vector V01 = XM_PERMUTE_PS(T, _MM_SHUFFLE(2, 3, 0, 1));
			V01 = _mm_and_ps(V01, g_XMMaskXY);
			V01 = _mm_xor_ps(V01, SignMask2);
			V01 = _mm_add_ps(g_XMIdentityR0, V01);

			Vector S0 = _mm_mul_ps(V01, Omega);
			S0 = VectorSin(S0);
			S0 = _mm_div_ps(S0, SinOmega);

			S0 = VectorSelect(V01, S0, Control);

			Vector S1 = VectorSplatY(S0);
			S0 = VectorSplatX(S0);

			S1 = _mm_mul_ps(S1, Sign);
			Vector Result = _mm_mul_ps(Q0, S0);
			S1 = _mm_mul_ps(S1, Q1);
			Result = _mm_add_ps(Result, S1);
			return Result;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionSquad
		(
			FVector Q0,
			FVector Q1,
			FVector Q2,
			GVector Q3,
			float    t
		)
		{
			Vector T = VectorReplicate(t);
			return QuaternionSquadV(Q0, Q1, Q2, Q3, T);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionSquadV
		(
			FVector Q0,
			FVector Q1,
			FVector Q2,
			GVector Q3,
			HVector T
		)
		{
			assert((VectorGetY(T) == VectorGetX(T)) && (VectorGetZ(T) == VectorGetX(T)) && (VectorGetW(T) == VectorGetX(T)));

			Vector TP = T;
			const Vector Two = VectorSplatConstant(2, 0);

			Vector Q03 = QuaternionSlerpV(Q0, Q3, T);
			Vector Q12 = QuaternionSlerpV(Q1, Q2, T);

			TP = VectorNegativeMultiplySubtract(TP, TP, TP);
			TP = VectorMultiply(TP, Two);

			Vector Result = QuaternionSlerpV(Q03, Q12, TP);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV QuaternionSquadSetup
		(
			Vector* pA,
			Vector* pB,
			Vector* pC,
			FVector  Q0,
			FVector  Q1,
			FVector  Q2,
			GVector  Q3
		)
		{
			assert(pA);
			assert(pB);
			assert(pC);

			Vector LS12 = QuaternionLengthSq(VectorAdd(Q1, Q2));
			Vector LD12 = QuaternionLengthSq(VectorSubtract(Q1, Q2));
			Vector SQ2 = VectorNegate(Q2);

			Vector Control1 = VectorLess(LS12, LD12);
			SQ2 = VectorSelect(Q2, SQ2, Control1);

			Vector LS01 = QuaternionLengthSq(VectorAdd(Q0, Q1));
			Vector LD01 = QuaternionLengthSq(VectorSubtract(Q0, Q1));
			Vector SQ0 = VectorNegate(Q0);

			Vector LS23 = QuaternionLengthSq(VectorAdd(SQ2, Q3));
			Vector LD23 = QuaternionLengthSq(VectorSubtract(SQ2, Q3));
			Vector SQ3 = VectorNegate(Q3);

			Vector Control0 = VectorLess(LS01, LD01);
			Vector Control2 = VectorLess(LS23, LD23);

			SQ0 = VectorSelect(Q0, SQ0, Control0);
			SQ3 = VectorSelect(Q3, SQ3, Control2);

			Vector InvQ1 = QuaternionInverse(Q1);
			Vector InvQ2 = QuaternionInverse(SQ2);

			Vector LnQ0 = QuaternionLn(QuaternionMultiply(InvQ1, SQ0));
			Vector LnQ2 = QuaternionLn(QuaternionMultiply(InvQ1, SQ2));
			Vector LnQ1 = QuaternionLn(QuaternionMultiply(InvQ2, Q1));
			Vector LnQ3 = QuaternionLn(QuaternionMultiply(InvQ2, SQ3));

			const Vector NegativeOneQuarter = VectorSplatConstant(-1, 2);

			Vector ExpQ02 = VectorMultiply(VectorAdd(LnQ0, LnQ2), NegativeOneQuarter);
			Vector ExpQ13 = VectorMultiply(VectorAdd(LnQ1, LnQ3), NegativeOneQuarter);
			ExpQ02 = QuaternionExp(ExpQ02);
			ExpQ13 = QuaternionExp(ExpQ13);

			*pA = QuaternionMultiply(Q1, ExpQ02);
			*pB = QuaternionMultiply(SQ2, ExpQ13);
			*pC = SQ2;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionBaryCentric
		(
			FVector Q0,
			FVector Q1,
			FVector Q2,
			float    f,
			float    g
		)
		{
			float s = f + g;

			Vector Result;
			if ((s < 0.00001f) && (s > -0.00001f))
			{
				Result = Q0;
			}
			else
			{
				Vector Q01 = QuaternionSlerp(Q0, Q1, s);
				Vector Q02 = QuaternionSlerp(Q0, Q2, s);

				Result = QuaternionSlerp(Q01, Q02, g / s);
			}

			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionBaryCentricV
		(
			FVector Q0,
			FVector Q1,
			FVector Q2,
			GVector F,
			HVector G
		)
		{
			assert((VectorGetY(F) == VectorGetX(F)) && (VectorGetZ(F) == VectorGetX(F)) && (VectorGetW(F) == VectorGetX(F)));
			assert((VectorGetY(G) == VectorGetX(G)) && (VectorGetZ(G) == VectorGetX(G)) && (VectorGetW(G) == VectorGetX(G)));

			const Vector Epsilon = VectorSplatConstant(1, 16);

			Vector S = VectorAdd(F, G);

			Vector Result;
			if (Vector4InBounds(S, Epsilon))
			{
				Result = Q0;
			}
			else
			{
				Vector Q01 = QuaternionSlerpV(Q0, Q1, S);
				Vector Q02 = QuaternionSlerpV(Q0, Q2, S);
				Vector GS = VectorReciprocal(S);
				GS = VectorMultiply(G, GS);

				Result = QuaternionSlerpV(Q01, Q02, GS);
			}

			return Result;
		}

		//------------------------------------------------------------------------------
		// Transformation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionIdentity()
		{
			return g_XMIdentityR3.v;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionRotationRollPitchYaw
		(
			float Pitch,
			float Yaw,
			float Roll
		)
		{
			Vector Angles = VectorSet(Pitch, Yaw, Roll, 0.0f);
			Vector Q = QuaternionRotationRollPitchYawFromVector(Angles);
			return Q;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionRotationRollPitchYawFromVector
		(
			FVector Angles // <Pitch, Yaw, Roll, 0>
		)
		{
			static const VectorF32  Sign = { { { 1.0f, -1.0f, -1.0f, 1.0f } } };

			Vector HalfAngles = VectorMultiply(Angles, g_XMOneHalf.v);

			Vector SinAngles, CosAngles;
			VectorSinCos(&SinAngles, &CosAngles, HalfAngles);

			Vector P0 = VectorPermute<XM_PERMUTE_0X, XM_PERMUTE_1X, XM_PERMUTE_1X, XM_PERMUTE_1X>(SinAngles, CosAngles);
			Vector Y0 = VectorPermute<XM_PERMUTE_1Y, XM_PERMUTE_0Y, XM_PERMUTE_1Y, XM_PERMUTE_1Y>(SinAngles, CosAngles);
			Vector R0 = VectorPermute<XM_PERMUTE_1Z, XM_PERMUTE_1Z, XM_PERMUTE_0Z, XM_PERMUTE_1Z>(SinAngles, CosAngles);
			Vector P1 = VectorPermute<XM_PERMUTE_0X, XM_PERMUTE_1X, XM_PERMUTE_1X, XM_PERMUTE_1X>(CosAngles, SinAngles);
			Vector Y1 = VectorPermute<XM_PERMUTE_1Y, XM_PERMUTE_0Y, XM_PERMUTE_1Y, XM_PERMUTE_1Y>(CosAngles, SinAngles);
			Vector R1 = VectorPermute<XM_PERMUTE_1Z, XM_PERMUTE_1Z, XM_PERMUTE_0Z, XM_PERMUTE_1Z>(CosAngles, SinAngles);

			Vector Q1 = VectorMultiply(P1, Sign.v);
			Vector Q0 = VectorMultiply(P0, Y0);
			Q1 = VectorMultiply(Q1, Y1);
			Q0 = VectorMultiply(Q0, R0);
			Vector Q = VectorMultiplyAdd(Q1, R1, Q0);

			return Q;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionRotationNormal
		(
			FVector NormalAxis,
			float    Angle
		)
		{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)

			Vector N = VectorSelect(g_XMOne.v, NormalAxis, g_XMSelect1110.v);

			float SinV, CosV;
			ScalarSinCos(&SinV, &CosV, 0.5f * Angle);

			Vector Scale = VectorSet(SinV, SinV, SinV, CosV);
			return VectorMultiply(N, Scale);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector N = _mm_and_ps(NormalAxis, g_XMMask3);
			N = _mm_or_ps(N, g_XMIdentityR3);
			Vector Scale = _mm_set_ps1(0.5f * Angle);
			Vector vSine;
			Vector vCosine;
			VectorSinCos(&vSine, &vCosine, Scale);
			Scale = _mm_and_ps(vSine, g_XMMask3);
			vCosine = _mm_and_ps(vCosine, g_XMMaskW);
			Scale = _mm_or_ps(Scale, vCosine);
			N = _mm_mul_ps(N, Scale);
			return N;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionRotationAxis
		(
			FVector Axis,
			float    Angle
		)
		{
			assert(!Vector3Equal(Axis, VectorZero()));
			assert(!Vector3IsInfinite(Axis));

			Vector Normal = Vector3Normalize(Axis);
			Vector Q = QuaternionRotationNormal(Normal, Angle);
			return Q;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV QuaternionRotationMatrix
		(
			FMatrix M
		)
		{
#if defined(_XM_NO_INTRINSICS_)

			VectorF32 q;
			float r22 = M.m[2][2];
			if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
			{
				float dif10 = M.m[1][1] - M.m[0][0];
				float omr22 = 1.f - r22;
				if (dif10 <= 0.f)  // x^2 >= y^2
				{
					float fourXSqr = omr22 - dif10;
					float inv4x = 0.5f / sqrtf(fourXSqr);
					q.f[0] = fourXSqr * inv4x;
					q.f[1] = (M.m[0][1] + M.m[1][0]) * inv4x;
					q.f[2] = (M.m[0][2] + M.m[2][0]) * inv4x;
					q.f[3] = (M.m[1][2] - M.m[2][1]) * inv4x;
				}
				else  // y^2 >= x^2
				{
					float fourYSqr = omr22 + dif10;
					float inv4y = 0.5f / sqrtf(fourYSqr);
					q.f[0] = (M.m[0][1] + M.m[1][0]) * inv4y;
					q.f[1] = fourYSqr * inv4y;
					q.f[2] = (M.m[1][2] + M.m[2][1]) * inv4y;
					q.f[3] = (M.m[2][0] - M.m[0][2]) * inv4y;
				}
			}
			else  // z^2 + w^2 >= x^2 + y^2
			{
				float sum10 = M.m[1][1] + M.m[0][0];
				float opr22 = 1.f + r22;
				if (sum10 <= 0.f)  // z^2 >= w^2
				{
					float fourZSqr = opr22 - sum10;
					float inv4z = 0.5f / sqrtf(fourZSqr);
					q.f[0] = (M.m[0][2] + M.m[2][0]) * inv4z;
					q.f[1] = (M.m[1][2] + M.m[2][1]) * inv4z;
					q.f[2] = fourZSqr * inv4z;
					q.f[3] = (M.m[0][1] - M.m[1][0]) * inv4z;
				}
				else  // w^2 >= z^2
				{
					float fourWSqr = opr22 + sum10;
					float inv4w = 0.5f / sqrtf(fourWSqr);
					q.f[0] = (M.m[1][2] - M.m[2][1]) * inv4w;
					q.f[1] = (M.m[2][0] - M.m[0][2]) * inv4w;
					q.f[2] = (M.m[0][1] - M.m[1][0]) * inv4w;
					q.f[3] = fourWSqr * inv4w;
				}
			}
			return q.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
			static const VectorF32 XMPMMP = { { { +1.0f, -1.0f, -1.0f, +1.0f } } };
			static const VectorF32 XMMPMP = { { { -1.0f, +1.0f, -1.0f, +1.0f } } };
			static const VectorF32 XMMMPP = { { { -1.0f, -1.0f, +1.0f, +1.0f } } };
			static const VectorU32 Select0110 = { { { XM_SELECT_0, XM_SELECT_1, XM_SELECT_1, XM_SELECT_0 } } };
			static const VectorU32 Select0010 = { { { XM_SELECT_0, XM_SELECT_0, XM_SELECT_1, XM_SELECT_0 } } };

			Vector r0 = M.r[0];
			Vector r1 = M.r[1];
			Vector r2 = M.r[2];

			Vector r00 = vdupq_lane_f32(vget_low_f32(r0), 0);
			Vector r11 = vdupq_lane_f32(vget_low_f32(r1), 1);
			Vector r22 = vdupq_lane_f32(vget_high_f32(r2), 0);

			// x^2 >= y^2 equivalent to r11 - r00 <= 0
			Vector r11mr00 = vsubq_f32(r11, r00);
			Vector x2gey2 = vcleq_f32(r11mr00, g_XMZero);

			// z^2 >= w^2 equivalent to r11 + r00 <= 0
			Vector r11pr00 = vaddq_f32(r11, r00);
			Vector z2gew2 = vcleq_f32(r11pr00, g_XMZero);

			// x^2 + y^2 >= z^2 + w^2 equivalent to r22 <= 0
			Vector x2py2gez2pw2 = vcleq_f32(r22, g_XMZero);

			// (4*x^2, 4*y^2, 4*z^2, 4*w^2)
			Vector t0 = vmulq_f32(XMPMMP, r00);
			Vector x2y2z2w2 = vmlaq_f32(t0, XMMPMP, r11);
			x2y2z2w2 = vmlaq_f32(x2y2z2w2, XMMMPP, r22);
			x2y2z2w2 = vaddq_f32(x2y2z2w2, g_XMOne);

			// (r01, r02, r12, r11)
			t0 = vextq_f32(r0, r0, 1);
			Vector t1 = vextq_f32(r1, r1, 1);
			t0 = vcombine_f32(vget_low_f32(t0), vrev64_f32(vget_low_f32(t1)));

			// (r10, r20, r21, r10)
			t1 = vextq_f32(r2, r2, 3);
			Vector r10 = vdupq_lane_f32(vget_low_f32(r1), 0);
			t1 = vbslq_f32(Select0110, t1, r10);

			// (4*x*y, 4*x*z, 4*y*z, unused)
			Vector xyxzyz = vaddq_f32(t0, t1);

			// (r21, r20, r10, r10)
			t0 = vcombine_f32(vrev64_f32(vget_low_f32(r2)), vget_low_f32(r10));

			// (r12, r02, r01, r12)
			Vector t2 = vcombine_f32(vrev64_f32(vget_high_f32(r0)), vrev64_f32(vget_low_f32(r0)));
			Vector t3 = vdupq_lane_f32(vget_high_f32(r1), 0);
			t1 = vbslq_f32(Select0110, t2, t3);

			// (4*x*w, 4*y*w, 4*z*w, unused)
			Vector xwywzw = vsubq_f32(t0, t1);
			xwywzw = vmulq_f32(XMMPMP, xwywzw);

			// (4*x*x, 4*x*y, 4*x*z, 4*x*w)
			t0 = vextq_f32(xyxzyz, xyxzyz, 3);
			t1 = vbslq_f32(Select0110, t0, x2y2z2w2);
			t2 = vdupq_lane_f32(vget_low_f32(xwywzw), 0);
			Vector tensor0 = vbslq_f32(g_XMSelect1110, t1, t2);

			// (4*y*x, 4*y*y, 4*y*z, 4*y*w)
			t0 = vbslq_f32(g_XMSelect1011, xyxzyz, x2y2z2w2);
			t1 = vdupq_lane_f32(vget_low_f32(xwywzw), 1);
			Vector tensor1 = vbslq_f32(g_XMSelect1110, t0, t1);

			// (4*z*x, 4*z*y, 4*z*z, 4*z*w)
			t0 = vextq_f32(xyxzyz, xyxzyz, 1);
			t1 = vcombine_f32(vget_low_f32(t0), vrev64_f32(vget_high_f32(xwywzw)));
			Vector tensor2 = vbslq_f32(Select0010, x2y2z2w2, t1);

			// (4*w*x, 4*w*y, 4*w*z, 4*w*w)
			Vector tensor3 = vbslq_f32(g_XMSelect1110, xwywzw, x2y2z2w2);

			// Select the row of the tensor-product matrix that has the largest
			// magnitude.
			t0 = vbslq_f32(x2gey2, tensor0, tensor1);
			t1 = vbslq_f32(z2gew2, tensor2, tensor3);
			t2 = vbslq_f32(x2py2gez2pw2, t0, t1);

			// Normalize the row.  No division by zero is possible because the
			// quaternion is unit-length (and the row is a nonzero multiple of
			// the quaternion).
			t0 = Vector4Length(t2);
			return VectorDivide(t2, t0);
#elif defined(_XM_SSE_INTRINSICS_)
			static const VectorF32 XMPMMP = { { { +1.0f, -1.0f, -1.0f, +1.0f } } };
			static const VectorF32 XMMPMP = { { { -1.0f, +1.0f, -1.0f, +1.0f } } };
			static const VectorF32 XMMMPP = { { { -1.0f, -1.0f, +1.0f, +1.0f } } };

			Vector r0 = M.r[0];  // (r00, r01, r02, 0)
			Vector r1 = M.r[1];  // (r10, r11, r12, 0)
			Vector r2 = M.r[2];  // (r20, r21, r22, 0)

			// (r00, r00, r00, r00)
			Vector r00 = XM_PERMUTE_PS(r0, _MM_SHUFFLE(0, 0, 0, 0));
			// (r11, r11, r11, r11)
			Vector r11 = XM_PERMUTE_PS(r1, _MM_SHUFFLE(1, 1, 1, 1));
			// (r22, r22, r22, r22)
			Vector r22 = XM_PERMUTE_PS(r2, _MM_SHUFFLE(2, 2, 2, 2));

			// x^2 >= y^2 equivalent to r11 - r00 <= 0
			// (r11 - r00, r11 - r00, r11 - r00, r11 - r00)
			Vector r11mr00 = _mm_sub_ps(r11, r00);
			Vector x2gey2 = _mm_cmple_ps(r11mr00, g_XMZero);

			// z^2 >= w^2 equivalent to r11 + r00 <= 0
			// (r11 + r00, r11 + r00, r11 + r00, r11 + r00)
			Vector r11pr00 = _mm_add_ps(r11, r00);
			Vector z2gew2 = _mm_cmple_ps(r11pr00, g_XMZero);

			// x^2 + y^2 >= z^2 + w^2 equivalent to r22 <= 0
			Vector x2py2gez2pw2 = _mm_cmple_ps(r22, g_XMZero);

			// (+r00, -r00, -r00, +r00)
			Vector t0 = _mm_mul_ps(XMPMMP, r00);

			// (-r11, +r11, -r11, +r11)
			Vector t1 = _mm_mul_ps(XMMPMP, r11);

			// (-r22, -r22, +r22, +r22)
			Vector t2 = _mm_mul_ps(XMMMPP, r22);

			// (4*x^2, 4*y^2, 4*z^2, 4*w^2)
			Vector x2y2z2w2 = _mm_add_ps(t0, t1);
			x2y2z2w2 = _mm_add_ps(t2, x2y2z2w2);
			x2y2z2w2 = _mm_add_ps(x2y2z2w2, g_XMOne);

			// (r01, r02, r12, r11)
			t0 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 2, 2, 1));
			// (r10, r10, r20, r21)
			t1 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(1, 0, 0, 0));
			// (r10, r20, r21, r10)
			t1 = XM_PERMUTE_PS(t1, _MM_SHUFFLE(1, 3, 2, 0));
			// (4*x*y, 4*x*z, 4*y*z, unused)
			Vector xyxzyz = _mm_add_ps(t0, t1);

			// (r21, r20, r10, r10)
			t0 = _mm_shuffle_ps(r2, r1, _MM_SHUFFLE(0, 0, 0, 1));
			// (r12, r12, r02, r01)
			t1 = _mm_shuffle_ps(r1, r0, _MM_SHUFFLE(1, 2, 2, 2));
			// (r12, r02, r01, r12)
			t1 = XM_PERMUTE_PS(t1, _MM_SHUFFLE(1, 3, 2, 0));
			// (4*x*w, 4*y*w, 4*z*w, unused)
			Vector xwywzw = _mm_sub_ps(t0, t1);
			xwywzw = _mm_mul_ps(XMMPMP, xwywzw);

			// (4*x^2, 4*y^2, 4*x*y, unused)
			t0 = _mm_shuffle_ps(x2y2z2w2, xyxzyz, _MM_SHUFFLE(0, 0, 1, 0));
			// (4*z^2, 4*w^2, 4*z*w, unused)
			t1 = _mm_shuffle_ps(x2y2z2w2, xwywzw, _MM_SHUFFLE(0, 2, 3, 2));
			// (4*x*z, 4*y*z, 4*x*w, 4*y*w)
			t2 = _mm_shuffle_ps(xyxzyz, xwywzw, _MM_SHUFFLE(1, 0, 2, 1));

			// (4*x*x, 4*x*y, 4*x*z, 4*x*w)
			Vector tensor0 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(2, 0, 2, 0));
			// (4*y*x, 4*y*y, 4*y*z, 4*y*w)
			Vector tensor1 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(3, 1, 1, 2));
			// (4*z*x, 4*z*y, 4*z*z, 4*z*w)
			Vector tensor2 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(2, 0, 1, 0));
			// (4*w*x, 4*w*y, 4*w*z, 4*w*w)
			Vector tensor3 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(1, 2, 3, 2));

			// Select the row of the tensor-product matrix that has the largest
			// magnitude.
			t0 = _mm_and_ps(x2gey2, tensor0);
			t1 = _mm_andnot_ps(x2gey2, tensor1);
			t0 = _mm_or_ps(t0, t1);
			t1 = _mm_and_ps(z2gew2, tensor2);
			t2 = _mm_andnot_ps(z2gew2, tensor3);
			t1 = _mm_or_ps(t1, t2);
			t0 = _mm_and_ps(x2py2gez2pw2, t0);
			t1 = _mm_andnot_ps(x2py2gez2pw2, t1);
			t2 = _mm_or_ps(t0, t1);

			// Normalize the row.  No division by zero is possible because the
			// quaternion is unit-length (and the row is a nonzero multiple of
			// the quaternion).
			t0 = Vector4Length(t2);
			return _mm_div_ps(t2, t0);
#endif
		}

		//------------------------------------------------------------------------------
		// Conversion operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV QuaternionToAxisAngle
		(
			Vector* pAxis,
			float* pAngle,
			FVector  Q
		)
		{
			assert(pAxis);
			assert(pAngle);

			*pAxis = Q;

			*pAngle = 2.0f * ScalarACos(VectorGetW(Q));
		}

		/****************************************************************************
		 *
		 * Plane
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------
		 // Comparison operations
		 //------------------------------------------------------------------------------

		 //------------------------------------------------------------------------------

		inline bool XM_CALLCONV PlaneEqual
		(
			FVector P1,
			FVector P2
		)
		{
			return Vector4Equal(P1, P2);
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV PlaneNearEqual
		(
			FVector P1,
			FVector P2,
			FVector Epsilon
		)
		{
			Vector NP1 = PlaneNormalize(P1);
			Vector NP2 = PlaneNormalize(P2);
			return Vector4NearEqual(NP1, NP2, Epsilon);
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV PlaneNotEqual
		(
			FVector P1,
			FVector P2
		)
		{
			return Vector4NotEqual(P1, P2);
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV PlaneIsNaN
		(
			FVector P
		)
		{
			return Vector4IsNaN(P);
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV PlaneIsInfinite
		(
			FVector P
		)
		{
			return Vector4IsInfinite(P);
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV PlaneDot
		(
			FVector P,
			FVector V
		)
		{
			return Vector4Dot(P, V);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV PlaneDotCoord
		(
			FVector P,
			FVector V
		)
		{
			// Result = P[0] * V[0] + P[1] * V[1] + P[2] * V[2] + P[3]

			Vector V3 = VectorSelect(g_XMOne.v, V, g_XMSelect1110.v);
			Vector Result = Vector4Dot(P, V3);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV PlaneDotNormal
		(
			FVector P,
			FVector V
		)
		{
			return Vector3Dot(P, V);
		}

		//------------------------------------------------------------------------------
		// PlaneNormalizeEst uses a reciprocal estimate and
		// returns QNaN on zero and infinite vectors.

		inline Vector XM_CALLCONV PlaneNormalizeEst
		(
			FVector P
		)
		{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)

			Vector Result = Vector3ReciprocalLengthEst(P);
			return VectorMultiply(P, Result);

#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vTemp = _mm_dp_ps(P, P, 0x7f);
			Vector vResult = _mm_rsqrt_ps(vTemp);
			return _mm_mul_ps(vResult, P);
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product
			Vector vDot = _mm_mul_ps(P, P);
			// x=Dot.y, y=Dot.z
			Vector vTemp = XM_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
			// Result.x = x+y
			vDot = _mm_add_ss(vDot, vTemp);
			// x=Dot.z
			vTemp = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			// Result.x = (x+y)+z
			vDot = _mm_add_ss(vDot, vTemp);
			// Splat x
			vDot = XM_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
			// Get the reciprocal
			vDot = _mm_rsqrt_ps(vDot);
			// Get the reciprocal
			vDot = _mm_mul_ps(vDot, P);
			return vDot;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV PlaneNormalize
		(
			FVector P
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			float fLengthSq = sqrtf((P.vector4_f32[0] * P.vector4_f32[0]) + (P.vector4_f32[1] * P.vector4_f32[1]) + (P.vector4_f32[2] * P.vector4_f32[2]));
			// Prevent divide by zero
			if (fLengthSq > 0)
			{
				fLengthSq = 1.0f / fLengthSq;
			}
			VectorF32 vResult = { { {
					P.vector4_f32[0] * fLengthSq,
					P.vector4_f32[1] * fLengthSq,
					P.vector4_f32[2] * fLengthSq,
					P.vector4_f32[3] * fLengthSq
				} } };
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Vector vLength = Vector3ReciprocalLength(P);
			return VectorMultiply(P, vLength);
#elif defined(_XM_SSE4_INTRINSICS_)
			Vector vLengthSq = _mm_dp_ps(P, P, 0x7f);
			// Prepare for the division
			Vector vResult = _mm_sqrt_ps(vLengthSq);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_XMInfinity);
			// Reciprocal mul to perform the normalization
			vResult = _mm_div_ps(P, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vLengthSq);
			return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
			// Perform the dot product on x,y and z only
			Vector vLengthSq = _mm_mul_ps(P, P);
			Vector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 1, 2, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vTemp = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			// Prepare for the division
			Vector vResult = _mm_sqrt_ps(vLengthSq);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_XMInfinity);
			// Reciprocal mul to perform the normalization
			vResult = _mm_div_ps(P, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vLengthSq);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV PlaneIntersectLine
		(
			FVector P,
			FVector LinePoint1,
			FVector LinePoint2
		)
		{
			Vector V1 = Vector3Dot(P, LinePoint1);
			Vector V2 = Vector3Dot(P, LinePoint2);
			Vector D = VectorSubtract(V1, V2);

			Vector VT = PlaneDotCoord(P, LinePoint1);
			VT = VectorDivide(VT, D);

			Vector Point = VectorSubtract(LinePoint2, LinePoint1);
			Point = VectorMultiplyAdd(Point, VT, LinePoint1);

			const Vector Zero = VectorZero();
			Vector Control = VectorNearEqual(D, Zero, g_XMEpsilon.v);

			return VectorSelect(Point, g_XMQNaN.v, Control);
		}

		//------------------------------------------------------------------------------

		inline void XM_CALLCONV PlaneIntersectPlane
		(
			Vector* pLinePoint1,
			Vector* pLinePoint2,
			FVector  P1,
			FVector  P2
		)
		{
			assert(pLinePoint1);
			assert(pLinePoint2);

			Vector V1 = Vector3Cross(P2, P1);

			Vector LengthSq = Vector3LengthSq(V1);

			Vector V2 = Vector3Cross(P2, V1);

			Vector P1W = VectorSplatW(P1);
			Vector Point = VectorMultiply(V2, P1W);

			Vector V3 = Vector3Cross(V1, P1);

			Vector P2W = VectorSplatW(P2);
			Point = VectorMultiplyAdd(V3, P2W, Point);

			Vector LinePoint1 = VectorDivide(Point, LengthSq);

			Vector LinePoint2 = VectorAdd(LinePoint1, V1);

			Vector Control = VectorLessOrEqual(LengthSq, g_XMEpsilon.v);
			*pLinePoint1 = VectorSelect(LinePoint1, g_XMQNaN.v, Control);
			*pLinePoint2 = VectorSelect(LinePoint2, g_XMQNaN.v, Control);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV PlaneTransform
		(
			FVector P,
			FMatrix M
		)
		{
			Vector W = VectorSplatW(P);
			Vector Z = VectorSplatZ(P);
			Vector Y = VectorSplatY(P);
			Vector X = VectorSplatX(P);

			Vector Result = VectorMultiply(W, M.r[3]);
			Result = VectorMultiplyAdd(Z, M.r[2], Result);
			Result = VectorMultiplyAdd(Y, M.r[1], Result);
			Result = VectorMultiplyAdd(X, M.r[0], Result);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline Float4* XM_CALLCONV PlaneTransformStream
		(
			Float4* pOutputStream,
			size_t          OutputStride,
			const Float4* pInputStream,
			size_t          InputStride,
			size_t          PlaneCount,
			FMatrix       M
		)
		{
			return Vector4TransformStream(pOutputStream,
				OutputStride,
				pInputStream,
				InputStride,
				PlaneCount,
				M);
		}

		//------------------------------------------------------------------------------
		// Conversion operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV PlaneFromPointNormal
		(
			FVector Point,
			FVector Normal
		)
		{
			Vector W = Vector3Dot(Point, Normal);
			W = VectorNegate(W);
			return VectorSelect(W, Normal, g_XMSelect1110.v);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV PlaneFromPoints
		(
			FVector Point1,
			FVector Point2,
			FVector Point3
		)
		{
			Vector V21 = VectorSubtract(Point1, Point2);
			Vector V31 = VectorSubtract(Point1, Point3);

			Vector N = Vector3Cross(V21, V31);
			N = Vector3Normalize(N);

			Vector D = PlaneDotNormal(N, Point1);
			D = VectorNegate(D);

			Vector Result = VectorSelect(D, N, g_XMSelect1110.v);

			return Result;
		}

		/****************************************************************************
		 *
		 * Color
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------
		 // Comparison operations
		 //------------------------------------------------------------------------------

		 //------------------------------------------------------------------------------

		inline bool XM_CALLCONV ColorEqual
		(
			FVector C1,
			FVector C2
		)
		{
			return Vector4Equal(C1, C2);
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV ColorNotEqual
		(
			FVector C1,
			FVector C2
		)
		{
			return Vector4NotEqual(C1, C2);
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV ColorGreater
		(
			FVector C1,
			FVector C2
		)
		{
			return Vector4Greater(C1, C2);
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV ColorGreaterOrEqual
		(
			FVector C1,
			FVector C2
		)
		{
			return Vector4GreaterOrEqual(C1, C2);
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV ColorLess
		(
			FVector C1,
			FVector C2
		)
		{
			return Vector4Less(C1, C2);
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV ColorLessOrEqual
		(
			FVector C1,
			FVector C2
		)
		{
			return Vector4LessOrEqual(C1, C2);
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV ColorIsNaN
		(
			FVector C
		)
		{
			return Vector4IsNaN(C);
		}

		//------------------------------------------------------------------------------

		inline bool XM_CALLCONV ColorIsInfinite
		(
			FVector C
		)
		{
			return Vector4IsInfinite(C);
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorNegative
		(
			FVector vColor
		)
		{
#if defined(_XM_NO_INTRINSICS_)
			VectorF32 vResult = { { {
					1.0f - vColor.vector4_f32[0],
					1.0f - vColor.vector4_f32[1],
					1.0f - vColor.vector4_f32[2],
					vColor.vector4_f32[3]
				} } };
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Vector vTemp = veorq_u32(vColor, g_XMNegate3);
			return vaddq_f32(vTemp, g_XMOne3);
#elif defined(_XM_SSE_INTRINSICS_)
			// Negate only x,y and z.
			Vector vTemp = _mm_xor_ps(vColor, g_XMNegate3);
			// Add 1,1,1,0 to -x,-y,-z,w
			return _mm_add_ps(vTemp, g_XMOne3);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorModulate
		(
			FVector C1,
			FVector C2
		)
		{
			return VectorMultiply(C1, C2);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorAdjustSaturation
		(
			FVector vColor,
			float    fSaturation
		)
		{
			// Luminance = 0.2125f * C[0] + 0.7154f * C[1] + 0.0721f * C[2];
			// Result = (C - Luminance) * Saturation + Luminance;

			const VectorF32 gvLuminance = { { { 0.2125f, 0.7154f, 0.0721f, 0.0f } } };
#if defined(_XM_NO_INTRINSICS_)
			float fLuminance = (vColor.vector4_f32[0] * gvLuminance.f[0]) + (vColor.vector4_f32[1] * gvLuminance.f[1]) + (vColor.vector4_f32[2] * gvLuminance.f[2]);
			Vector vResult;
			vResult.vector4_f32[0] = ((vColor.vector4_f32[0] - fLuminance) * fSaturation) + fLuminance;
			vResult.vector4_f32[1] = ((vColor.vector4_f32[1] - fLuminance) * fSaturation) + fLuminance;
			vResult.vector4_f32[2] = ((vColor.vector4_f32[2] - fLuminance) * fSaturation) + fLuminance;
			vResult.vector4_f32[3] = vColor.vector4_f32[3];
			return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Vector vLuminance = Vector3Dot(vColor, gvLuminance);
			Vector vResult = vsubq_f32(vColor, vLuminance);
			vResult = vmlaq_n_f32(vLuminance, vResult, fSaturation);
			return vbslq_f32(g_XMSelect1110, vResult, vColor);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vLuminance = Vector3Dot(vColor, gvLuminance);
			// Splat fSaturation
			Vector vSaturation = _mm_set_ps1(fSaturation);
			// vResult = ((vColor-vLuminance)*vSaturation)+vLuminance;
			Vector vResult = _mm_sub_ps(vColor, vLuminance);
			vResult = _mm_mul_ps(vResult, vSaturation);
			vResult = _mm_add_ps(vResult, vLuminance);
			// Retain w from the source color
			vLuminance = _mm_shuffle_ps(vResult, vColor, _MM_SHUFFLE(3, 2, 2, 2));   // x = vResult.z,y = vResult.z,z = vColor.z,w=vColor.w
			vResult = _mm_shuffle_ps(vResult, vLuminance, _MM_SHUFFLE(3, 0, 1, 0));  // x = vResult.x,y = vResult.y,z = vResult.z,w=vColor.w
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorAdjustContrast
		(
			FVector vColor,
			float    fContrast
		)
		{
			// Result = (vColor - 0.5f) * fContrast + 0.5f;

#if defined(_XM_NO_INTRINSICS_)
			VectorF32 vResult = { { {
					((vColor.vector4_f32[0] - 0.5f) * fContrast) + 0.5f,
					((vColor.vector4_f32[1] - 0.5f) * fContrast) + 0.5f,
					((vColor.vector4_f32[2] - 0.5f) * fContrast) + 0.5f,
					vColor.vector4_f32[3]        // Leave W untouched
				} } };
			return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
			Vector vResult = vsubq_f32(vColor, g_XMOneHalf.v);
			vResult = vmlaq_n_f32(g_XMOneHalf.v, vResult, fContrast);
			return vbslq_f32(g_XMSelect1110, vResult, vColor);
#elif defined(_XM_SSE_INTRINSICS_)
			Vector vScale = _mm_set_ps1(fContrast);           // Splat the scale
			Vector vResult = _mm_sub_ps(vColor, g_XMOneHalf);  // Subtract 0.5f from the source (Saving source)
			vResult = _mm_mul_ps(vResult, vScale);               // Mul by scale
			vResult = _mm_add_ps(vResult, g_XMOneHalf);          // Add 0.5f
		// Retain w from the source color
			vScale = _mm_shuffle_ps(vResult, vColor, _MM_SHUFFLE(3, 2, 2, 2));   // x = vResult.z,y = vResult.z,z = vColor.z,w=vColor.w
			vResult = _mm_shuffle_ps(vResult, vScale, _MM_SHUFFLE(3, 0, 1, 0));  // x = vResult.x,y = vResult.y,z = vResult.z,w=vColor.w
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorRGBToHSL(FVector rgb)
		{
			Vector r = VectorSplatX(rgb);
			Vector g = VectorSplatY(rgb);
			Vector b = VectorSplatZ(rgb);

			Vector min = VectorMin(r, VectorMin(g, b));
			Vector max = VectorMax(r, VectorMax(g, b));

			Vector l = VectorMultiply(VectorAdd(min, max), g_XMOneHalf);

			Vector d = VectorSubtract(max, min);

			Vector la = VectorSelect(rgb, l, g_XMSelect1110);

			if (Vector3Less(d, g_XMEpsilon))
			{
				// Achromatic, assume H and S of 0
				return VectorSelect(la, g_XMZero, g_XMSelect1100);
			}
			else
			{
				Vector s, h;

				Vector d2 = VectorAdd(min, max);

				if (Vector3Greater(l, g_XMOneHalf))
				{
					// d / (2-max-min)
					s = VectorDivide(d, VectorSubtract(g_XMTwo, d2));
				}
				else
				{
					// d / (max+min)
					s = VectorDivide(d, d2);
				}

				if (Vector3Equal(r, max))
				{
					// Red is max
					h = VectorDivide(VectorSubtract(g, b), d);
				}
				else if (Vector3Equal(g, max))
				{
					// Green is max
					h = VectorDivide(VectorSubtract(b, r), d);
					h = VectorAdd(h, g_XMTwo);
				}
				else
				{
					// Blue is max
					h = VectorDivide(VectorSubtract(r, g), d);
					h = VectorAdd(h, g_XMFour);
				}

				h = VectorDivide(h, g_XMSix);

				if (Vector3Less(h, g_XMZero))
					h = VectorAdd(h, g_XMOne);

				Vector lha = VectorSelect(la, h, g_XMSelect1100);
				return VectorSelect(s, lha, g_XMSelect1011);
			}
		}

		//------------------------------------------------------------------------------

		namespace Internal
		{

			inline Vector XM_CALLCONV ColorHue2Clr(FVector p, FVector q, FVector h)
			{
				static const VectorF32 oneSixth = { { { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f } } };
				static const VectorF32 twoThirds = { { { 2.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f } } };

				Vector t = h;

				if (Vector3Less(t, g_XMZero))
					t = VectorAdd(t, g_XMOne);

				if (Vector3Greater(t, g_XMOne))
					t = VectorSubtract(t, g_XMOne);

				if (Vector3Less(t, oneSixth))
				{
					// p + (q - p) * 6 * t
					Vector t1 = VectorSubtract(q, p);
					Vector t2 = VectorMultiply(g_XMSix, t);
					return VectorMultiplyAdd(t1, t2, p);
				}

				if (Vector3Less(t, g_XMOneHalf))
					return q;

				if (Vector3Less(t, twoThirds))
				{
					// p + (q - p) * 6 * (2/3 - t)
					Vector t1 = VectorSubtract(q, p);
					Vector t2 = VectorMultiply(g_XMSix, VectorSubtract(twoThirds, t));
					return VectorMultiplyAdd(t1, t2, p);
				}

				return p;
			}

		} // namespace Internal

		inline Vector XM_CALLCONV ColorHSLToRGB(FVector hsl)
		{
			static const VectorF32 oneThird = { { { 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f } } };

			Vector s = VectorSplatY(hsl);
			Vector l = VectorSplatZ(hsl);

			if (Vector3NearEqual(s, g_XMZero, g_XMEpsilon))
			{
				// Achromatic
				return VectorSelect(hsl, l, g_XMSelect1110);
			}
			else
			{
				Vector h = VectorSplatX(hsl);

				Vector q;
				if (Vector3Less(l, g_XMOneHalf))
				{
					q = VectorMultiply(l, VectorAdd(g_XMOne, s));
				}
				else
				{
					q = VectorSubtract(VectorAdd(l, s), VectorMultiply(l, s));
				}

				Vector p = VectorSubtract(VectorMultiply(g_XMTwo, l), q);

				Vector r = Internal::ColorHue2Clr(p, q, VectorAdd(h, oneThird));
				Vector g = Internal::ColorHue2Clr(p, q, h);
				Vector b = Internal::ColorHue2Clr(p, q, VectorSubtract(h, oneThird));

				Vector rg = VectorSelect(g, r, g_XMSelect1000);
				Vector ba = VectorSelect(hsl, b, g_XMSelect1110);

				return VectorSelect(ba, rg, g_XMSelect1100);
			}
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorRGBToHSV(FVector rgb)
		{
			Vector r = VectorSplatX(rgb);
			Vector g = VectorSplatY(rgb);
			Vector b = VectorSplatZ(rgb);

			Vector min = VectorMin(r, VectorMin(g, b));
			Vector v = VectorMax(r, VectorMax(g, b));

			Vector d = VectorSubtract(v, min);

			Vector s = (Vector3NearEqual(v, g_XMZero, g_XMEpsilon)) ? g_XMZero : VectorDivide(d, v);

			if (Vector3Less(d, g_XMEpsilon))
			{
				// Achromatic, assume H of 0
				Vector hv = VectorSelect(v, g_XMZero, g_XMSelect1000);
				Vector hva = VectorSelect(rgb, hv, g_XMSelect1110);
				return VectorSelect(s, hva, g_XMSelect1011);
			}
			else
			{
				Vector h;

				if (Vector3Equal(r, v))
				{
					// Red is max
					h = VectorDivide(VectorSubtract(g, b), d);

					if (Vector3Less(g, b))
						h = VectorAdd(h, g_XMSix);
				}
				else if (Vector3Equal(g, v))
				{
					// Green is max
					h = VectorDivide(VectorSubtract(b, r), d);
					h = VectorAdd(h, g_XMTwo);
				}
				else
				{
					// Blue is max
					h = VectorDivide(VectorSubtract(r, g), d);
					h = VectorAdd(h, g_XMFour);
				}

				h = VectorDivide(h, g_XMSix);

				Vector hv = VectorSelect(v, h, g_XMSelect1000);
				Vector hva = VectorSelect(rgb, hv, g_XMSelect1110);
				return VectorSelect(s, hva, g_XMSelect1011);
			}
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorHSVToRGB(FVector hsv)
		{
			Vector h = VectorSplatX(hsv);
			Vector s = VectorSplatY(hsv);
			Vector v = VectorSplatZ(hsv);

			Vector h6 = VectorMultiply(h, g_XMSix);

			Vector i = VectorFloor(h6);
			Vector f = VectorSubtract(h6, i);

			// p = v* (1-s)
			Vector p = VectorMultiply(v, VectorSubtract(g_XMOne, s));

			// q = v*(1-f*s)
			Vector q = VectorMultiply(v, VectorSubtract(g_XMOne, VectorMultiply(f, s)));

			// t = v*(1 - (1-f)*s)
			Vector t = VectorMultiply(v, VectorSubtract(g_XMOne, VectorMultiply(VectorSubtract(g_XMOne, f), s)));

			auto ii = static_cast<int>(VectorGetX(VectorMod(i, g_XMSix)));

			Vector _rgb;

			switch (ii)
			{
			case 0: // rgb = vtp
			{
				Vector vt = VectorSelect(t, v, g_XMSelect1000);
				_rgb = VectorSelect(p, vt, g_XMSelect1100);
			}
			break;
			case 1: // rgb = qvp
			{
				Vector qv = VectorSelect(v, q, g_XMSelect1000);
				_rgb = VectorSelect(p, qv, g_XMSelect1100);
			}
			break;
			case 2: // rgb = pvt
			{
				Vector pv = VectorSelect(v, p, g_XMSelect1000);
				_rgb = VectorSelect(t, pv, g_XMSelect1100);
			}
			break;
			case 3: // rgb = pqv
			{
				Vector pq = VectorSelect(q, p, g_XMSelect1000);
				_rgb = VectorSelect(v, pq, g_XMSelect1100);
			}
			break;
			case 4: // rgb = tpv
			{
				Vector tp = VectorSelect(p, t, g_XMSelect1000);
				_rgb = VectorSelect(v, tp, g_XMSelect1100);
			}
			break;
			default: // rgb = vpq
			{
				Vector vp = VectorSelect(p, v, g_XMSelect1000);
				_rgb = VectorSelect(q, vp, g_XMSelect1100);
			}
			break;
			}

			return VectorSelect(hsv, _rgb, g_XMSelect1110);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorRGBToYUV(FVector rgb)
		{
			static const VectorF32 Scale0 = { { { 0.299f, -0.147f, 0.615f, 0.0f } } };
			static const VectorF32 Scale1 = { { { 0.587f, -0.289f, -0.515f, 0.0f } } };
			static const VectorF32 Scale2 = { { { 0.114f, 0.436f, -0.100f, 0.0f } } };

			Matrix M(Scale0, Scale1, Scale2, g_XMZero);
			Vector clr = Vector3Transform(rgb, M);

			return VectorSelect(rgb, clr, g_XMSelect1110);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorYUVToRGB(FVector yuv)
		{
			static const VectorF32 Scale1 = { { { 0.0f, -0.395f, 2.032f, 0.0f } } };
			static const VectorF32 Scale2 = { { { 1.140f, -0.581f, 0.0f, 0.0f } } };

			Matrix M(g_XMOne, Scale1, Scale2, g_XMZero);
			Vector clr = Vector3Transform(yuv, M);

			return VectorSelect(yuv, clr, g_XMSelect1110);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorRGBToYUV_HD(FVector rgb)
		{
			static const VectorF32 Scale0 = { { { 0.2126f, -0.0997f, 0.6150f, 0.0f } } };
			static const VectorF32 Scale1 = { { { 0.7152f, -0.3354f, -0.5586f, 0.0f } } };
			static const VectorF32 Scale2 = { { { 0.0722f, 0.4351f, -0.0564f, 0.0f } } };

			Matrix M(Scale0, Scale1, Scale2, g_XMZero);
			Vector clr = Vector3Transform(rgb, M);

			return VectorSelect(rgb, clr, g_XMSelect1110);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorYUVToRGB_HD(FVector yuv)
		{
			static const VectorF32 Scale1 = { { { 0.0f, -0.2153f, 2.1324f, 0.0f } } };
			static const VectorF32 Scale2 = { { { 1.2803f, -0.3806f, 0.0f, 0.0f } } };

			Matrix M(g_XMOne, Scale1, Scale2, g_XMZero);
			Vector clr = Vector3Transform(yuv, M);

			return VectorSelect(yuv, clr, g_XMSelect1110);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorRGBToXYZ(FVector rgb)
		{
			static const VectorF32 Scale0 = { { { 0.4887180f, 0.1762044f, 0.0000000f, 0.0f } } };
			static const VectorF32 Scale1 = { { { 0.3106803f, 0.8129847f, 0.0102048f, 0.0f } } };
			static const VectorF32 Scale2 = { { { 0.2006017f, 0.0108109f, 0.9897952f, 0.0f } } };
			static const VectorF32 Scale = { { { 1.f / 0.17697f, 1.f / 0.17697f, 1.f / 0.17697f, 0.0f } } };

			Matrix M(Scale0, Scale1, Scale2, g_XMZero);
			Vector clr = VectorMultiply(Vector3Transform(rgb, M), Scale);

			return VectorSelect(rgb, clr, g_XMSelect1110);
		}

		inline Vector XM_CALLCONV ColorXYZToRGB(FVector xyz)
		{
			static const VectorF32 Scale0 = { { { 2.3706743f, -0.5138850f, 0.0052982f, 0.0f } } };
			static const VectorF32 Scale1 = { { { -0.9000405f, 1.4253036f, -0.0146949f, 0.0f } } };
			static const VectorF32 Scale2 = { { { -0.4706338f, 0.0885814f, 1.0093968f, 0.0f } } };
			static const VectorF32 Scale = { { { 0.17697f, 0.17697f, 0.17697f, 0.0f } } };

			Matrix M(Scale0, Scale1, Scale2, g_XMZero);
			Vector clr = Vector3Transform(VectorMultiply(xyz, Scale), M);

			return VectorSelect(xyz, clr, g_XMSelect1110);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorXYZToSRGB(FVector xyz)
		{
			static const VectorF32 Scale0 = { { { 3.2406f, -0.9689f, 0.0557f, 0.0f } } };
			static const VectorF32 Scale1 = { { { -1.5372f, 1.8758f, -0.2040f, 0.0f } } };
			static const VectorF32 Scale2 = { { { -0.4986f, 0.0415f, 1.0570f, 0.0f } } };
			static const VectorF32 Cutoff = { { { 0.0031308f, 0.0031308f, 0.0031308f, 0.0f } } };
			static const VectorF32 Exp = { { { 1.0f / 2.4f, 1.0f / 2.4f, 1.0f / 2.4f, 1.0f } } };

			Matrix M(Scale0, Scale1, Scale2, g_XMZero);
			Vector lclr = Vector3Transform(xyz, M);

			Vector sel = VectorGreater(lclr, Cutoff);

			// clr = 12.92 * lclr for lclr <= 0.0031308f
			Vector smallC = VectorMultiply(lclr, g_XMsrgbScale);

			// clr = (1+a)*pow(lclr, 1/2.4) - a for lclr > 0.0031308 (where a = 0.055)
			Vector largeC = VectorSubtract(VectorMultiply(g_XMsrgbA1, VectorPow(lclr, Exp)), g_XMsrgbA);

			Vector clr = VectorSelect(smallC, largeC, sel);

			return VectorSelect(xyz, clr, g_XMSelect1110);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorSRGBToXYZ(FVector srgb)
		{
			static const VectorF32 Scale0 = { { { 0.4124f, 0.2126f, 0.0193f, 0.0f } } };
			static const VectorF32 Scale1 = { { { 0.3576f, 0.7152f, 0.1192f, 0.0f } } };
			static const VectorF32 Scale2 = { { { 0.1805f, 0.0722f, 0.9505f, 0.0f } } };
			static const VectorF32 Cutoff = { { { 0.04045f, 0.04045f, 0.04045f, 0.0f } } };
			static const VectorF32 Exp = { { { 2.4f, 2.4f, 2.4f, 1.0f } } };

			Vector sel = VectorGreater(srgb, Cutoff);

			// lclr = clr / 12.92
			Vector smallC = VectorDivide(srgb, g_XMsrgbScale);

			// lclr = pow( (clr + a) / (1+a), 2.4 )
			Vector largeC = VectorPow(VectorDivide(VectorAdd(srgb, g_XMsrgbA), g_XMsrgbA1), Exp);

			Vector lclr = VectorSelect(smallC, largeC, sel);

			Matrix M(Scale0, Scale1, Scale2, g_XMZero);
			Vector clr = Vector3Transform(lclr, M);

			return VectorSelect(srgb, clr, g_XMSelect1110);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorRGBToSRGB(FVector rgb)
		{
			static const VectorF32 Cutoff = { { { 0.0031308f, 0.0031308f, 0.0031308f, 1.f } } };
			static const VectorF32 Linear = { { { 12.92f, 12.92f, 12.92f, 1.f } } };
			static const VectorF32 Scale = { { { 1.055f, 1.055f, 1.055f, 1.f } } };
			static const VectorF32 Bias = { { { 0.055f, 0.055f, 0.055f, 0.f } } };
			static const VectorF32 InvGamma = { { { 1.0f / 2.4f, 1.0f / 2.4f, 1.0f / 2.4f, 1.f } } };

			Vector V = VectorSaturate(rgb);
			Vector V0 = VectorMultiply(V, Linear);
			Vector V1 = VectorSubtract(VectorMultiply(Scale, VectorPow(V, InvGamma)), Bias);
			Vector select = VectorLess(V, Cutoff);
			V = VectorSelect(V1, V0, select);
			return VectorSelect(rgb, V, g_XMSelect1110);
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV ColorSRGBToRGB(FVector srgb)
		{
			static const VectorF32 Cutoff = { { { 0.04045f, 0.04045f, 0.04045f, 1.f } } };
			static const VectorF32 ILinear = { { { 1.f / 12.92f, 1.f / 12.92f, 1.f / 12.92f, 1.f } } };
			static const VectorF32 Scale = { { { 1.f / 1.055f, 1.f / 1.055f, 1.f / 1.055f, 1.f } } };
			static const VectorF32 Bias = { { { 0.055f, 0.055f, 0.055f, 0.f } } };
			static const VectorF32 Gamma = { { { 2.4f, 2.4f, 2.4f, 1.f } } };

			Vector V = VectorSaturate(srgb);
			Vector V0 = VectorMultiply(V, ILinear);
			Vector V1 = VectorPow(VectorMultiply(VectorAdd(V, Bias), Scale), Gamma);
			Vector select = VectorGreater(V, Cutoff);
			V = VectorSelect(V0, V1, select);
			return VectorSelect(srgb, V, g_XMSelect1110);
		}

		/****************************************************************************
		 *
		 * Miscellaneous
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------

		inline bool VerifyCPUSupport()
		{
#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			int CPUInfo[4] = { -1 };
			__cpuid(CPUInfo, 0);

#ifdef __AVX2__
			if (CPUInfo[0] < 7)
				return false;
#else
			if (CPUInfo[0] < 1)
				return false;
#endif

			__cpuid(CPUInfo, 1);

#if defined(__AVX2__) || defined(_XM_AVX2_INTRINSICS_)
			// The compiler can emit FMA3 instructions even without explicit intrinsics use
			if ((CPUInfo[2] & 0x38081001) != 0x38081001)
				return false; // No F16C/AVX/OSXSAVE/SSE4.1/FMA3/SSE3 support
#elif defined(_XM_FMA3_INTRINSICS_) && defined(_XM_F16C_INTRINSICS_)
			if ((CPUInfo[2] & 0x38081001) != 0x38081001)
				return false; // No F16C/AVX/OSXSAVE/SSE4.1/FMA3/SSE3 support
#elif defined(_XM_FMA3_INTRINSICS_)
			if ((CPUInfo[2] & 0x18081001) != 0x18081001)
				return false; // No AVX/OSXSAVE/SSE4.1/FMA3/SSE3 support
#elif defined(_XM_F16C_INTRINSICS_)
			if ((CPUInfo[2] & 0x38080001) != 0x38080001)
				return false; // No F16C/AVX/OSXSAVE/SSE4.1/SSE3 support
#elif defined(__AVX__) || defined(_XM_AVX_INTRINSICS_)
			if ((CPUInfo[2] & 0x18080001) != 0x18080001)
				return false; // No AVX/OSXSAVE/SSE4.1/SSE3 support
#elif defined(_XM_SSE4_INTRINSICS_)
			if ((CPUInfo[2] & 0x80001) != 0x80001)
				return false; // No SSE3/SSE4.1 support
#elif defined(_XM_SSE3_INTRINSICS_)
			if (!(CPUInfo[2] & 0x1))
				return false; // No SSE3 support  
#endif

	// The x64 processor model requires SSE2 support, but no harm in checking
			if ((CPUInfo[3] & 0x6000000) != 0x6000000)
				return false; // No SSE2/SSE support

#if defined(__AVX2__) || defined(_XM_AVX2_INTRINSICS_)
			__cpuidex(CPUInfo, 7, 0);
			if (!(CPUInfo[1] & 0x20))
				return false; // No AVX2 support
#endif

			return true;
#elif defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			// ARM-NEON support is required for the Windows on ARM platform
			return true;
#else
			// No intrinsics path always supported
			return true;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector XM_CALLCONV FresnelTerm
		(
			FVector CosIncidentAngle,
			FVector RefractionIndex
		)
		{
			assert(!Vector4IsInfinite(CosIncidentAngle));

			// Result = 0.5f * (g - c)^2 / (g + c)^2 * ((c * (g + c) - 1)^2 / (c * (g - c) + 1)^2 + 1) where
			// c = CosIncidentAngle
			// g = sqrt(c^2 + RefractionIndex^2 - 1)

#if defined(_XM_NO_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)

			Vector G = VectorMultiplyAdd(RefractionIndex, RefractionIndex, g_XMNegativeOne.v);
			G = VectorMultiplyAdd(CosIncidentAngle, CosIncidentAngle, G);
			G = VectorAbs(G);
			G = VectorSqrt(G);

			Vector S = VectorAdd(G, CosIncidentAngle);
			Vector D = VectorSubtract(G, CosIncidentAngle);

			Vector V0 = VectorMultiply(D, D);
			Vector V1 = VectorMultiply(S, S);
			V1 = VectorReciprocal(V1);
			V0 = VectorMultiply(g_XMOneHalf.v, V0);
			V0 = VectorMultiply(V0, V1);

			Vector V2 = VectorMultiplyAdd(CosIncidentAngle, S, g_XMNegativeOne.v);
			Vector V3 = VectorMultiplyAdd(CosIncidentAngle, D, g_XMOne.v);
			V2 = VectorMultiply(V2, V2);
			V3 = VectorMultiply(V3, V3);
			V3 = VectorReciprocal(V3);
			V2 = VectorMultiplyAdd(V2, V3, g_XMOne.v);

			Vector Result = VectorMultiply(V0, V2);

			Result = VectorSaturate(Result);

			return Result;

#elif defined(_XM_SSE_INTRINSICS_)
	// G = sqrt(abs((RefractionIndex^2-1) + CosIncidentAngle^2))
			Vector G = _mm_mul_ps(RefractionIndex, RefractionIndex);
			Vector vTemp = _mm_mul_ps(CosIncidentAngle, CosIncidentAngle);
			G = _mm_sub_ps(G, g_XMOne);
			vTemp = _mm_add_ps(vTemp, G);
			// max((0-vTemp),vTemp) == abs(vTemp)
			// The abs is needed to deal with refraction and cosine being zero
			G = _mm_setzero_ps();
			G = _mm_sub_ps(G, vTemp);
			G = _mm_max_ps(G, vTemp);
			// Last operation, the sqrt()
			G = _mm_sqrt_ps(G);

			// Calc G-C and G+C
			Vector GAddC = _mm_add_ps(G, CosIncidentAngle);
			Vector GSubC = _mm_sub_ps(G, CosIncidentAngle);
			// Perform the term (0.5f *(g - c)^2) / (g + c)^2 
			Vector vResult = _mm_mul_ps(GSubC, GSubC);
			vTemp = _mm_mul_ps(GAddC, GAddC);
			vResult = _mm_mul_ps(vResult, g_XMOneHalf);
			vResult = _mm_div_ps(vResult, vTemp);
			// Perform the term ((c * (g + c) - 1)^2 / (c * (g - c) + 1)^2 + 1)
			GAddC = _mm_mul_ps(GAddC, CosIncidentAngle);
			GSubC = _mm_mul_ps(GSubC, CosIncidentAngle);
			GAddC = _mm_sub_ps(GAddC, g_XMOne);
			GSubC = _mm_add_ps(GSubC, g_XMOne);
			GAddC = _mm_mul_ps(GAddC, GAddC);
			GSubC = _mm_mul_ps(GSubC, GSubC);
			GAddC = _mm_div_ps(GAddC, GSubC);
			GAddC = _mm_add_ps(GAddC, g_XMOne);
			// Multiply the two term parts
			vResult = _mm_mul_ps(vResult, GAddC);
			// Clamp to 0.0 - 1.0f
			vResult = _mm_max_ps(vResult, g_XMZero);
			vResult = _mm_min_ps(vResult, g_XMOne);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool ScalarNearEqual
		(
			float S1,
			float S2,
			float Epsilon
		)
		{
			float Delta = S1 - S2;
			return (fabsf(Delta) <= Epsilon);
		}

		//------------------------------------------------------------------------------
		// Modulo the range of the given angle such that -XM_PI <= Angle < XM_PI
		inline float ScalarModAngle
		(
			float Angle
		)
		{
			// Note: The modulo is performed with unsigned math only to work
			// around a precision error on numbers that are close to PI

			// Normalize the range from 0.0f to XM_2PI
			Angle = Angle + XM_PI;
			// Perform the modulo, unsigned
			float fTemp = fabsf(Angle);
			fTemp = fTemp - (XM_2PI * static_cast<float>(static_cast<int32_t>(fTemp / XM_2PI)));
			// Restore the number to the range of -XM_PI to XM_PI-epsilon
			fTemp = fTemp - XM_PI;
			// If the modulo'd value was negative, restore negation
			if (Angle < 0.0f) {
				fTemp = -fTemp;
			}
			return fTemp;
		}

		//------------------------------------------------------------------------------

		inline float ScalarSin
		(
			float Value
		)
		{
			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = XM_1DIV2PI * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - XM_2PI * quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
			if (y > XM_PIDIV2)
			{
				y = XM_PI - y;
			}
			else if (y < -XM_PIDIV2)
			{
				y = -XM_PI - y;
			}

			// 11-degree minimax approximation
			float y2 = y * y;
			return (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
		}

		//------------------------------------------------------------------------------

		inline float ScalarSinEst
		(
			float Value
		)
		{
			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = XM_1DIV2PI * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - XM_2PI * quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
			if (y > XM_PIDIV2)
			{
				y = XM_PI - y;
			}
			else if (y < -XM_PIDIV2)
			{
				y = -XM_PI - y;
			}

			// 7-degree minimax approximation
			float y2 = y * y;
			return (((-0.00018524670f * y2 + 0.0083139502f) * y2 - 0.16665852f) * y2 + 1.0f) * y;
		}

		//------------------------------------------------------------------------------

		inline float ScalarCos
		(
			float Value
		)
		{
			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = XM_1DIV2PI * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - XM_2PI * quotient;

			// Map y to [-pi/2,pi/2] with cos(y) = sign*cos(x).
			float sign;
			if (y > XM_PIDIV2)
			{
				y = XM_PI - y;
				sign = -1.0f;
			}
			else if (y < -XM_PIDIV2)
			{
				y = -XM_PI - y;
				sign = -1.0f;
			}
			else
			{
				sign = +1.0f;
			}

			// 10-degree minimax approximation
			float y2 = y * y;
			float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
			return sign * p;
		}

		//------------------------------------------------------------------------------

		inline float ScalarCosEst
		(
			float Value
		)
		{
			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = XM_1DIV2PI * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - XM_2PI * quotient;

			// Map y to [-pi/2,pi/2] with cos(y) = sign*cos(x).
			float sign;
			if (y > XM_PIDIV2)
			{
				y = XM_PI - y;
				sign = -1.0f;
			}
			else if (y < -XM_PIDIV2)
			{
				y = -XM_PI - y;
				sign = -1.0f;
			}
			else
			{
				sign = +1.0f;
			}

			// 6-degree minimax approximation
			float y2 = y * y;
			float p = ((-0.0012712436f * y2 + 0.041493919f) * y2 - 0.49992746f) * y2 + 1.0f;
			return sign * p;
		}

		//------------------------------------------------------------------------------


		inline void ScalarSinCos
		(
			float* pSin,
			float* pCos,
			float  Value
		)
		{
			assert(pSin);
			assert(pCos);

			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = XM_1DIV2PI * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - XM_2PI * quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
			float sign;
			if (y > XM_PIDIV2)
			{
				y = XM_PI - y;
				sign = -1.0f;
			}
			else if (y < -XM_PIDIV2)
			{
				y = -XM_PI - y;
				sign = -1.0f;
			}
			else
			{
				sign = +1.0f;
			}

			float y2 = y * y;

			// 11-degree minimax approximation
			*pSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

			// 10-degree minimax approximation
			float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
			*pCos = sign * p;
		}

		//------------------------------------------------------------------------------


		inline void ScalarSinCosEst
		(
			float* pSin,
			float* pCos,
			float  Value
		)
		{
			assert(pSin);
			assert(pCos);

			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = XM_1DIV2PI * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - XM_2PI * quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
			float sign;
			if (y > XM_PIDIV2)
			{
				y = XM_PI - y;
				sign = -1.0f;
			}
			else if (y < -XM_PIDIV2)
			{
				y = -XM_PI - y;
				sign = -1.0f;
			}
			else
			{
				sign = +1.0f;
			}

			float y2 = y * y;

			// 7-degree minimax approximation
			*pSin = (((-0.00018524670f * y2 + 0.0083139502f) * y2 - 0.16665852f) * y2 + 1.0f) * y;

			// 6-degree minimax approximation
			float p = ((-0.0012712436f * y2 + 0.041493919f) * y2 - 0.49992746f) * y2 + 1.0f;
			*pCos = sign * p;
		}

		//------------------------------------------------------------------------------

		inline float ScalarASin
		(
			float Value
		)
		{
			// Clamp input to [-1,1].
			bool nonnegative = (Value >= 0.0f);
			float x = fabsf(Value);
			float omx = 1.0f - x;
			if (omx < 0.0f)
			{
				omx = 0.0f;
			}
			float root = sqrtf(omx);

			// 7-degree minimax approximation
			float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
			result *= root;  // acos(|x|)

			// acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
			return (nonnegative ? XM_PIDIV2 - result : result - XM_PIDIV2);
		}

		//------------------------------------------------------------------------------

		inline float ScalarASinEst
		(
			float Value
		)
		{
			// Clamp input to [-1,1].
			bool nonnegative = (Value >= 0.0f);
			float x = fabsf(Value);
			float omx = 1.0f - x;
			if (omx < 0.0f)
			{
				omx = 0.0f;
			}
			float root = sqrtf(omx);

			// 3-degree minimax approximation
			float result = ((-0.0187293f * x + 0.0742610f) * x - 0.2121144f) * x + 1.5707288f;
			result *= root;  // acos(|x|)

			// acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
			return (nonnegative ? XM_PIDIV2 - result : result - XM_PIDIV2);
		}

		//------------------------------------------------------------------------------

		inline float ScalarACos
		(
			float Value
		)
		{
			// Clamp input to [-1,1].
			bool nonnegative = (Value >= 0.0f);
			float x = fabsf(Value);
			float omx = 1.0f - x;
			if (omx < 0.0f)
			{
				omx = 0.0f;
			}
			float root = sqrtf(omx);

			// 7-degree minimax approximation
			float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
			result *= root;

			// acos(x) = pi - acos(-x) when x < 0
			return (nonnegative ? result : XM_PI - result);
		}

		//------------------------------------------------------------------------------

		inline float ScalarACosEst
		(
			float Value
		)
		{
			// Clamp input to [-1,1].
			bool nonnegative = (Value >= 0.0f);
			float x = fabsf(Value);
			float omx = 1.0f - x;
			if (omx < 0.0f)
			{
				omx = 0.0f;
			}
			float root = sqrtf(omx);

			// 3-degree minimax approximation
			float result = ((-0.0187293f * x + 0.0742610f) * x - 0.2121144f) * x + 1.5707288f;
			result *= root;

			// acos(x) = pi - acos(-x) when x < 0
			return (nonnegative ? result : XM_PI - result);
		}



#ifdef _PREFAST_
#pragma prefast(pop)
#endif

#pragma warning(pop)

	}
}

