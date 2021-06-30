#include "Rpch.h"
#include "../Core/RDeviceMemory.h"
#include "../Core/RUtils.h"


RrError RrAllocateMemory(
	RrLogicalDevice device, const RrMemoryAllocateInfo* pAllocateInfo, RrDeviceMemory* pMemory)
{
	VkMemoryAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.pNext = NULL;
	allocInfo.allocationSize = pAllocateInfo->allocationSize;
	allocInfo.memoryTypeIndex = pAllocateInfo->memoryTypeIndex;

	VkResult error = vkAllocateMemory((VkDevice)device, &allocInfo, NULL, (VkDeviceMemory*)pMemory);
	if (error != VK_SUCCESS)
		return GetError(error);

	return RrErrorNone;
}

RrError RrDeviceMemoryGetMemoryTypeIndex(uint32_t memoryTypeBits,
	RrMemoryPropertyFlags memoryProperties, uint32_t memoryTypeCount,
	const RrMemoryType* pMemoryTypes, uint32_t* pTypeIndex)
{
	for (uint32_t i = 0; i < memoryTypeCount; ++i)
	{
		if (memoryTypeBits & (1 << i) &&
			(pMemoryTypes[i].propertyFlags & memoryProperties) == memoryProperties)
		{
			*pTypeIndex = i;
			return RrErrorNone;
		}
	}

	LogError("Failed to find suitable memory type");
	return RrErrorIncomplete;
}

RrError RrMapMemory(RrLogicalDevice device, RrDeviceMemory memory, RrDeviceSize offset,
	RrDeviceSize size, void** ppMapped)
{
	return GetError(
		vkMapMemory((VkDevice)device, (VkDeviceMemory)memory, offset, size, 0, ppMapped));
}

void RrUnmapMemory(RrLogicalDevice device, RrDeviceMemory memory)
{
	vkUnmapMemory((VkDevice)device, (VkDeviceMemory)memory);
}

RrError RrFlushMappedMemoryRanges(RrLogicalDevice device, uint32_t memoryRangeCount,
	const RrMappedMemoryRange* pMappedMemoryRanges)
{
	VkMappedMemoryRange* pRanges = malloc(sizeof(VkMappedMemoryRange) * memoryRangeCount);
	for (uint32_t i = 0; i < memoryRangeCount; ++i)
	{
		pRanges[i].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		pRanges[i].pNext = NULL;
		pRanges[i].size = pMappedMemoryRanges[i].size;
		pRanges[i].offset = pMappedMemoryRanges[i].offset;
		pRanges[i].memory = (VkDeviceMemory)pMappedMemoryRanges[i].memory;
	}

	RrError error =
		GetError(vkFlushMappedMemoryRanges((VkDevice)device, memoryRangeCount, pRanges));
	free(pRanges);
	return error;
}

void RrFreeMemory(RrLogicalDevice device, RrDeviceMemory memory)
{
	vkFreeMemory((VkDevice)device, (VkDeviceMemory)memory, NULL);
}
