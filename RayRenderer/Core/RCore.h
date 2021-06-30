#pragma once

#include "../RBase.h"

RR_EXTERN_C_BEG

#define RR_WHOLE_SIZE (~0ULL)
#define RR_MAX_MEMORY_TYPES 32
#define RR_MAX_MEMORY_HEAPS 16
#define RR_MAX_PHYSICAL_DEVICE_NAME_SIZE 256
#define RR_UUID_SIZE 16
#define RR_DEFINE_HANDLE(object) typedef struct object##_T* object


typedef uint64_t RrDeviceSize;
typedef uint32_t RrBool32;
typedef void (*RrPFNVoidFunction)(void);


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
