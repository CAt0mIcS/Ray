#include "Rpch.h"

#include "../Core/RCommandPool.h"
#include "../Core/RUtils.h"


RrError RrCreateCommandPool(RrLogicalDevice pDevice,
	const RrCommandPoolCreateInfo* const pCreateInfo, RrCommandPool* ppCommandPool)
{
	VkCommandPoolCreateInfo commandPoolCreateInfo;
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.pNext = NULL;
	commandPoolCreateInfo.flags = (VkCommandPoolCreateFlags)pCreateInfo->flags;
	commandPoolCreateInfo.queueFamilyIndex = pCreateInfo->queueFamilyIndex;

	VkResult error =
		vkCreateCommandPool(pDevice, &commandPoolCreateInfo, NULL, (VkCommandPool*)ppCommandPool);
	if (error != VK_SUCCESS)
		return GetError(error);

	return RrErrorNone;
}

void RrDestroyCommandPool(RrLogicalDevice pDevice, RrCommandPool pCommandPool)
{
	vkDestroyCommandPool(pDevice, pCommandPool, NULL);
}
