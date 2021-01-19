#pragma once

#include "../../RConstantBuffers.h"


namespace At0::Ray
{
	template<typename T>
	class RR_API OpenGLConstantBuffer : public ConstantBuffer
	{
	public:
		OpenGLConstantBuffer(const T& data);

		virtual void Bind() override;
	};
}