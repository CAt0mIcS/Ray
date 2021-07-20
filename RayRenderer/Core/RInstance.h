#pragma once

#include "RCore.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrInstance);
RR_DEFINE_HANDLE(RrDebugMessenger);

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
} RrInitializeInfo;

RR_API RrError RrInitialize(
	RrInitializeInfo* pInitInfo, RrInstance* pInstance, RrDebugMessenger* pDebugMessenger);

RR_API void RrDestroyInstance(RrInstance instance, RrDebugMessenger debugMessenger);

RR_API RrPFNVoidFunction RrGetInstanceProcAddr(RrInstance instance, const char* pName);

RR_API RrError RrEnumerateInstanceExtensionProperties(
	const char* pLayerName, uint32_t* pPropertyCount, RrExtensionProperties* pProperties);

RR_EXTERN_C_END
