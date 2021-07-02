#include "Rpch.h"
#include <../Core/RImage.h>
#include <../Core/RUtils.h>


RrError RrCreateImage(RrLogicalDevice device, const RrImageCreateInfo* pCreateInfo, RrImage* pImage)
{
	VkImageCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = pCreateInfo->flags;
	createInfo.imageType = pCreateInfo->imageType;
	createInfo.format = pCreateInfo->format;
	createInfo.extent.width = pCreateInfo->extent.width;
	createInfo.extent.height = pCreateInfo->extent.height;
	createInfo.extent.depth = pCreateInfo->extent.depth;
	createInfo.mipLevels = pCreateInfo->mipLevels;
	createInfo.arrayLayers = pCreateInfo->arrayLayers;
	createInfo.samples = pCreateInfo->samples;
	createInfo.tiling = pCreateInfo->tiling;
	createInfo.usage = pCreateInfo->usage;
	createInfo.sharingMode = pCreateInfo->sharingMode;
	createInfo.queueFamilyIndexCount = pCreateInfo->queueFamilyIndexCount;
	createInfo.pQueueFamilyIndices = pCreateInfo->pQueueFamilyIndices;
	createInfo.initialLayout = pCreateInfo->initialLayout;

	return GetError(vkCreateImage((VkDevice)device, &createInfo, NULL, (VkImage*)pImage));
}

void RrImageGetMemoryRequirements(
	RrLogicalDevice device, RrImage image, RrMemoryRequirements* pMemRequirements)
{
	VkMemoryRequirements memRequirememts;
	vkGetImageMemoryRequirements((VkDevice)device, (VkImage)image, &memRequirememts);

	pMemRequirements->size = memRequirememts.size;
	pMemRequirements->alignment = memRequirememts.alignment;
	pMemRequirements->memoryTypeBits = memRequirememts.memoryTypeBits;
}

RrError RrBindImageMemory(
	RrLogicalDevice device, RrImage image, RrDeviceMemory memory, RrDeviceSize memoryOffset)
{
	return GetError(
		vkBindImageMemory((VkDevice)device, (VkImage)image, (VkDeviceMemory)memory, memoryOffset));
}

void RrGetImageSubresourceLayout(RrLogicalDevice device, RrImage image,
	RrImageSubresource* pSubresource, RrSubresourceLayout* pLayout)
{
	VkImageSubresource subresource;
	VkSubresourceLayout layout;
	vkGetImageSubresourceLayout((VkDevice)device, (VkImage)image, &subresource, &layout);

	pLayout->size = layout.size;
	pLayout->rowPitch = layout.rowPitch;
	pLayout->offset = layout.offset;
	pLayout->depthPitch = layout.depthPitch;
	pLayout->arrayPitch = layout.arrayPitch;
	pSubresource->arrayLayer = subresource.arrayLayer;
	pSubresource->aspectMask = subresource.aspectMask;
	pSubresource->mipLevel = subresource.mipLevel;
}

void RrDestroyImage(RrLogicalDevice device, RrImage image)
{
	vkDestroyImage((VkDevice)device, (VkImage)image, NULL);
}
