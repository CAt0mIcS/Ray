#include "Rpch.h"

#include "../Core/RCommandPool.h"

RrError RrCreateCommandPool(RrLogicalDevice pDevice,
	const RrCommandPoolCreateInfo* const pCreateInfo, RrCommandPool* ppCommandPool)
{
	*ppCommandPool = NULL;
	return RrErrorSuccess;
}

void RrDestroyCommandPool(RrLogicalDevice pDevice, RrCommandPool pCommandPool) {}
