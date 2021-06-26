#include "Rpch.h"

#include "Core/RCore.h"


#if RR_RENDERER_API == RR_RENDERER_API_OPENGL

RrError RrInitialize(const RrInitializeInfo* pInitInfo, RrInstance* pInstance)
{
	if (!gladLoadGLLoader((GLADloadproc)pInitInfo->loaderFunction))
	{
		if (!pInitInfo->loaderFunction)
			return RrErrorIncomplete;
		else
			return RrErrorLoaderFailed;
	}

	// Test
	glViewport(0, 0, 800, 600);

	return RrErrorNone;
}

#endif
