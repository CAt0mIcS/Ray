#pragma once

#include "RCore.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrInstance);
RR_DEFINE_HANDLE(RrDebugMessenger);

#define RR_MAKE_VERSION(major, minor, patch) \
	((((uint32_t)(major)) << 22) | (((uint32_t)(minor)) << 12) | ((uint32_t)(patch)))

// Patch version should always be set to 0
#define RR_API_VERSION_1_0 RR_MAKE_VERSION(1, 0, 0)
#define RR_API_VERSION_1_1 RR_MAKE_VERSION(1, 1, 0)
#define RR_API_VERSION_1_2 RR_MAKE_VERSION(1, 2, 0)

#define RR_GET_VERSION_MAJOR(version) \
	((((uint32_t)version) & 0b11111111110000000000000000000000) >> 22)
#define RR_GET_VERSION_MINOR(version) \
	((((uint32_t)version) & 0b00000000001111111111000000000000) >> 12)
#define RR_GET_VERSION_PATCH(version) (((uint32_t)version) & 0b00000000000000000000111111111111)


typedef struct RrInitializeInfo
{
	void* pfnLoader;
	const void* pNext;
	bool (*pfnValidationCallback)(RrLogMessageSeverity, const char*);

	uint32_t enabledExtensionCount;
	const char* const* ppEnabledExtensions;

	uint32_t enabledLayerCount;
	const char* const* ppEnabledLayers;

	bool enableValidationLayers;
	uint32_t apiVersion;
} RrInitializeInfo;

RR_API RrError RrInitialize(
	RrInitializeInfo* pInitInfo, RrInstance* pInstance, RrDebugMessenger* pDebugMessenger);

RR_API void RrDestroyInstance(RrInstance instance, RrDebugMessenger debugMessenger);

RR_API RrPFNVoidFunction RrGetInstanceProcAddr(RrInstance instance, const char* pName);

RR_API RrError RrEnumerateInstanceExtensionProperties(
	const char* pLayerName, uint32_t* pPropertyCount, RrExtensionProperties* pProperties);

RR_EXTERN_C_END
