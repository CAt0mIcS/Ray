#include "Rpch.h"
#include <../Synchronization/RFence.h>
#include <../Core/RUtils.h>


RrError RrCreateFence(RrLogicalDevice device, const RrFenceCreateInfo* pCreateInfo, RrFence* pFence)
{
	VkFenceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = (VkFenceCreateFlags)pCreateInfo->flags;

	return GetError(vkCreateFence((VkDevice)device, &createInfo, NULL, (VkFence*)pFence));
}

RrError RrWaitForFences(RrLogicalDevice device, uint32_t fenceCount, const RrFence* pFences,
	RrBool32 waitAll, uint64_t timeout)
{
	return GetError(
		vkWaitForFences((VkDevice)device, fenceCount, (const VkFence*)pFences, waitAll, timeout));
}

RrError RrResetFences(RrLogicalDevice device, uint32_t fenceCount, const RrFence* pFences)
{
	return GetError(vkResetFences((VkDevice)device, fenceCount, (const VkFence*)pFences));
}

void RrDestroyFence(RrLogicalDevice device, RrFence fence)
{
	vkDestroyFence((VkDevice)device, (VkFence)fence, NULL);
}
