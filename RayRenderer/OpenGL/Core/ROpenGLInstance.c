#include "Rpch.h"

#include "../Core/RInstance.h"
#include "../Core/RUtils.h"

extern bool (*RrpfnValidationCallback)(RrLogMessageSeverity, const char*);


RrError RrInitialize(
	RrInitializeInfo* const pInitInfo, RrInstance* pInstance, RrDebugMessenger* pDebugMessenger)
{
	RrpfnValidationCallback = pInitInfo->pfnValidationCallback;
	LogError("OpenGL is currently not supported");

	if (!gladLoadGLLoader((GLADloadproc)pInitInfo->pfnLoader))
	{
		if (!pInitInfo->pfnLoader)
		{
			LogError("pInitInfo->pfnLoader was NULL. Expected function to load glad.");
			return RrErrorIncomplete;
		}
		else
			return RrErrorLoaderFailed;
	}

	return RrErrorSuccess;
}

void RrDestroyInstance(RrInstance pInstance, RrDebugMessenger pDebugMessenger) {}

RrPFNVoidFunction RrGetInstanceProcAddr(RrInstance instance, const char* pName)
{
	return NULL;
}

RrError RrEnumerateInstanceExtensionProperties(
	const char* pLayerName, uint32_t* pPropertyCount, RrExtensionProperties* pProperties)
{
	return RrErrorSuccess;
}