#pragma once

#include "RCore.h"
#include "RPhysicalDevice.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrDeviceMemory);
RR_DEFINE_HANDLE(RrLogicalDevice);

typedef struct RrMappedMemoryRange
{
	RrDeviceMemory memory;
	RrDeviceSize offset;
	RrDeviceSize size;
} RrMappedMemoryRange;

typedef struct RrMemoryAllocateInfo
{
	RrDeviceSize allocationSize;
	uint32_t memoryTypeIndex;
} RrMemoryAllocateInfo;


RR_API RrError RrAllocateMemory(
	RrLogicalDevice device, const RrMemoryAllocateInfo* pAllocateInfo, RrDeviceMemory* pMemory);

RR_API RrError RrDeviceMemoryGetMemoryTypeIndex(uint32_t memoryTypeBits,
	RrMemoryPropertyFlags memoryProperties, uint32_t memoryTypeCount,
	const RrMemoryType* pMemoryTypes, uint32_t* pTypeIndex);

RR_API RrError RrMapMemory(RrLogicalDevice device, RrDeviceMemory memory, RrDeviceSize offset,
	RrDeviceSize size, void** ppMapped);

RR_API void RrUnmapMemory(RrLogicalDevice device, RrDeviceMemory memory);

RR_API RrError RrFlushMappedMemoryRanges(RrLogicalDevice device, uint32_t memoryRangeCount,
	const RrMappedMemoryRange* pMappedMemoryRanges);

RR_API void RrFreeMemory(RrLogicalDevice device, RrDeviceMemory memory);

RR_EXTERN_C_END
