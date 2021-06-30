#pragma once

#include "../Core/RCore.h"

RR_DEFINE_HANDLE(RrFence);
RR_DEFINE_HANDLE(RrLogicalDevice);

typedef enum RrFenceCreateFlagBits
{
	RrFenceCreateSignaled = 0x00000001
} RrFenceCreateFlagBits;
typedef uint32_t RrFenceCreateFlags;

typedef struct RrFenceCreateInfo
{
	RrFenceCreateFlags flags;
} RrFenceCreateInfo;


RR_API RrError RrCreateFence(
	RrLogicalDevice device, const RrFenceCreateInfo* pCreateInfo, RrFence* pFence);

RR_API RrError RrWaitForFences(RrLogicalDevice device, uint32_t fenceCount, const RrFence* pFences,
	RrBool32 waitAll, uint64_t timeout);

RR_API RrError RrResetFences(RrLogicalDevice device, uint32_t fenceCount, const RrFence* pFences);

RR_API void RrDestroyFence(RrLogicalDevice device, RrFence fence);
