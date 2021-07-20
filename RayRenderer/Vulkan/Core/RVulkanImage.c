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


void RrCmdBlitImage(RrCommandBuffer commandBuffer, RrImage srcImage, RrImageLayout srcImageLayout,
	RrImage dstImage, RrImageLayout dstImageLayout, uint32_t regionCount,
	const RrImageBlit* pRegions, RrFilter filter)
{
	VkImageBlit* regions = malloc(sizeof(VkImageBlit) * regionCount);
	for (uint32_t i = 0; i < regionCount; ++i)
	{
		regions[i].dstOffsets[0].x = pRegions[i].dstOffsets[0].x;
		regions[i].dstOffsets[0].y = pRegions[i].dstOffsets[0].y;
		regions[i].dstOffsets[0].z = pRegions[i].dstOffsets[0].z;
		regions[i].dstOffsets[1].x = pRegions[i].dstOffsets[1].x;
		regions[i].dstOffsets[1].y = pRegions[i].dstOffsets[1].y;
		regions[i].dstOffsets[1].z = pRegions[i].dstOffsets[1].z;

		regions[i].srcOffsets[0].x = pRegions[i].srcOffsets[0].x;
		regions[i].srcOffsets[0].y = pRegions[i].srcOffsets[0].y;
		regions[i].srcOffsets[0].z = pRegions[i].srcOffsets[0].z;
		regions[i].srcOffsets[1].x = pRegions[i].srcOffsets[1].x;
		regions[i].srcOffsets[1].y = pRegions[i].srcOffsets[1].y;
		regions[i].srcOffsets[1].z = pRegions[i].srcOffsets[1].z;

		regions[i].dstSubresource.aspectMask = pRegions[i].dstSubresource.aspectMask;
		regions[i].dstSubresource.baseArrayLayer = pRegions[i].dstSubresource.baseArrayLayer;
		regions[i].dstSubresource.layerCount = pRegions[i].dstSubresource.layerCount;
		regions[i].dstSubresource.mipLevel = pRegions[i].dstSubresource.mipLevel;

		regions[i].srcSubresource.aspectMask = pRegions[i].srcSubresource.aspectMask;
		regions[i].srcSubresource.baseArrayLayer = pRegions[i].srcSubresource.baseArrayLayer;
		regions[i].srcSubresource.layerCount = pRegions[i].srcSubresource.layerCount;
		regions[i].srcSubresource.mipLevel = pRegions[i].srcSubresource.mipLevel;
	}

	vkCmdBlitImage((VkCommandBuffer)commandBuffer, (VkImage)srcImage, srcImageLayout,
		(VkImage)dstImage, dstImageLayout, regionCount, regions, filter);

	free(regions);
}

void RrCmdCopyBufferToImage(RrCommandBuffer commandBuffer, RrBuffer srcBuffer, RrImage dstImage,
	RrImageLayout dstImageLayout, uint32_t regionCount, const RrBufferImageCopy* pRegions)
{
	VkBufferImageCopy* regions = malloc(sizeof(VkBufferImageCopy) * regionCount);
	for (uint32_t i = 0; i < regionCount; ++i)
	{
		regions[i].bufferOffset = pRegions[i].bufferOffset;
		regions[i].bufferRowLength = pRegions[i].bufferRowLength;
		regions[i].bufferImageHeight = pRegions[i].bufferImageHeight;
		regions[i].imageSubresource.aspectMask = pRegions[i].imageSubresource.aspectMask;
		regions[i].imageSubresource.baseArrayLayer = pRegions[i].imageSubresource.baseArrayLayer;
		regions[i].imageSubresource.layerCount = pRegions[i].imageSubresource.layerCount;
		regions[i].imageSubresource.mipLevel = pRegions[i].imageSubresource.mipLevel;
		regions[i].imageOffset.x = pRegions[i].imageOffset.x;
		regions[i].imageOffset.y = pRegions[i].imageOffset.y;
		regions[i].imageOffset.z = pRegions[i].imageOffset.z;
		regions[i].imageExtent.width = pRegions[i].imageExtent.width;
		regions[i].imageExtent.height = pRegions[i].imageExtent.height;
		regions[i].imageExtent.depth = pRegions[i].imageExtent.depth;
	}

	vkCmdCopyBufferToImage((VkCommandBuffer)commandBuffer, (VkBuffer)srcBuffer, (VkImage)dstImage,
		dstImageLayout, regionCount, regions);

	free(regions);
}
