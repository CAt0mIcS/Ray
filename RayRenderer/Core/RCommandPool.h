#pragma once

#include "RCore.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrCommandPool);
RR_DEFINE_HANDLE(RrLogicalDevice);

typedef enum RrCommandPoolCreateFlagBits
{
	RrCommandPoolCreateTransient = 0x00000001,
	RrCommandPoolCreateResetCommandBuffer = 0x00000002,
	RrCommandPoolCreateProtected = 0x00000004,
} RrCommandPoolCreateFlagBits;
typedef uint32_t RrCommandPoolCreateFlags;

typedef enum RrCommandPoolResetFlagBits
{
	RrCommandPoolResetReleaseResources = 0x00000001
} RrCommandPoolResetFlagBits;
typedef uint32_t RrCommandPoolResetFlags;

typedef struct RrCommandPoolCreateInfo
{
	RrCommandPoolCreateFlags flags;
	uint32_t queueFamilyIndex;
} RrCommandPoolCreateInfo;

RR_API RrError RrCreateCommandPool(RrLogicalDevice device,
	const RrCommandPoolCreateInfo* pCreateInfo, RrCommandPool* pCommandPool);

RR_API void RrDestroyCommandPool(RrLogicalDevice device, RrCommandPool commandPool);

RR_API RrError RrResetCommandPool(
	RrLogicalDevice device, RrCommandPool commandPool, RrCommandPoolResetFlags flags);

RR_EXTERN_C_END
