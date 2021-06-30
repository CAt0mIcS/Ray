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
	RrInitializeInfo* const pInitInfo, RrInstance* pInstance, RrDebugMessenger* pDebugMessenger);
typedef RrError (*RrPFNInitialize)(RrInitializeInfo* const, RrInstance*, RrDebugMessenger*);

RR_API void RrDestroyInstance(RrInstance pInstance, RrDebugMessenger pDebugMessenger);
typedef RrError (*RrPFNDestroyInstance)(RrInstance, RrDebugMessenger);

RR_EXTERN_C_END
