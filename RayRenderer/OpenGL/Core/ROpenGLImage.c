#include "Rpch.h"

#include "../Core/RImage.h"
#include "../Core/RUtils.h"


RrError RrCreateImage(RrLogicalDevice device, const RrImageCreateInfo* pCreateInfo, RrImage* pImage)
{
	return RrErrorSuccess;
}

void RrImageGetMemoryRequirements(
	RrLogicalDevice device, RrImage image, RrMemoryRequirements* pMemRequirements)
{
}

RrError RrBindImageMemory(
	RrLogicalDevice device, RrImage image, RrDeviceMemory memory, RrDeviceSize memoryOffset)
{
	return RrErrorSuccess;
}

void RrGetImageSubresourceLayout(RrLogicalDevice device, RrImage image,
	RrImageSubresource* pSubresource, RrSubresourceLayout* pLayout)
{
}

void RrDestroyImage(RrLogicalDevice device, RrImage image) {}


void RrCmdBlitImage(RrCommandBuffer commandBuffer, RrImage srcImage, RrImageLayout srcImageLayout,
	RrImage dstImage, RrImageLayout dstImageLayout, uint32_t regionCount,
	const RrImageBlit* pRegions, RrFilter filter)
{
}

void RrCmdCopyBufferToImage(RrCommandBuffer commandBuffer, RrBuffer srcBuffer, RrImage dstImage,
	RrImageLayout dstImageLayout, uint32_t regionCount, const RrBufferImageCopy* pRegions)
{
}