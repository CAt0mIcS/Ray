#pragma once

#include "../Core/RCore.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrShaderModule);
RR_DEFINE_HANDLE(RrLogicalDevice);

typedef enum RrShaderStageFlagBits
{
	RrShaderStageVertex = 0x00000001,
	RrShaderStageTessellationControl = 0x00000002,
	RrShaderStageTessellationEvaluation = 0x00000004,
	RrShaderStageGeometry = 0x00000008,
	RrShaderStageFragment = 0x00000010,
	RrShaderStageCompute = 0x00000020,
	RrShaderStageAllGraphics = 0x0000001F,
	RrShaderStageAll = 0x7FFFFFFF,
	RrShaderStageRayGenKHR = 0x00000100,
	RrShaderStageAnyHitKHR = 0x00000200,
	RrShaderStageClosestHitKHR = 0x00000400,
	RrShaderStageMissKHR = 0x00000800,
	RrShaderStageIntersectionKHR = 0x00001000,
	RrShaderStageCallableKHR = 0x00002000,
	RrShaderStageTaskNV = 0x00000040,
	RrShaderStageMeshNV = 0x00000080,
} RrShaderStageFlagBits;
typedef uint32_t RrShaderStageFlags;

typedef struct RrShaderModuleCreateInfo
{
	size_t codeSize;
	const uint32_t* pCode;
} RrShaderModuleCreateInfo;

RR_API RrError RrCreateShaderModule(RrLogicalDevice device,
	const RrShaderModuleCreateInfo* pCreateInfo, RrShaderModule* pShaderModule);

RR_API void RrDestroyShaderModule(RrLogicalDevice device, RrShaderModule shaderModule);

RR_EXTERN_C_END
