#pragma once

#include "../../RConstantBuffers.h"


namespace At0::Ray
{
	// -----------------------------------------------------------------
	template<typename T>
	class RR_API OpenGLConstantBuffer : public ConstantBuffer
	{
	public:
		OpenGLConstantBuffer(std::string_view name, const Shader* pShader, const T& data);
		OpenGLConstantBuffer(std::string_view name, const Shader* pShader);

		virtual void Update(const T& data) override;

	protected:
		uint32_t m_Buffer;
		const Shader* m_Shader;
		T m_Data;
		std::string m_Name;
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