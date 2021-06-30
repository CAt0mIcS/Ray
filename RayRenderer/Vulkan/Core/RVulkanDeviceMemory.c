#include "Rpch.h"
#include "../Core/RDeviceMemory.h"
#include "../Core/RUtils.h"


RrError RrAllocateMemory(RrLogicalDevice pDevice, const RrMemoryAllocateInfo* const pAllocateInfo,
	RrDeviceMemory* ppMemory)
{
	VkMemoryAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.pNext = NULL;
	allocInfo.allocationSize = pAllocateInfo->allocationSize;
	allocInfo.memoryTypeIndex = pAllocateInfo->memoryTypeIndex;

	VkResult error =
		vkAllocateMemory((VkDevice)pDevice, &allocInfo, NULL, (VkDeviceMemory*)ppMemory);
	if (error != VK_SUCCESS)
		return GetError(error);

	return RrErrorNone;
}

RrError RrDeviceMemoryGetMemoryTypeIndex(uint32_t memoryTypeBits,
	RrMemoryPropertyFlags memoryProperties, uint32_t memoryTypeCount,
	const RrMemoryType* const pMemoryTypes, uint32_t* pTypeIndex)
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

RrError RrMapMemory(RrLogicalDevice pDevice, RrDeviceMemory pMemory, RrDeviceSize offset,
	RrDeviceSize size, void** mapped)
{
	return GetError(
		vkMapMemory((VkDevice)pDevice, (VkDeviceMemory)pMemory, offset, size, 0, mapped));
}

void RrUnmapMemory(RrLogicalDevice pDevice, RrDeviceMemory pMemory)
{
	vkUnmapMemory((VkDevice)pDevice, (VkDeviceMemory)pMemory);
}

RrError RrFlushMappedMemoryRanges(
	RrLogicalDevice pDevice, uint32_t memoryRangeCount, RrMappedMemoryRange* pMappedMemoryRanges)
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
		GetError(vkFlushMappedMemoryRanges((VkDevice)pDevice, memoryRangeCount, pRanges));
	free(pRanges);
	return error;
}
