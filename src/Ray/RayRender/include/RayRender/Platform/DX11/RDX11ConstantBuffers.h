#pragma once

#include "../../RConstantBuffers.h"


namespace At0::Ray
{
	template<typename T>
	class RR_API DX11ConstantBuffer : public ConstantBuffer
	{
	public:
		DX11ConstantBuffer(const T& data);

		virtual void Bind() override;
	};
}