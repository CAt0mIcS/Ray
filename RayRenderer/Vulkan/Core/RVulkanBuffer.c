#include "Rpch.h"

#include "../Core/RBuffer.h"
#include "../Core/RUtils.h"


RrError RrCreateBuffer(
	RrLogicalDevice pDevice, const RrBufferCreateInfo* const pCreateInfo, RrBuffer* ppBuffer)
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

	VkResult error = vkCreateBuffer((VkDevice)pDevice, &createInfo, NULL, (VkBuffer*)ppBuffer);
	if (error != VK_SUCCESS)
		return GetError(error);

	return RrErrorNone;
}

void RrBufferGetMemoryRequirements(
	RrLogicalDevice pDevice, const RrBuffer pBuffer, RrMemoryRequirements* const pMemRequirements)
{
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements((VkDevice)pDevice, (VkBuffer)pBuffer, &memRequirements);

	pMemRequirements->size = memRequirements.size;
	pMemRequirements->alignment = memRequirements.alignment;
	pMemRequirements->memoryTypeBits = memRequirements.memoryTypeBits;
}

RrError RrBindBufferMemory(
	RrLogicalDevice pDevice, RrBuffer buffer, RrDeviceMemory memory, RrDeviceSize memoryOffset)
{
	return GetError(vkBindBufferMemory(
		(VkDevice)pDevice, (VkBuffer)buffer, (VkDeviceMemory)memory, memoryOffset));
}
