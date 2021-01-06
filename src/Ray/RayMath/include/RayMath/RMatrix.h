#pragma once

#include "RMathInternals.h"
#include "RVector.h"

#include <utility>


namespace At0::Ray
{
	//------------------------------------------------------------------------------
// Matrix type: Sixteen 32 bit floating point components aligned on a
// 16 byte boundary and mapped to four hardware vector registers

	struct Matrix;

	// Fix-up for (1st) Matrix parameter to pass in-register for ARM64 and vector call; by reference otherwise
#if ( defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || _XM_VECTORCALL_ ) && !defined(RAY_NO_INTRINSICS)
	typedef const Matrix FMatrix;
#else
	typedef const Matrix& FMatrix;
#endif

	// Fix-up for (2nd+) Matrix parameters to pass by reference
	typedef const Matrix& CMatrix;

#ifdef RAY_NO_INTRINSICS
	struct Matrix
#else
	RAY_ALIGNED(16) struct Matrix
#endif
	{
#ifdef RAY_NO_INTRINSICS
		union
		{
			Vector r[4];
			struct
			{
				float f00, f01, f02, f03;
				float f10, f11, f12, f13;
				float f20, f21, f22, f23;
				float f30, f31, f32, f33;
			};
			float m[4][4];
		};
#else
		Vector r[4];
#endif

		Matrix() = default;

		Matrix(const Matrix&) = default;

		Matrix& operator=(const Matrix&) = default;

		Matrix(Matrix&&) = default;
		Matrix& operator=(Matrix&&) = default;

		constexpr Matrix(FVector R0, FVector R1, FVector R2, CVector R3) : r{ R0,R1,R2,R3 } {}
		constexpr Matrix(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

		// --------------------------------------------------------------------
		// Operators
		Matrix operator+ () const { return *this; }
		Matrix operator- () const;

		Matrix& RAYMATH_CALLCONV     operator+= (FMatrix M);
		Matrix& RAYMATH_CALLCONV     operator-= (FMatrix M);
		Matrix& RAYMATH_CALLCONV     operator*= (FMatrix M);
		Matrix& operator*= (float S);
		Matrix& operator/= (float S);

		const Vector& RAYMATH_CALLCONV operator[](uint32_t idx) const { assert(idx < 4 && "Index Out Of Range"); return r[idx]; }
		Vector& RAYMATH_CALLCONV operator[](uint32_t idx) { return const_cast<Vector&>(std::as_const(*this)[idx]); }

		Matrix RAYMATH_CALLCONV     operator+ (FMatrix M) const;
		Matrix RAYMATH_CALLCONV     operator- (FMatrix M) const;
		Matrix RAYMATH_CALLCONV     operator* (FMatrix M) const;
		Matrix operator* (float S) const;
		Matrix operator/ (float S) const;

		friend Matrix RAYMATH_CALLCONV operator* (float S, FMatrix M);

		// --------------------------------------------------------------------
		// Helper Functions
		static Matrix Identity();

	private:
		static Matrix Multiply(FMatrix M1, CMatrix M2);
	};


	inline constexpr Matrix::Matrix(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
	)
		:
		f00(m00), f01(m01), f02(m02), f03(m03),
		f10(m10), f11(m11), f12(m12), f13(m13),
		f20(m20), f21(m21), f22(m22), f23(m23),
		f30(m30), f31(m31), f32(m32), f33(m33)
	{
	}


	inline Matrix Matrix::operator-() const
	{
		Matrix mat;
		mat[0] = -r[0];
		mat[1] - -r[1];
		mat[2] - -r[2];
		mat[3] - -r[3];
		return mat;
	}

	inline Matrix& Matrix::operator+=(FMatrix M)
	{
		r[0] += M.r[0];
		r[1] += M.r[1];
		r[2] += M.r[2];
		r[3] += M.r[3];
		return *this;
	}

	inline Matrix& Matrix::operator-=(FMatrix M)
	{
		r[0] -= M.r[0];
		r[1] -= M.r[1];
		r[2] -= M.r[2];
		r[3] -= M.r[3];
		return *this;
	}

	inline Matrix& Matrix::operator*=(FMatrix M)
	{
		*this = Multiply(*this, M);
		return *this;
	}

	inline Matrix& Matrix::operator*=(float S)
	{
		r[0] = r[0] * S;
		r[1] = r[1] * S;
		r[2] = r[2] * S;
		r[3] = r[3] * S;
		return *this;
	}

	inline Matrix& Matrix::operator/=(float S)
	{
		r[0] = r[0] / S;
		r[1] = r[1] / S;
		r[2] = r[2] / S;
		r[3] = r[3] / S;
		return *this;
	}


	inline Matrix Matrix::operator+(FMatrix M) const
	{
		Matrix r;
		r[0] += M[0];
		r[1] += M[1];
		r[2] += M[2];
		r[3] += M[3];
		return r;
	}

	inline Matrix Matrix::operator-(FMatrix M) const
	{
		Matrix r;
		r[0] -= M[0];
		r[1] -= M[1];
		r[2] -= M[2];
		r[3] -= M[3];
		return r;
	}

	inline Matrix Matrix::operator*(FMatrix M) const
	{
		return Multiply(*this, M);
	}

	inline Matrix Matrix::operator*(float S) const
	{
		Matrix R;
		R[0] = r[0] * S;
		R[1] = r[1] * S;
		R[2] = r[2] * S;
		R[3] = r[3] * S;
		return R;
	}

	inline Matrix Matrix::operator/(float S) const
	{
		Matrix R;
		R[0] = r[0] / S;
		R[1] = r[1] / S;
		R[2] = r[2] / S;
		R[3] = r[3] / S;
		return R;
	}


	//friend Matrix     XM_CALLCONV     operator* (float S, FMatrix M);

	// --------------------------------------------------------------------
	// Helper Functions
	inline Matrix Matrix::Identity()
	{
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	inline Matrix Matrix::Multiply(FMatrix M1, CMatrix M2)
	{
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
	}

}