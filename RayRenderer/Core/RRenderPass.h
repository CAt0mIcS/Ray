#pragma once

#include "RCore.h"

RR_EXTERN_C_BEG

#define RR_SUBPASS_EXTERNAL (~0U)
RR_DEFINE_HANDLE(RrRenderPass);
RR_DEFINE_HANDLE(RrLogicalDevice);
RR_DEFINE_HANDLE(RrCommandBuffer);
RR_DEFINE_HANDLE(RrFramebuffer);

typedef enum RrAttachmentLoadOp
{
	RrAttachmentLoadOpLoad = 0,
	RrAttachmentLoadOpClear = 1,
	RrAttachmentLoadOpDontCare = 2
} RrAttachmentLoadOp;

typedef enum RrAttachmentStoreOp
{
	RrAttachmentStoreOpStore = 0,
	RrAttachmentStoreOpDontCare = 1,
	RrAttachmentStoreOpNoneQCom = 1000301000
} RrAttachmentStoreOp;

typedef enum RrAttachmentDescriptionFlagBits
{
	RrAttachmentDescriptionMayAlias = 0x00000001
} RrAttachmentDescriptionFlagBits;
typedef uint32_t RrAttachmentDescriptionFlags;

typedef struct RrAttachmentDescription
{
	RrAttachmentDescriptionFlags flags;
	RrFormat format;
	RrSampleCountFlagBits samples;
	RrAttachmentLoadOp loadOp;
	RrAttachmentStoreOp storeOp;
	RrAttachmentLoadOp stencilLoadOp;
	RrAttachmentStoreOp stencilStoreOp;
	RrImageLayout initialLayout;
	RrImageLayout finalLayout;
} RrAttachmentDescription;

typedef enum RrSubpassDescriptionFlagBits
{
	RrSubpassDescriptionPerViewAttributesNVX = 0x00000001,
	RrSubpassDescriptionPerViewPositionnXOnlyBitNVX = 0x00000002,
	RrSubpassDescriptionFraagmentRegionQCom = 0x00000004,
	RrSubpassDescriptionShaderResolveQCom = 0x00000008
} RrSubpassDescriptionFlagBits;
typedef uint32_t RrSubpassDescriptionFlags;

typedef struct RrAttachmentReference
{
	uint32_t attachment;
	RrImageLayout layout;
} RrAttachmentReference;

typedef struct RrSubpassDescription
{
	RrSubpassDescriptionFlags flags;
	RrPipelineBindPoint pipelineBindPoint;
	uint32_t inputAttachmentCount;
	const RrAttachmentReference* pInputAttachments;
	uint32_t colorAttachmentCount;
	const RrAttachmentReference* pColorAttachments;
	const RrAttachmentReference* pResolveAttachments;
	const RrAttachmentReference* pDepthStencilAttachment;
	uint32_t preserveAttachmentCount;
	const uint32_t* pPreserveAttachments;
} RrSubpassDescription;

typedef struct RrSubpassDependency
{
	uint32_t srcSubpass;
	uint32_t dstSubpass;
	RrPipelineStageFlags srcStageMask;
	RrPipelineStageFlags dstStageMask;
	RrAccessFlags srcAccessMask;
	RrAccessFlags dstAccessMask;
	RrDependencyFlags dependencyFlags;
} RrSubpassDependency;

typedef enum RrRenderPassCreateFlagBits
{
	RrRenderPassCreateTransformQCom = 0x00000002
} RrRenderPassCreateFlagBits;
typedef uint32_t RrRenderPassCreateFlags;

typedef struct RrRenderPassCreateInfo
{
	RrRenderPassCreateFlags flags;
	uint32_t attachmentCount;
	const RrAttachmentDescription* pAttachments;
	uint32_t subpassCount;
	const RrSubpassDescription* pSubpasses;
	uint32_t dependencyCount;
	const RrSubpassDependency* pDependencies;
} RrRenderPassCreateInfo;

typedef enum RrSubpassContents
{
	RrSubpassContentsInline = 0,
	RrSubpassContentsSecondaryCommandBuffers = 1
} RrSubpassContents;

typedef union RrClearColorValue
{
	float float32[4];
	int32_t int32[4];
	uint32_t uint32[4];
} RrClearColorValue;

typedef struct RrClearDepthStencilValue
{
	float depth;
	uint32_t stencil;
} RrClearDepthStencilValue;

typedef union RrClearValue
{
	RrClearColorValue color;
	RrClearDepthStencilValue depthStencil;
} RrClearValue;

typedef struct RrRenderPassBeginInfo
{
	RrRenderPass renderPass;
	RrFramebuffer framebuffer;
	RrRect2D renderArea;
	uint32_t clearValueCount;
	const RrClearValue* pClearValues;
} RrRenderPassBeginInfo;

RR_API RrError RrCreateRenderPass(
	RrLogicalDevice device, const RrRenderPassCreateInfo* pCreateInfo, RrRenderPass* pRenderPass);

RR_API void RrDestroyRenderPass(RrLogicalDevice device, RrRenderPass renderPass);

RR_API void RrCmdBeginRenderPass(RrCommandBuffer commandBuffer,
	const RrRenderPassBeginInfo* pBeginInfo, RrSubpassContents subpassContents);

RR_API void RrCmdEndRenderPass(RrCommandBuffer commandBuffer);

RR_EXTERN_C_END
