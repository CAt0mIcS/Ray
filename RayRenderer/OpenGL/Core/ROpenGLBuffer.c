#include "Rpch.h"

#include "../Core/RBuffer.h"
#include "../Core/RUtils.h"


RrError RrCreateBuffer(
	RrLogicalDevice device, const RrBufferCreateInfo* pCreateInfo, RrBuffer* pBuffer)
{
	return RrErrorSuccess;
}

void RrBufferGetMemoryRequirements(
	RrLogicalDevice device, RrBuffer buffer, RrMemoryRequirements* pMemRequirements)
{
}

RrError RrBindBufferMemory(
	RrLogicalDevice device, RrBuffer buffer, RrDeviceMemory memory, RrDeviceSize memoryOffset)
{
	return RrErrorSuccess;
}

void RrDestroyBuffer(RrLogicalDevice device, RrBuffer buffer) {}


void RrCmdBindIndexBuffer(
	RrCommandBuffer commandBuffer, RrBuffer buffer, RrDeviceSize offset, RrIndexType indexType)
{
}

void RrCmdBindVertexBuffers(RrCommandBuffer commandBuffer, uint32_t firstBinding,
	uint32_t bindingCount, const RrBuffer* pBuffers, const RrDeviceSize* pOffsets)
{
}

void RrCmdCopyBuffer(RrCommandBuffer commandBuffer, RrBuffer srcBuffer, RrBuffer dstBuffer,
	uint32_t regionCount, const RrBufferCopy* pRegions)
{
}