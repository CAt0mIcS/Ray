#pragma once


#include "../../RIndexBuffer.h"


namespace At0::Ray
{
	class RR_API OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(std::initializer_list<uint32_t> indices);
		~OpenGLIndexBuffer();

		virtual void Bind() override;
		virtual uint32_t GetIndicesCount() const override;

	private:
		uint32_t m_Buffer;
		uint32_t m_NumIndices;
	};
}

