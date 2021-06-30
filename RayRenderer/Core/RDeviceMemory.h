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
typedef RrError (*RrPFNAllocateMemory)(
	RrLogicalDevice, const RrMemoryAllocateInfo* const, RrDeviceMemory*);

RR_API RrError RrDeviceMemoryGetMemoryTypeIndex(uint32_t memoryTypeBits,
	RrMemoryPropertyFlags memoryProperties, uint32_t memoryTypeCount,
	const RrMemoryType* const pMemoryTypes, uint32_t* pTypeIndex);
typedef RrError (*RrPFNDeviceMemoryGetMemoryTypeIndex)(
	uint32_t, RrMemoryPropertyFlags, uint32_t, const RrMemoryType* const, uint32_t*);

RR_API RrError RrMapMemory(RrLogicalDevice pDevice, RrDeviceMemory pMemory, RrDeviceSize offset,
	RrDeviceSize size, void** mapped);
typedef RrError (*RrPFNMapMemory)(
	RrLogicalDevice, RrDeviceMemory, RrDeviceSize, RrDeviceSize, void**);

RR_API void RrUnmapMemory(RrLogicalDevice pDevice, RrDeviceMemory pMemory);
typedef void (*RrPFNUnmapMemory)(RrLogicalDevice, RrDeviceMemory);

RR_API RrError RrFlushMappedMemoryRanges(
	RrLogicalDevice pDevice, uint32_t memoryRangeCount, RrMappedMemoryRange* pMappedMemoryRanges);
typedef RrError (*RrPFNFlushMappedMemoryRanges)(RrLogicalDevice, uint32_t, RrMappedMemoryRange*);

RR_EXTERN_C_END
