#include "Rpch.h"

#include "../Pipeline/RPipeline.h"
#include "../Core/RUtils.h"


RrError RrCreatePipelineLayout(RrLogicalDevice device,
	const RrPipelineLayoutCreateInfo* pCreateInfo, RrPipelineLayout* pLayout)
{
	return RrErrorSuccess;
}

RrError RrCreateGraphicsPipeline(RrLogicalDevice device, RrPipelineCache pipelineCache,
	const RrGraphicsPipelineCreateInfo* pCreateInfo, RrPipeline* pPipeline)
{
	return RrErrorSuccess;
}

void RrDestroyPipelineLayout(RrLogicalDevice device, RrPipelineLayout pipelineLayout) {}

void RrDestroyPipeline(RrLogicalDevice device, RrPipeline pipeline) {}


void RrCmdBindPipeline(
	RrCommandBuffer commandBuffer, RrPipelineBindPoint pipelineBindPoint, RrPipeline pipeline)
{
}

void RrCmdSetViewport(RrCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount,
	const RrViewport* pViewports)
{
}

void RrCmdSetScissor(RrCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount,
	const RrRect2D* pScissors)
{
}

void RrCmdDrawIndexed(RrCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount,
	uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
}

void RrCmdPipelineBarrier(RrCommandBuffer commandBuffer, RrPipelineStageFlags srcStageMask,
	RrPipelineStageFlags dstStageMask, RrDependencyFlags dependencyFlags,
	uint32_t memoryBarrierCount, const RrMemoryBarrier* pMemoryBarriers,
	uint32_t bufferMemoryBarrierCount, const RrBufferMemoryBarrier* pBufferMemoryBarriers,
	uint32_t imageMemoryBarrierCount, const RrImageMemoryBarrier* pImageMemoryBarriers)
{
}