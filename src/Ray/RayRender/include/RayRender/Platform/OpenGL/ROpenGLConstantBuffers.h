#pragma once

#include "../../RConstantBuffers.h"


namespace At0::Ray
{
	class PixelShader;
	class VertexShader;

	// -----------------------------------------------------------------
	template<typename T>
	class RR_API OpenGLConstantBuffer : public ConstantBuffer
	{
	public:
		OpenGLConstantBuffer(const T& data);
		OpenGLConstantBuffer();

		virtual void Update(const T& data) override;

	protected:
		uint32_t m_Buffer;
	};


	// -----------------------------------------------------------------
	template<typename T>
	class RR_API OpenGLPixelConstantBuffer : public OpenGLConstantBuffer<T>
	{
	public:
		OpenGLPixelConstantBuffer(std::string_view name, const PixelShader* pShader, const T& data);
		OpenGLPixelConstantBuffer(std::string_view name, const PixelShader* pShader);

		virtual void Bind() override;

	private:
		const PixelShader* m_PShader;
		std::string m_Name;
	};


	// -----------------------------------------------------------------
	template<typename T>
	class RR_API OpenGLVertexConstantBuffer : public OpenGLConstantBuffer<T>
	{
	public:
		OpenGLVertexConstantBuffer(std::string_view name, const VertexShader* pShader, const T& data);
		OpenGLVertexConstantBuffer(std::string_view name, const VertexShader* pShader);

		virtual void Bind() override;

	private:
		const VertexShader* m_VShader;
		std::string m_Name;
	};
}