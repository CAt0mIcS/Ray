#pragma once

#include "../../RTopology.h"
#include "Internal/RVulkanGraphicsResources.h"


namespace At0::Ray
{
	class RR_API VulkanTopology : public Topology, VulkanGraphicsResources
	{
	public:


		virtual void Bind() override {}
	};
}

