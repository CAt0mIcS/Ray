#include "Rpch.h"

#include "Core/RCore.h"
#include "Core/RUtils.h"


#if RR_RENDERER_API == RR_RENDERER_API_OPENGL

bool (*RrpfnValidationCallback)(RrLogMessageSeverity, const char*) = NULL;


RrError RrInitialize(
	const RrInitializeInfo* pInitInfo, RrInstance* pInstance, RrDebugMessenger* pDebugMessenger)
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

	return RrErrorNone;
}

void RrDestroyInstance(RrInstance pInstance, RrDebugMessenger pDebugMessenger) {}

#endif
