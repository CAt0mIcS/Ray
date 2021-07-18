#include "Rpch.h"
#include <../Core/RSwapchain.h>
#include <../Core/RUtils.h>


RrError RrCreateSwapchainKHR(
	RrLogicalDevice device, const RrSwapchainCreateInfoKHR* pCreateInfo, RrSwapchainKHR* pSwapchain)
{
	VkSwapchainCreateInfoKHR createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.pNext = NULL;
	createInfo.flags = pCreateInfo->flags;
	createInfo.surface = (VkSurfaceKHR)pCreateInfo->surface;
	createInfo.minImageCount = pCreateInfo->minImageCount;
	createInfo.imageFormat = pCreateInfo->imageFormat;
	createInfo.imageColorSpace = pCreateInfo->imageColorSpace;
	createInfo.imageExtent.width = pCreateInfo->imageExtent.width;
	createInfo.imageExtent.height = pCreateInfo->imageExtent.height;
	createInfo.imageArrayLayers = pCreateInfo->imageArrayLayers;
	createInfo.imageUsage = pCreateInfo->imageUsage;
	createInfo.imageSharingMode = pCreateInfo->imageSharingMode;
	createInfo.queueFamilyIndexCount = pCreateInfo->queueFamilyIndexCount;
	createInfo.pQueueFamilyIndices = pCreateInfo->pQueueFamilyIndices;
	createInfo.preTransform = pCreateInfo->preTransform;
	createInfo.compositeAlpha = pCreateInfo->compositeAlpha;
	createInfo.presentMode = pCreateInfo->presentMode;
	createInfo.clipped = pCreateInfo->clipped;
	createInfo.oldSwapchain = (VkSwapchainKHR)pCreateInfo->oldSwapchain;

	return GetError(
		vkCreateSwapchainKHR((VkDevice)device, &createInfo, NULL, (VkSwapchainKHR*)pSwapchain));
}

RrError RrGetSwapchainImagesKHR(RrLogicalDevice device, RrSwapchainKHR swapchain,
	uint32_t* pSwapchainImageCount, RrImage* pSwapchainImages)
{
	return GetError(vkGetSwapchainImagesKHR((VkDevice)device, (VkSwapchainKHR)swapchain,
		pSwapchainImageCount, (VkImage*)pSwapchainImages));
}

void RrDestroySwapchainKHR(RrLogicalDevice device, RrSwapchainKHR swapchain)
{
	vkDestroySwapchainKHR((VkDevice)device, (VkSwapchainKHR)swapchain, NULL);
}

RrError RrAcquireNextImageKHR(RrLogicalDevice device, RrSwapchainKHR swapchain, uint64_t timeout,
	RrSemaphore semaphore, RrFence fence, uint32_t* pImageIndex)
{
	return GetError(vkAcquireNextImageKHR((VkDevice)device, (VkSwapchainKHR)swapchain, timeout,
		(VkSemaphore)semaphore, (VkFence)fence, pImageIndex));
}

RrError RrQueuePresentKHR(RrQueue queue, const RrPresentInfoKHR* pPresentInfo)
{
	VkPresentInfoKHR presentInfo;
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = NULL;
	presentInfo.waitSemaphoreCount = pPresentInfo->waitSemaphoreCount;
	presentInfo.pWaitSemaphores = (const VkSemaphore*)pPresentInfo->pWaitSemaphores;
	presentInfo.swapchainCount = pPresentInfo->swapchainCount;
	presentInfo.pSwapchains = (const VkSwapchainKHR*)pPresentInfo->pSwapchains;
	presentInfo.pImageIndices = pPresentInfo->pImageIndices;
	presentInfo.pResults = (VkResult*)pPresentInfo->pResults;
	return GetError(vkQueuePresentKHR((VkQueue)queue, &presentInfo));
}
