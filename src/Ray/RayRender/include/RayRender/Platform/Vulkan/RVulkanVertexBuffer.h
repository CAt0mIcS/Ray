#pragma once

#include "../../RVertexBuffer.h"
#include "Internal/RVulkanGraphicsResources.h"


namespace At0::Ray
{
	class RR_API VulkanVertexBuffer : public VertexBuffer, VulkanGraphicsResources
	{
	public:
		VulkanVertexBuffer(const VertexData& data);

		virtual void Bind() override;
	};
}
