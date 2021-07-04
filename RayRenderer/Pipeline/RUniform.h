#pragma once

#include "../Core/RCore.h"
#include "RShader.h"

RR_EXTERN_C_BEG

typedef struct RrPushConstantRange
{
	RrShaderStageFlags stageFlags;
	uint32_t offset;
	uint32_t size;
} RrPushConstantRange;

RR_EXTERN_C_END
