#include "Rpch.h"
#include <../Pipeline/RUniform.h>
#include <../Core/RUtils.h>


void RrCmdPushConstants(RrCommandBuffer commandBuffer, RrPipelineLayout pipelineLayout,
	RrShaderStageFlags shaderStage, uint32_t offset, uint32_t size, const void* pValues)
{
	vkCmdPushConstants((VkCommandBuffer)commandBuffer, (VkPipelineLayout)pipelineLayout,
		(VkShaderStageFlags)shaderStage, offset, size, pValues);
}
