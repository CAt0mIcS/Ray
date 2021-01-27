#pragma once

#include "../../RIndexBuffer.h"
#include "Internal/RVulkanGraphicsResources.h"


namespace At0::Ray
{
	class RR_API VulkanIndexBuffer : public IndexBuffer, VulkanGraphicsResources
	{
	public:


		virtual void Bind() override {}
		virtual uint32_t GetIndicesCount() const override { return 0; }
	};
}

