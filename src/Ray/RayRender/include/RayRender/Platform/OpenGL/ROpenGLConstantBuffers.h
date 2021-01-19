#pragma once

#include "../../RConstantBuffers.h"


namespace At0::Ray
{
	// -----------------------------------------------------------------
	template<typename T>
	class RR_API OpenGLConstantBuffer : public ConstantBuffer
	{
	public:
		OpenGLConstantBuffer(const T& data);
		OpenGLConstantBuffer();

		virtual void Update(const Matrix& data) override;
		virtual void Update(const Float3& data) override;
	};


	// -----------------------------------------------------------------
	template<typename T>
	class RR_API OpenGLPixelConstantBuffer : public OpenGLConstantBuffer<T>
	{
	public:
		// "Import" constructor from base class
		using OpenGLConstantBuffer<T>::OpenGLConstantBuffer;

		virtual void Bind() override;
	};


	// -----------------------------------------------------------------
	template<typename T>
	class RR_API OpenGLVertexConstantBuffer : public OpenGLConstantBuffer<T>
	{
	public:
		// "Import" constructor from base class
		using OpenGLConstantBuffer<T>::OpenGLConstantBuffer;

		virtual void Bind() override;
	};
}