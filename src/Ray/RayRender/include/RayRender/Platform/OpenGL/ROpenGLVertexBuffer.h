#pragma once

#include "../../RVertexBuffer.h"


namespace At0::Ray
{
	class VertexShader;

	class RR_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const VertexData& data, const Scope<VertexShader>& vShader);
		~OpenGLVertexBuffer();

		virtual void Bind() override;

	private:
		uint32_t m_Buffer;
	};
}
