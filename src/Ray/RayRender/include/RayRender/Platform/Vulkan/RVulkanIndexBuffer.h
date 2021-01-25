#pragma once


#include "../../RIndexBuffer.h"


namespace At0::Ray
{
	class RR_API VulkanIndexBuffer : public IndexBuffer
	{
	public:
		
		
		virtual void Bind() override {}
		virtual uint32_t GetIndicesCount() const override { return 0; }
	};
}

