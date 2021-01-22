#pragma once

#include "../../RVertexBuffer.h"


namespace At0::Ray
{
	class RR_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const VertexData& data);
		~OpenGLVertexBuffer();

		virtual void Bind() override;

	private:
		uint32_t m_Buffer;
	};
}
