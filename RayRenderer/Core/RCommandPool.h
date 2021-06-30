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

RR_API RrError RrCreateCommandPool(RrLogicalDevice pDevice,
	const RrCommandPoolCreateInfo* const pCreateInfo, RrCommandPool* ppCommandPool);
typedef RrError (*RrPFNCreateCommandPool)(
	RrLogicalDevice, const RrCommandPoolCreateInfo* const, RrCommandPool*);

RR_API void RrDestroyCommandPool(RrLogicalDevice pDevice, RrCommandPool pCommandPool);
typedef void (*RrPFNDestroyCommandPool)(RrLogicalDevice, RrCommandPool);

RR_EXTERN_C_END
