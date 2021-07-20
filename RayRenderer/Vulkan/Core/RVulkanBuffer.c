#include "Rpch.h"

#include "../Core/RBuffer.h"
#include "../Core/RUtils.h"


RrError RrCreateBuffer(
	RrLogicalDevice device, const RrBufferCreateInfo* pCreateInfo, RrBuffer* pBuffer)
{
	VkBufferCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = 0;
	createInfo.size = pCreateInfo->size;
	createInfo.usage = pCreateInfo->usage;
	createInfo.sharingMode = (VkSharingMode)pCreateInfo->sharingMode;
	createInfo.queueFamilyIndexCount = pCreateInfo->queueFamilyIndexCount;
	createInfo.pQueueFamilyIndices = pCreateInfo->pQueueFamilyIndices;

	return GetError(vkCreateBuffer((VkDevice)device, &createInfo, NULL, (VkBuffer*)pBuffer));
}

void RrBufferGetMemoryRequirements(
	RrLogicalDevice device, RrBuffer buffer, RrMemoryRequirements* pMemRequirements)
{
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements((VkDevice)device, (VkBuffer)buffer, &memRequirements);

	pMemRequirements->size = memRequirements.size;
	pMemRequirements->alignment = memRequirements.alignment;
	pMemRequirements->memoryTypeBits = memRequirements.memoryTypeBits;
}

RrError RrBindBufferMemory(
	RrLogicalDevice device, RrBuffer buffer, RrDeviceMemory memory, RrDeviceSize memoryOffset)
{
	return GetError(vkBindBufferMemory(
		(VkDevice)device, (VkBuffer)buffer, (VkDeviceMemory)memory, memoryOffset));
}

void RrDestroyBuffer(RrLogicalDevice device, RrBuffer buffer)
{
	vkDestroyBuffer((VkDevice)device, (VkBuffer)buffer, NULL);
}

void RrCmdBindIndexBuffer(
	RrCommandBuffer commandBuffer, RrBuffer buffer, RrDeviceSize offset, RrIndexType indexType)
{
	vkCmdBindIndexBuffer((VkCommandBuffer)commandBuffer, (VkBuffer)buffer, offset, indexType);
}

void RrCmdBindVertexBuffers(RrCommandBuffer commandBuffer, uint32_t firstBinding,
	uint32_t bindingCount, const RrBuffer* pBuffers, const RrDeviceSize* pOffsets)
{
	vkCmdBindVertexBuffers((VkCommandBuffer)commandBuffer, firstBinding, bindingCount,
		(const VkBuffer*)pBuffers, (const VkDeviceSize*)pOffsets);
}

void RrCmdCopyBuffer(RrCommandBuffer commandBuffer, RrBuffer srcBuffer, RrBuffer dstBuffer,
	uint32_t regionCount, const RrBufferCopy* pRegions)
{
	vkCmdCopyBuffer((VkCommandBuffer)commandBuffer, (VkBuffer)srcBuffer, (VkBuffer)dstBuffer,
		regionCount, (const VkBufferCopy*)pRegions);
}
