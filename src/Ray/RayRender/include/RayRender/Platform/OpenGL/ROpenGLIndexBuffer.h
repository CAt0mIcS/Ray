#pragma once


#include "../../RIndexBuffer.h"


namespace At0::Ray
{
	class RR_API OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(std::initializer_list<uint32_t> indices);

		virtual void Bind() override;
	};
}

