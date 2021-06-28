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

	VkResult error = vkCreateBuffer(pDevice, &createInfo, NULL, ppBuffer);
	if (error != VK_SUCCESS)
		if (LogError("Failed to create buffer"))
			return RrErrorInitializationFailed;

	return RrErrorNone;
}
