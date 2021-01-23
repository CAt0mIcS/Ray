#pragma once

#include "RRBase.h"

#include <../../RayUtil/include/RayUtil/RMath.h>


namespace At0::Ray
{
	class Shader;

	// -----------------------------------------------------------------
	class RR_API ConstantBuffer
	{
	public:
		virtual void Bind() = 0;

		virtual void Update(const Matrix& data) {};
		virtual void Update(const Float3& data) {};

		virtual ~ConstantBuffer() = default;

	protected:
		ConstantBuffer() = default;
	};


	// -----------------------------------------------------------------
	class RR_API PixelConstantBuffer final : public ConstantBuffer
	{
	public:
		template<typename T>
		static Scope<ConstantBuffer> Create(std::string_view name, const Shader * pShader, const T & data);

		template<typename T>
		static Scope<ConstantBuffer> Create(std::string_view name, const Shader* pShader);

		PixelConstantBuffer() = delete;
	};

	// -----------------------------------------------------------------
	class RR_API VertexConstantBuffer final : public ConstantBuffer
	{
	public:
		template<typename T>
		static Scope<ConstantBuffer> Create(std::string_view name, const Shader * pShader, const T & data);

		template<typename T>
		static Scope<ConstantBuffer> Create(std::string_view name, const Shader* pShader);

		VertexConstantBuffer() = delete;
	};

#ifdef _MSC_VER
	// RAY_TODO: Add more data
	// -----------------------------------------------------------------
	// explicit template instantiation

	// PixelConstantBuffer
	template RR_API Scope<ConstantBuffer> PixelConstantBuffer::Create<Matrix>(std::string_view name, const Shader* pShader, const Matrix& data);
	template RR_API Scope<ConstantBuffer> PixelConstantBuffer::Create<Float3>(std::string_view name, const Shader* pShader, const Float3& data);
	template RR_API Scope<ConstantBuffer> PixelConstantBuffer::Create<Matrix>(std::string_view name, const Shader* pShader);
	template RR_API Scope<ConstantBuffer> PixelConstantBuffer::Create<Float3>(std::string_view name, const Shader* pShader);

	// VertexConstantBuffer
	template RR_API Scope<ConstantBuffer> VertexConstantBuffer::Create<Matrix>(std::string_view name, const Shader* pShader, const Matrix& data);
	template RR_API Scope<ConstantBuffer> VertexConstantBuffer::Create<Float3>(std::string_view name, const Shader* pShader, const Float3& data);
	template RR_API Scope<ConstantBuffer> VertexConstantBuffer::Create<Matrix>(std::string_view name, const Shader* pShader);
	template RR_API Scope<ConstantBuffer> VertexConstantBuffer::Create<Float3>(std::string_view name, const Shader* pShader);
#endif
}
