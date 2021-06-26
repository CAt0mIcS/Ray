#pragma once

#include "../RBase.h"

RR_EXTERN_C_BEG

typedef void* RrInstance;
typedef void* RrDebugMessenger;

typedef enum RrError
{
	RrErrorNone = 0,
	RrErrorUnknown,
	RrErrorNotReady,
	RrErrorTimeout,
	RrErrorEventSet,
	RrErrorEventReset,
	RrErrorIncomplete,
	RrErrorOutOfHostMemory,
	RrErrorOutOfDeviceMemory,
	RrErrorInitializationFailed,
	RrErrorDeviceLost,
	RrErrorMemoryMapFailed,
	RrErrorLayerNotPresent,
	RrErrorExtensionNotPresent,
	RrErrorFeatureNotPresent,
	RrErrorIncompatibleDriver,
	RrErrorTooManyObjects,
	RrErrorFormatNotSupported,
	RrErrorFragmentedPool,
	RrErrorOutofPoolMemory,
	RrErrorInvalidExternalHandle,
	RrErrorFragmentation,
	RrErrorInvalidOpaqueCaptureAddress,
	RrErrorSurfaceLost,
	RrErrorNativeWindowInUse,
	RrErrorSuboptimal,
	RrErrorOutOfDate,
	RrErrorIncompatibleDisplay,
	RrErrorValidationFailed,
	RrErrorInvalidShader,
	RrErrorInvalidDRMFormatModifierPlaneLayout,
	RrErrorNotPermitted,
	RrErrorFullScreenExclusiveModeLost,
	RrErrorThreadIdle,
	RrErrorThreadDone,
	RrErrorOperationDeferred,
	RrErrorOperationNotDeferred,
	RrErrorPipelineCompileRequired,

	RrErrorLoaderFailed
} RrError;

typedef enum RrLogMessageSeverity
{
	RrLogMessageSeverityDebug,
	RrLogMessageSeverityInfo,
	RrLogMessageSeverityWarning,
	RrLogMessageSeverityError
} RrLogMessageSeverity;


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


extern bool (*RrpfnValidationCallback)(RrLogMessageSeverity, const char*);


RR_API RrError RrInitialize(
	RrInitializeInfo* const pInitInfo, RrInstance* pInstance, RrDebugMessenger* pDebugMessenger);

RR_EXTERN_C_END
