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

	VkResult error = vkCreateCommandPool(pDevice, &commandPoolCreateInfo, NULL, ppCommandPool);
	if (error != VK_SUCCESS)
		if (LogError("Failed to create command pool"))
			return RrErrorInitializationFailed;

	return RrErrorNone;
}

void RrDestroyCommandPool(RrLogicalDevice pDevice, RrCommandPool pCommandPool)
{
	vkDestroyCommandPool(pDevice, pCommandPool, NULL);
}
