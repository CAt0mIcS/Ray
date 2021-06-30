#pragma once

#include "../Core/RCore.h"

RR_DEFINE_HANDLE(RrSemaphore);
RR_DEFINE_HANDLE(RrLogicalDevice);

typedef enum RrPipelineStageFlagBits
{
	RrPipelineStageTopOfPipe = 0x00000001,
	RrPipelineStageDrawIndirect = 0x00000002,
	RrPipelineStageVertexInput = 0x00000004,
	RrPipelineStageVertexShader = 0x00000008,
	RrPipelineStageTesselationControlShader = 0x00000010,
	RrPipelineStageTesselationEvaluationShader = 0x00000020,
	RrPipelineStageGeometryShader = 0x00000040,
	RrPipelineStageFragmentShader = 0x00000080,
	RrPipelineStageEarlyFragmentTests = 0x00000100,
	RrPipelineStageLateFragmentTests = 0x00000200,
	RrPipelineStageColorAttachmentOutput = 0x00000400,
	RrPipelineStageComputeShader = 0x00000800,
	RrPipelineStageTransfer = 0x00001000,
	RrPipelineStageBottomOfPipe = 0x00002000,
	RrPipelineStageHostAll = 0x00004000,
	RrPipelineStageAllGraphics = 0x00008000,
	RrPipelineStageAllCommands = 0x00010000,
	RrPipelineStageTransformFeedbackEXT = 0x01000000,
	RrPipelineStageConditionalRenderingEXT = 0x00040000,
	RrPipelineStageAccelerationStructureBuildKHR = 0x02000000,
	RrPipelineStageRayTracingShaderKHR = 0x00200000,
	RrPipelineStageShadingRateImageNV = 0x00400000,
	RrPipelineStageTaskShaderNV = 0x00080000,
	RrPipelineStageMeshShaderNV = 0x00100000,
	RrPipelineStageFragmentDensityProcessEXT = 0x00800000,
	RrPipelineStageCommandPreprocessBitNV = 0x00020000
} RrPipelineStageFlagBits;
typedef uint32_t RrPipelineStageFlags;


RR_API RrError RrCreateSemaphore(RrLogicalDevice device, RrSemaphore* pSemaphore);

RR_API void RrDestroySemaphore(RrLogicalDevice device, RrSemaphore semaphore);
