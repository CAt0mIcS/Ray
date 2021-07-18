#include "Rpch.h"
#include <../Core/RImageView.h>
#include <../Core/RUtils.h>

RrError RrCreateImageView(
	RrLogicalDevice device, const RrImageViewCreateInfo* pCreateInfo, RrImageView* pImageView)
{
	VkImageViewCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.flags = 0;
	createInfo.pNext = NULL;
	createInfo.image = (VkImage)pCreateInfo->image;
	createInfo.viewType = pCreateInfo->viewType;
	createInfo.format = pCreateInfo->format;
	createInfo.components.r = pCreateInfo->components.r;
	createInfo.components.g = pCreateInfo->components.g;
	createInfo.components.b = pCreateInfo->components.b;
	createInfo.components.a = pCreateInfo->components.a;
	createInfo.subresourceRange.aspectMask = pCreateInfo->subresourceRange.aspectMask;
	createInfo.subresourceRange.baseMipLevel = pCreateInfo->subresourceRange.baseMipLevel;
	createInfo.subresourceRange.levelCount = pCreateInfo->subresourceRange.levelCount;
	createInfo.subresourceRange.baseArrayLayer = pCreateInfo->subresourceRange.baseArrayLayer;
	createInfo.subresourceRange.layerCount = pCreateInfo->subresourceRange.layerCount;

	return GetError(
		vkCreateImageView((VkDevice)device, &createInfo, NULL, (VkImageView*)pImageView));
}

void RrDestroyImageView(RrLogicalDevice device, RrImageView imageView)
{
	vkDestroyImageView((VkDevice)device, (VkImageView)imageView, NULL);
}
