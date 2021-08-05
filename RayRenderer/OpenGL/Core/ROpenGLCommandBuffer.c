#include "Rpch.h"

#include "../Core/RCommandBuffer.h"
#include "../Core/RUtils.h"


RrError RrAllocateCommandBuffers(RrLogicalDevice device, RrCommandBufferAllocateInfo* pAllocInfo,
	RrCommandBuffer* pCommandBuffers)
{
	return RrErrorSuccess;
}

RrError RrBeginCommandBuffer(RrCommandBuffer commandBuffer, RrCommandBufferBeginInfo* pBeginInfo)
{
	return RrErrorSuccess;
}

void RrExecuteCommands(RrCommandBuffer commandBuffer, uint32_t secondaryCommandBufferCount,
	const RrCommandBuffer* pSecondaryCommandBuffers)
{
}

RrError RrQueueSubmit(
	RrQueue queue, uint32_t submitCount, const RrSubmitInfo* pSubmits, RrFence fence)
{
	return RrErrorSuccess;
}

RrError RrQueueWaitIdle(RrQueue queue)
{
	return RrErrorSuccess;
}

RrError RrEndCommandBuffer(RrCommandBuffer commandBuffer)
{
	return RrErrorSuccess;
}

void RrFreeCommandBuffers(RrLogicalDevice device, RrCommandPool commandPool,
	uint32_t commandBufferCount, const RrCommandBuffer* pCommandBuffers)
{
}