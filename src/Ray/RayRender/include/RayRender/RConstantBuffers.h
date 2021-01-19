#pragma once

#include "RRBase.h"

#include <../../RayUtil/include/RayUtil/RMath.h>


namespace At0::Ray
{
	class RR_API ConstantBuffer
	{
	public:
		template<typename T>
		static Scope<ConstantBuffer> Create(const T& data);

		virtual void Bind() = 0;

		virtual ~ConstantBuffer() = default;
	};

	// RAY_TODO: Add more data
	// explicit instantiation
	template RR_API Scope<ConstantBuffer> ConstantBuffer::Create<Matrix>(const Matrix& data);
	template RR_API Scope<ConstantBuffer> ConstantBuffer::Create<Float3>(const Float3& data);
}