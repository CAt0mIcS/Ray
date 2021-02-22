#pragma once

#include "RPipeline.h"


namespace At0::Ray
{
	class ComputePipeline : public Pipeline
	{
	public:
		VkPipelineBindPoint GetBindPoint() const override { return VK_PIPELINE_BIND_POINT_COMPUTE; }
	};
}  // namespace At0::Ray
