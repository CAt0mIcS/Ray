#pragma once

#include "RCore.h"
#include "RInstance.h"

RR_EXTERN_C_BEG

typedef void* RrPhysicalDevice;

typedef struct RrPhysicalDeviceEnumerationInfo
{
	uint32_t deviceExtensionCount;
	const char* const* ppDeviceExtensions;
} RrPhysicalDeviceEnumerationInfo;

RR_API RrError RrEnumeratePhysicalDevice(RrInstance pInstance,
	const RrPhysicalDeviceEnumerationInfo* const pCreateInfo, RrPhysicalDevice* ppPhysicalDevice);
typedef RrError (*RrPFNEnumeratePhysicalDevice)(
	RrInstance, const RrPhysicalDeviceEnumerationInfo* const, RrPhysicalDevice*);

RR_EXTERN_C_END
