#pragma once

#include "RCore.h"
#include "RLogicalDevice.h"
#include "RPhysicalDevice.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrDeviceMemory);

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


RR_API RrError RrAllocateMemory(RrLogicalDevice pDevice,
	const RrMemoryAllocateInfo* const pAllocateInfo, RrDeviceMemory* ppMemory);

RR_API RrError RrDeviceMemoryGetMemoryTypeIndex(uint32_t memoryTypeBits,
	RrMemoryPropertyFlags memoryProperties, uint32_t memoryTypeCount,
	const RrMemoryType* const pMemoryTypes, uint32_t* pTypeIndex);

RR_API RrError RrMapMemory(RrLogicalDevice pDevice, RrDeviceMemory pMemory, RrDeviceSize offset,
	RrDeviceSize size, void** mapped);

RR_API void RrUnmapMemory(RrLogicalDevice pDevice, RrDeviceMemory pMemory);

RR_API RrError RrFlushMappedMemoryRanges(
	RrLogicalDevice pDevice, uint32_t memoryRangeCount, RrMappedMemoryRange* pMappedMemoryRanges);

RR_API void RrFreeMemory(RrLogicalDevice device, RrDeviceMemory memory);

RR_EXTERN_C_END
