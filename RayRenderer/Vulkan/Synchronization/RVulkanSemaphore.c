#include "Rpch.h"
#include <../Synchronization/RSemaphore.h>
#include <../Core/RUtils.h>


RrError RrCreateSemaphore(RrLogicalDevice device, RrSemaphore* pSemaphore)
{
	VkSemaphoreCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = 0;
	return GetError(
		vkCreateSemaphore((VkDevice)device, &createInfo, NULL, (VkSemaphore*)pSemaphore));
}

void RrDestroySemaphore(RrLogicalDevice device, RrSemaphore semaphore)
{
	vkDestroySemaphore((VkDevice)device, (VkSemaphore)semaphore, NULL);
}
