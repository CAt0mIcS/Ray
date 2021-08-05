#include "Rpch.h"

#include "../Synchronization/RFence.h"
#include "../Core/RUtils.h"


RrError RrCreateFence(RrLogicalDevice device, const RrFenceCreateInfo* pCreateInfo, RrFence* pFence)
{
	return RrErrorSuccess;
}

RrError RrWaitForFences(RrLogicalDevice device, uint32_t fenceCount, const RrFence* pFences,
	RrBool32 waitAll, uint64_t timeout)
{
	return RrErrorSuccess;
}

RrError RrResetFences(RrLogicalDevice device, uint32_t fenceCount, const RrFence* pFences)
{
	return RrErrorSuccess;
}

void RrDestroyFence(RrLogicalDevice device, RrFence fence) {}