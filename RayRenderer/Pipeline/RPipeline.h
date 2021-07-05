#pragma once

#include "../Core/RCore.h"
#include "RShader.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrPipeline);
RR_DEFINE_HANDLE(RrLogicalDevice);
RR_DEFINE_HANDLE(RrPipelineCache);
RR_DEFINE_HANDLE(RrRenderPass);
RR_DEFINE_HANDLE(RrPipelineLayout);
RR_DEFINE_HANDLE(RrCommandBuffer);
RR_DEFINE_HANDLE(RrDescriptorSetLayout);

struct RrPushConstantRange;

typedef struct RrPipelineLayoutCreateInfo
{
	uint32_t setLayoutCount;
	const RrDescriptorSetLayout* pSetLayouts;
	uint32_t pushConstantRangeCount;
	const struct RrPushConstantRange* pPushConstantRanges;
} RrPipelineLayoutCreateInfo;


typedef enum RrVertexInputRate
{
	RrVertexInputRateVertex,
	RrVertexInputRateInstance
} RrVertexInputRate;

typedef struct RrVertexInputBindingDescription
{
	uint32_t binding;
	uint32_t stride;
	RrVertexInputRate inputRate;
} RrVertexInputBindingDescription;

typedef struct RrVertexInputAttributeDescription
{
	uint32_t location;
	uint32_t binding;
	RrFormat format;
	uint32_t offset;
} RrVertexInputAttributeDescription;

typedef struct RrPipelineVertexInputStateCreateInfo
{
	uint32_t vertexBindingDescriptionCount;
	const RrVertexInputBindingDescription* pVertexBindingDescriptions;
	uint32_t vertexAttributeDescriptionCount;
	const RrVertexInputAttributeDescription* pVertexAttributeDescriptions;
} RrPipelineVertexInputStateCreateInfo;

typedef struct RrPipelineShaderStageCreateInfo
{
	RrShaderStageFlags stage;
	RrShaderModule module;
	const char* pName;
} RrPipelineShaderStageCreateInfo;

typedef enum RrPrimitiveTopology
{
	RrPrimitiveTopologyPointList = 0,
	RrPrimitiveTopologyLineList = 1,
	RrPrimitiveTopologyLineStrip = 2,
	RrPrimitiveTopologyTriangleList = 3,
	RrPrimitiveTopologyTriangleStrip = 4,
	RrPrimitiveTopologyTriangleFan = 5,
	RrPrimitiveTopologyLineListWithAdjacency = 6,
	RrPrimitiveTopologyLineStripWithAdjacency = 7,
	RrPrimitiveTopologyTriangleListWithAdjacency = 8,
	RrPrimitiveTopologyTriangleStripWithAdjacency = 9,
	RrPrimitiveTopologyPatchList = 10,
} RrPrimitiveTopology;

typedef struct RrPipelineInputAssemblyStateCreateInfo
{
	RrPrimitiveTopology topology;
	RrBool32 primitiveRestartEnable;
} RrPipelineInputAssemblyStateCreateInfo;

typedef struct RrPipelineViewportStateCreateInfo
{
	uint32_t viewportCount;
	const RrViewport* pViewports;
	uint32_t scissorCount;
	const RrRect2D* pScissors;
} RrPipelineViewportStateCreateInfo;

typedef enum RrPolygonMode
{
	RrPolygonModeFill = 0,
	RrPolygonModeLine = 1,
	RrPolygonModePoint = 2,
	RrPolygonModeFillRectangleNV = 1000153000,
} RrPolygonMode;

typedef enum RrCullModeFlagBits
{
	RrCullModeNone = 0,
	RrCullModeFront = 0x00000001,
	RrCullModeBack = 0x00000002,
	RrCullModeFrontAndBack = 0x00000003,
} RrCullModeFlagBits;
typedef uint32_t RrCullModeFlags;

typedef enum RrFrontFace
{
	RrFrontFaceCounterClockwise = 0,
	RrFrontFaceClockwise = 1,
} RrFrontFace;

typedef struct RrPipelineRasterizationStateCreateInfo
{
	RrBool32 depthClampEnable;
	RrBool32 rasterizerDiscardEnable;
	RrPolygonMode polygonMode;
	RrCullModeFlags cullMode;
	RrFrontFace frontFace;
	RrBool32 depthBiasEnable;
	float depthBiasConstantFactor;
	float depthBiasClamp;
	float depthBiasSlopeFactor;
	float lineWidth;
} RrPipelineRasterizationStateCreateInfo;

typedef uint32_t RrSampleMask;

typedef struct RrPipelineMultisampleStateCreateInfo
{
	RrSampleCountFlags rasterizationSamples;
	RrBool32 sampleShadingEnable;
	float minSampleShading;
	const RrSampleMask* pSampleMask;
	RrBool32 alphaToCoverageEnable;
	RrBool32 alphaToOneEnable;
} RrPipelineMultisampleStateCreateInfo;

typedef enum RrStencilOp
{
	RrStencilOpKeep = 0,
	RrStencilOpZero = 1,
	RrStencilOpReplace = 2,
	RrStencilOpIncrementAndClamp = 3,
	RrStencilOpDecrementAndClamp = 4,
	RrStencilOpInvert = 5,
	RrStencilOpIncrementAndWarp = 6,
	RrStencilOpDecrementAndWrap = 7
} RrStencilOp;

typedef struct RrStencilOpState
{
	RrStencilOp failOp;
	RrStencilOp passOp;
	RrStencilOp depthFailOp;
	RrCompareOp compareOp;
	uint32_t compareMask;
	uint32_t writeMask;
	uint32_t reference;
} RrStencilOpState;

typedef struct RrPipelineDepthStencilStateCreateInfo
{
	RrBool32 depthTestEnable;
	RrBool32 depthWriteEnable;
	RrCompareOp depthCompareOp;
	RrBool32 depthBoundsTestEnable;
	RrBool32 stencilTestEnable;
	RrStencilOpState front;
	RrStencilOpState back;
	float minDepthBounds;
	float maxDepthBounds;
} RrPipelineDepthStencilStateCreateInfo;

typedef enum RrBlendFactor
{
	RrBlendFactorZero = 0,
	RrBlendFactorOne = 1,
	RrBlendFactorSrcColor = 2,
	RrBlendFactorOneMinusSrcColor = 3,
	RrBlendFactorDstColor = 4,
	RrBlendFactorOneMinusDstColor = 5,
	RrBlendFactorSrcAlpha = 6,
	RrBlendFactorOneMinusSrcAlpha = 7,
	RrBlendFactorDstAlpha = 8,
	RrBlendFactorOneMinusDstAlpha = 9,
	RrBlendFactorConstantColor = 10,
	RrBlendFactorOneMinusConstantColor = 11,
	RrBlendFactorConstantAlpha = 12,
	RrBlendFactorOneMinusConstantAlpha = 13,
	RrBlendFactorSrcAlphaSaturate = 14,
	RrBlendFactorSrc1Color = 15,
	RrBlendFactorOneMinusSrc1Color = 16,
	RrBlendFactorSrc1Alpha = 17,
	RrBlendFactorOneMinusSrc1Alpha = 18
} RrBlendFactor;

typedef enum RrBlendOp
{
	RrBlendOpAdd = 0,
	RrBlendOpSubtract = 1,
	RrBlendOpReverseSubtract = 2,
	RrBlendOpMin = 3,
	RrBlendOpMax = 4,
	RrBlendOpZeroEXT = 1000148000,
	RrBlendOpSrcEXT = 1000148001,
	RrBlendOpDstEXT = 1000148002,
	RrBlendOpSrcOverEXT = 1000148003,
	RrBlendOpDstOverEXT = 1000148004,
	RrBlendOpSrcInEXT = 1000148005,
	RrBlendOpDstInEXT = 1000148006,
	RrBlendOpSrcOutEXT = 1000148007,
	RrBlendOpDstOutEXT = 1000148008,
	RrBlendOpSrcAtopEXT = 1000148009,
	RrBlendOpDstAtopEXT = 1000148010,
	RrBlendOpXorEXT = 1000148011,
	RrBlendOpMultiplyEXT = 1000148012,
	RrBlendOpScreenEXT = 1000148013,
	RrBlendOpOverlayEXT = 1000148014,
	RrBlendOpDarkenEXT = 1000148015,
	RrBlendOpLightenEXT = 1000148016,
	RrBlendOpColordodgeEXT = 1000148017,
	RrBlendOpColorburnEXT = 1000148018,
	RrBlendOpHardlightEXT = 1000148019,
	RrBlendOpSoftlightEXT = 1000148020,
	RrBlendOpDifferenceEXt = 1000148021,
	RrBlendOpExclusionEXT = 1000148022,
	RrBlendOpInvertEXT = 1000148023,
	RrBlendOpInvertRgbEXT = 1000148024,
	RrBlendOpLinerdodgeEXT = 1000148025,
	RrBlendOpLinearburnEXT = 1000148026,
	RrBlendOpVividlightEXT = 1000148027,
	RrBlendOpLinearlightEXT = 1000148028,
	RrBlendOpPinlightEXT = 1000148029,
	RrBlendOpHardmixEXT = 1000148030
} RrBlendOp;

typedef enum RrColorComponentFlagBits
{
	RrColorComponentR = 0x00000001,
	RrColorComponentG = 0x00000002,
	RrColorComponentB = 0x00000004,
	RrColorComponentA = 0x00000008
} RrColorComponentFlagBits;
typedef uint32_t RrColorComponentFlags;

typedef struct RrPipelineColorBlendAttachmentState
{
	RrBool32 blendEnable;
	RrBlendFactor srcColorBlendFactor;
	RrBlendFactor dstColorBlendFactor;
	RrBlendOp colorBlendOp;
	RrBlendFactor srcAlphaBlendFactor;
	RrBlendFactor dstAlphaBlendFactor;
	RrBlendOp alphaBlendOp;
	RrColorComponentFlags colorWriteMask;
} RrPipelineColorBlendAttachmentState;

typedef struct RrPipelineColorBlendStateCreateInfo
{
	RrBool32 logicOpEnable;
	RrLogicOp logicOp;
	uint32_t attachmentCount;
	const RrPipelineColorBlendAttachmentState* pAttachments;
	float blendConstants[4];
} RrPipelineColorBlendStateCreateInfo;

typedef enum RrDynamicState
{
	RrDynamicStateViewport = 0,
	RrDynamicStateScissor = 1,
	RrDynamicStateLineWidth = 2,
	RrDynamicStateDepthBias = 3,
	RrDynamicStateBlendConstants = 4,
	RrDynamicStateDepthBounds = 5,
	RrDynamicStateStencilCompareMask = 6,
	RrDynamicStateStencilWriteMast = 7,
	RrDynamicStateStencilReference = 8,
	RrDynamicStateViewportWScalingNV = 1000087000,
	RrDynamicStateDiscartRectangleEXT = 1000099000,
	RrDynamicStateSampleLocationsEXT = 1000143000,
	RrDynamicStateRayTracingPipelinStackSizeKHR = 1000347000,
	RrDynamicStateViewportShadingRatePaletteNV = 1000164004,
	RrDynamicStateViewportCoarseSampleOrderNV = 1000164006,
	RrDynamicStateExclusiveScissorNV = 1000205001,
	RrDynamicStateFragmentShadingRateKHR = 1000226000,
	RrDynamicStateLineStippleEXT = 1000259000,
	RrDynamicStateCullModeEXT = 1000267000,
	RrDynamicStateFrontFaceEXT = 1000267001,
	RrDynamicStatePrimitiveTopologyEXT = 1000267002,
	RrDynamicStateViewportWithCountEXT = 1000267003,
	RrDynamicStateScissorWithCountEXT = 1000267004,
	RrDynamicStateVertexInputBindingStrideEXT = 1000267005,
	RrDynamicStateDepthTestEnableEXT = 1000267006,
	RrDynamicStateDepthWriteEnableEXT = 1000267007,
	RrDynamicStateDepthCompareOpEXT = 1000267008,
	RrDynamicStateDepthBoundsEnableEXT = 1000267009,
	RrDynamicStateStencilTestEnableEXT = 1000267010,
	RrDynamicStateStencilOpEXT = 1000267011
} RrDynamicState;

typedef struct RrPipelineDynamicStateCreateInfo
{
	uint32_t dynamicStateCount;
	const RrDynamicState* pDynamicStates;
} RrPipelineDynamicStateCreateInfo;

typedef struct RrPipelineTessellationStateCreateInfo
{
	uint32_t patchControlPoints;
} RrPipelineTessellationStateCreateInfo;

typedef enum RrPipelineCreateFlagBits
{
	RrPipelineCreateDisableOptimizations = 0x00000001,
	RrPipelineCreateAllowDerivatives = 0x00000002,
	RrPipelineCreateDerivatives = 0x00000004,
	RrPipelineCreateViewIndexFromDeviceIndex = 0x00000008,
	RrPipelineCreateDispatchBase = 0x00000010,
	RrPipelineCreateRayTracingNoHullAnyHitShadersKHR = 0x00004000,
	RrPipelineCreateRayTracingNoHullClosestHitShadersKHR = 0x00008000,
	RrPipelineCreateRayTracingNoHullMissShadersKHR = 0x00010000,
	RrPipelineCreateRayTracingNoHullIntersectionShadersKHR = 0x00020000,
	RrPipelineCreateRayTracingSkipTrianglesKHR = 0x00001000,
	RrPipelineCreateRayTracingSkipAABBSKHR = 0x00002000,
	RrPipelineCreateRayTracingShaderGroupHandleCaptureReplayKHR = 0x00080000,
	RrPipelineCreateDeferCompileNV = 0x00000020,
	RrPipelineCreateCaptureStatisticsKHR = 0x00000040,
	RrPipelineCreateCaptureInternalRepresentationsKHR = 0x00000080,
	RrPipelineCreateIndirectBindableNV = 0x00040000,
	RrPipelineCreateLibraryKHR = 0x00000800,
	RrPipelineCreateFailOnPipelineCompileRequiredEXT = 0x00000100,
	RrPipelineCreateEarlyReturnOnFailureEXT = 0x00000200
} RrPipelineCreateFlagBits;
typedef uint32_t RrPipelineCreateFlags;

typedef struct RrGraphicsPipelineCreateInfo
{
	RrPipelineCreateFlags flags;
	uint32_t stageCount;
	const RrPipelineShaderStageCreateInfo* pStages;
	const RrPipelineVertexInputStateCreateInfo* pVertexInputState;
	const RrPipelineInputAssemblyStateCreateInfo* pInputAssemblyState;
	const RrPipelineTessellationStateCreateInfo* pTessellationState;
	const RrPipelineViewportStateCreateInfo* pViewportState;
	const RrPipelineRasterizationStateCreateInfo* pRasterizationState;
	const RrPipelineMultisampleStateCreateInfo* pMultisampleState;
	const RrPipelineDepthStencilStateCreateInfo* pDepthStencilState;
	const RrPipelineColorBlendStateCreateInfo* pColorBlendState;
	const RrPipelineDynamicStateCreateInfo* pDynamicState;
	RrPipelineLayout layout;
	RrRenderPass renderPass;
	uint32_t subpass;
	RrPipeline basePipelineHandle;
	int32_t basePipelineIndex;
} RrGraphicsPipelineCreateInfo;

RR_API RrError RrCreatePipelineLayout(RrLogicalDevice device,
	const RrPipelineLayoutCreateInfo* pCreateInfo, RrPipelineLayout* pLayout);

RR_API RrError RrCreateGraphicsPipeline(RrLogicalDevice device, RrPipelineCache pipelineCache,
	const RrGraphicsPipelineCreateInfo* pCreateInfo, RrPipeline* pPipeline);


RR_API void RrCmdSetViewport(RrCommandBuffer commandBuffer, uint32_t firstViewport,
	uint32_t viewportCount, const RrViewport* pViewports);

RR_API void RrCmdSetScissor(RrCommandBuffer commandBuffer, uint32_t firstScissor,
	uint32_t scissorCount, const RrRect2D* pScissors);

RR_EXTERN_C_END
