#pragma once

#include "../Core/RCore.h"
#include "RShader.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrLogicalDevice);
RR_DEFINE_HANDLE(RrCommandBuffer);
RR_DEFINE_HANDLE(RrPipelineLayout);

typedef struct RrPushConstantRange
{
	RrShaderStageFlags stageFlags;
	uint32_t offset;
	uint32_t size;
} RrPushConstantRange;

RR_API void RrCmdPushConstants(RrCommandBuffer commandBuffer, RrPipelineLayout pipelineLayout,
	RrShaderStageFlags shaderStage, uint32_t offset, uint32_t size, const void* pValues);

RR_EXTERN_C_END
