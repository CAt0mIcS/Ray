#pragma once

#include "RLogicalDevice.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrCommandPool);

typedef enum RrCommandPoolCreateFlagBits
{
	RrCommandPoolCreateTransient = 0x00000001,
	RrCommandPoolCreateResetCommandBuffer = 0x00000002,
	RrCommandPoolCreateProtected = 0x00000004,
} RrCommandPoolCreateFlagBits;
typedef uint32_t RrCommandPoolCreateFlags;

typedef struct RrCommandPoolCreateInfo
{
	RrCommandPoolCreateFlags flags;
	uint32_t queueFamilyIndex;
} RrCommandPoolCreateInfo;

RR_API RrError RrCreateCommandPool(RrLogicalDevice device,
	const RrCommandPoolCreateInfo* pCreateInfo, RrCommandPool* pCommandPool);

RR_API void RrDestroyCommandPool(RrLogicalDevice device, RrCommandPool commandPool);

RR_EXTERN_C_END
