#pragma once

#include "RCore.h"
#include "RImageView.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrFramebuffer);
RR_DEFINE_HANDLE(RrRenderPass);

typedef enum RrFramebufferCreateFlagBits
{
	VkFramebufferCreateImageless = 0x00000001
} RrFramebufferCreateFlagBits;
typedef uint32_t RrFramebufferCreateFlags;

typedef struct RrFramebufferCreateInfo
{
	RrFramebufferCreateFlags flags;
	RrRenderPass renderPass;
	uint32_t attachmentCount;
	const RrImageView* pAttachments;
	uint32_t width;
	uint32_t height;
	uint32_t layers;
} RrFramebufferCreateInfo;


RR_API RrError RrCreateFramebuffer(RrLogicalDevice device,
	const RrFramebufferCreateInfo* pCreateInfo, RrFramebuffer* pFramebuffer);

RR_API void RrDestroyFramebuffer(RrLogicalDevice device, RrFramebuffer framebuffer);

RR_EXTERN_C_END
