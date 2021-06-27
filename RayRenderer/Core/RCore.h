#pragma once

#include "../RBase.h"

RR_EXTERN_C_BEG

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


RR_EXTERN_C_END
