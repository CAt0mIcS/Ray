#pragma once

#include "RRBase.h"

#include <../../RayUtil/include/RayUtil/RMath.h>


namespace At0::Ray
{
	// -----------------------------------------------------------------
	class RR_API ConstantBuffer
	{
	public:
		virtual void Bind() = 0;

		virtual void Update(const Matrix& data) {};
		virtual void Update(const Float3& data) {};

		virtual ~ConstantBuffer() = default;
	};


	// -----------------------------------------------------------------
	class RR_API PixelConstantBuffer final : public ConstantBuffer
	{
	public:
		template<typename T>
		static Scope<ConstantBuffer> Create(const T & data);

		template<typename T>
		static Scope<ConstantBuffer> Create();
	};

	// -----------------------------------------------------------------
	class RR_API VertexConstantBuffer final : public ConstantBuffer
	{
	public:
		template<typename T>
		static Scope<ConstantBuffer> Create(const T & data);

		template<typename T>
		static Scope<ConstantBuffer> Create();
	};

#ifdef _MSC_VER
	// RAY_TODO: Add more data
	// -----------------------------------------------------------------
	// explicit template instantiation

	// PixelConstantBuffer
	template RR_API Scope<ConstantBuffer> PixelConstantBuffer::Create<Matrix>(const Matrix& data);
	template RR_API Scope<ConstantBuffer> PixelConstantBuffer::Create<Float3>(const Float3& data);
	template RR_API Scope<ConstantBuffer> PixelConstantBuffer::Create<Matrix>();
	template RR_API Scope<ConstantBuffer> PixelConstantBuffer::Create<Float3>();

	// VertexConstantBuffer
	template RR_API Scope<ConstantBuffer> VertexConstantBuffer::Create<Matrix>(const Matrix& data);
	template RR_API Scope<ConstantBuffer> VertexConstantBuffer::Create<Float3>(const Float3& data);
	template RR_API Scope<ConstantBuffer> VertexConstantBuffer::Create<Matrix>();
	template RR_API Scope<ConstantBuffer> VertexConstantBuffer::Create<Float3>();
#endif
}
