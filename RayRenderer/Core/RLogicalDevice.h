#pragma once

#include "RCore.h"
#include "RPhysicalDevice.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrLogicalDevice);
RR_DEFINE_HANDLE(RrQueue);

typedef enum RrQueueFlagBits
{
	RrQueueGraphics = 0x00000001,
	RrQueueCompute = 0x00000002,
	RrQueueTransfer = 0x00000004,
	RrQueueSparseBinding = 0x00000008,
	RrQueueProtected = 0x00000010,
} RrQueueFlagBits;
typedef uint32_t RrQueueFlags;

typedef struct RrDeviceQueueCreateInfo
{
	uint32_t queueFamilyIndex;
	uint32_t queueCount;
	const float* pQueuePriorities;
} RrDeviceQueueCreateInfo;

typedef struct RrLogicalDeviceCreateInfo
{
	uint32_t queueCreateInfoCount;
	const RrDeviceQueueCreateInfo* pQueueCreateInfos;
	uint32_t enabledLayerCount;
	const char* const* ppEnabledLayerNames;
	uint32_t enabledExtensionCount;
	const char* const* ppEnabledExtensionNames;
	const RrPhysicalDeviceFeatures* pEnabledFeatures;
} RrLogicalDeviceCreateInfo;

RR_API RrError RrCreateLogicalDevice(RrPhysicalDevice physicalDevice,
	RrLogicalDeviceCreateInfo* pCreateInfo, RrLogicalDevice* pDevice);
typedef RrError (*RrPFNCreateLogicalDevice)(
	RrPhysicalDevice, RrLogicalDeviceCreateInfo*, RrLogicalDevice*);

RR_API void RrGetDeviceQueue(
	RrLogicalDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, RrQueue* pQueue);
typedef void (*RrPFNGetDeviceQueue)(RrLogicalDevice, uint32_t, uint32_t, RrQueue*);

RR_API void RrDestroyLogicalDevice(RrLogicalDevice device);
typedef void (*RrPFNDestroyLogicalDevice)(RrLogicalDevice);

RR_API RrPFNVoidFunction RrGetDeviceProcAddr(RrLogicalDevice device, const char* pName);
typedef RrPFNVoidFunction (*RrPFNGetDeviceProcAddr)(RrLogicalDevice, const char*);

RR_API RrError RrDeviceWaitIdle(RrLogicalDevice device);
typedef RrError (*RrPFNDeviceWaitIdle)(RrLogicalDevice);

RR_EXTERN_C_END
