#include "Rpch.h"

#include <RayRenderer/Core/RInstance.h>

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

	// Test
	glViewport(0, 0, 800, 600);

	return RrErrorSuccess;
}

void RrDestroyInstance(RrInstance pInstance, RrDebugMessenger pDebugMessenger) {}
