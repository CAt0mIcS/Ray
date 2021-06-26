#include "Rpch.h"

#include "Core/RCore.h"


#if RR_RENDERER_API == RR_RENDERER_API_OPENGL

RrError RrInitialize(const RrInitializeInfo* pInitInfo, RrInstance* pInstance)
{
	// Test
	glViewport(10, 10, 10, 10);

	return RrErrorNone;
}

#endif
