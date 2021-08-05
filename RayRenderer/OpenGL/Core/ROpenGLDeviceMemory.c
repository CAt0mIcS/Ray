#include "Rpch.h"

#include "../Core/RDeviceMemory.h"
#include "../Core/RUtils.h"


RrError RrAllocateMemory(
	RrLogicalDevice device, const RrMemoryAllocateInfo* pAllocateInfo, RrDeviceMemory* pMemory)
{
	return RrErrorSuccess;
}

RrError RrDeviceMemoryGetMemoryTypeIndex(uint32_t memoryTypeBits,
	RrMemoryPropertyFlags memoryProperties, uint32_t memoryTypeCount,
	const RrMemoryType* pMemoryTypes, uint32_t* pTypeIndex)
{
	return RrErrorSuccess;
}

RrError RrMapMemory(RrLogicalDevice device, RrDeviceMemory memory, RrDeviceSize offset,
	RrDeviceSize size, void** ppMapped)
{
	return RrErrorSuccess;
}

void RrUnmapMemory(RrLogicalDevice device, RrDeviceMemory memory) {}

RrError RrFlushMappedMemoryRanges(RrLogicalDevice device, uint32_t memoryRangeCount,
	const RrMappedMemoryRange* pMappedMemoryRanges)
{
	return RrErrorSuccess;
}

void RrFreeMemory(RrLogicalDevice device, RrDeviceMemory memory) {}