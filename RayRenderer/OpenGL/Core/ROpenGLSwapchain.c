#include "Rpch.h"

#include "../Core/RSwapchain.h"
#include "../Core/RUtils.h"


RrError RrCreateSwapchainKHR(
	RrLogicalDevice device, const RrSwapchainCreateInfoKHR* pCreateInfo, RrSwapchainKHR* pSwapchain)
{
	return RrErrorSuccess;
}

RrError RrGetSwapchainImagesKHR(RrLogicalDevice device, RrSwapchainKHR swapchain,
	uint32_t* pSwapchainImageCount, RrImage* pSwapchainImages)
{
	return RrErrorSuccess;
}

void RrDestroySwapchainKHR(RrLogicalDevice device, RrSwapchainKHR swapchain) {}

RrError RrAcquireNextImageKHR(RrLogicalDevice device, RrSwapchainKHR swapchain, uint64_t timeout,
	RrSemaphore semaphore, RrFence fence, uint32_t* pImageIndex)
{
	return RrErrorSuccess;
}

RrError RrQueuePresentKHR(RrQueue queue, const RrPresentInfoKHR* pPresentInfo)
{
	return RrErrorSuccess;
}