#include "Rpch.h"

#include "../Core/RCommandPool.h"
#include "../Core/RUtils.h"


RrError RrCreateCommandPool(
	RrLogicalDevice device, const RrCommandPoolCreateInfo* pCreateInfo, RrCommandPool* pCommandPool)
{
	VkCommandPoolCreateInfo commandPoolCreateInfo;
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.pNext = NULL;
	commandPoolCreateInfo.flags = (VkCommandPoolCreateFlags)pCreateInfo->flags;
	commandPoolCreateInfo.queueFamilyIndex = pCreateInfo->queueFamilyIndex;

	return GetError(vkCreateCommandPool(
		(VkDevice)device, &commandPoolCreateInfo, NULL, (VkCommandPool*)pCommandPool));
}

void RrDestroyCommandPool(RrLogicalDevice pDevice, RrCommandPool pCommandPool)
{
	vkDestroyCommandPool((VkDevice)pDevice, (VkCommandPool)pCommandPool, NULL);
}
