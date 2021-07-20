#pragma once

#include "RCore.h"
#include "RPhysicalDevice.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrLogicalDevice);
RR_DEFINE_HANDLE(RrQueue);

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

RR_API void RrGetDeviceQueue(
	RrLogicalDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, RrQueue* pQueue);

RR_API void RrDestroyLogicalDevice(RrLogicalDevice device);

RR_API RrPFNVoidFunction RrGetDeviceProcAddr(RrLogicalDevice device, const char* pName);

RR_API RrError RrDeviceWaitIdle(RrLogicalDevice device);

RR_EXTERN_C_END
