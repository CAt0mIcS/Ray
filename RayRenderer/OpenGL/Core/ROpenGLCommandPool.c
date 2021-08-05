#include "Rpch.h"

#include "../Core/RCommandPool.h"
#include "../Core/RUtils.h"

RrError RrCreateCommandPool(
	RrLogicalDevice device, const RrCommandPoolCreateInfo* pCreateInfo, RrCommandPool* pCommandPool)
{
	return RrErrorSuccess;
}

void RrDestroyCommandPool(RrLogicalDevice device, RrCommandPool commandPool) {}

RrError RrResetCommandPool(
	RrLogicalDevice device, RrCommandPool commandPool, RrCommandPoolResetFlags flags)
{
	return RrErrorSuccess;
}
