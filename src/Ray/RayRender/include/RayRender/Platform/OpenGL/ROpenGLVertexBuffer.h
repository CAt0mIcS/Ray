#pragma once

#include "../../RVertexBuffer.h"


namespace At0::Ray
{
	class RR_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(std::initializer_list<Vertex> data);
		~OpenGLVertexBuffer();

		virtual void Bind() override;

	private:
		uint32_t m_Buffer;
	};
}
