#pragma once

#include "RCore.h"
#include "RLogicalDevice.h"

RR_EXTERN_C_BEG

typedef void* RrDeviceMemory;

typedef enum RrMemoryPropertyFlagBits
{
	RrMemoryPropertyDeviceLocal = 0x00000001,
	RrMemoryPropertyHostVisible = 0x00000002,
	RrMemoryPropertyHostCoherent = 0x00000004,
	RrMemoryPropertyHostCached = 0x00000008,
	RrMemoryPropertyLazilyAllocated = 0x00000010,
	RrMemoryPropertyProtected = 0x00000020,
	RrMemoryPropertyDeviceCoherentAMD = 0x00000040,
	RrMemoryPropertyDeviceUncachedAMD = 0x00000080,
} RrMemoryPropertyFlagBits;
typedef uint32_t RrMemoryPropertyFlags;

typedef struct RrMemoryType
{
	RrMemoryPropertyFlags propertyFlags;
	uint32_t heapIndex;
} RrMemoryType;

typedef enum RrMemoryHeapFlagBits
{
	RrMemoryHeapDeviceLocal = 0x00000001,
	RrMemoryHeapMultiInstance = 0x00000002,
	RrMemoryHeapMultiInstanceKHR = RrMemoryHeapMultiInstance
} RrMemoryHeapFlagBits;
typedef uint32_t RrMemoryHeapFlags;

typedef struct RrMemoryHeap
{
	RrDeviceSize size;
	RrMemoryHeapFlags flags;
} RrMemoryHeap;

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
