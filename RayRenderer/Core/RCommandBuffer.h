#pragma once

#include "RCore.h"
#include "../Synchronization/RSemaphore.h"

RR_DEFINE_HANDLE(RrCommandBuffer);
RR_DEFINE_HANDLE(RrCommandPool);
RR_DEFINE_HANDLE(RrRenderPass);
RR_DEFINE_HANDLE(RrFramebuffer);
RR_DEFINE_HANDLE(RrFence);
RR_DEFINE_HANDLE(RrQueue);

typedef enum RrCommandBufferLevel
{
	RrCommandBufferLevelPrimary,
	RrcommandBufferLevelSecondary
} RrCommandBufferLevel;

typedef enum RrCommandBufferUsageFlagBits
{
	RrCommandBufferUsageOneTimeSubmit = 0x00000001,
	RrCommandBufferUsageRenderPassContinue = 0x00000002,
	RrCommandBufferUsageSimultaneousUse = 0x00000004,
} RrCommandBufferUsageFlagBits;
typedef uint32_t RrCommandBufferUsageFlags;

typedef struct RrCommandBufferAllocateInfo
{
	RrCommandPool commandPool;
	RrCommandBufferLevel level;
	uint32_t commandBufferCount;
} RrCommandBufferAllocateInfo;

typedef struct RrCommandBufferInheritanceInfo
{
	RrRenderPass renderPass;
	uint32_t subpass;
	RrFramebuffer framebuffer;
} RrCommandBufferInheritanceInfo;

typedef struct RrCommandBufferBeginInfo
{
	RrCommandBufferUsageFlags flags;
	const RrCommandBufferInheritanceInfo* pInheritanceInfo;
} RrCommandBufferBeginInfo;

typedef struct RrSubmitInfo
{
	uint32_t waitSemaphoreCount;
	const RrSemaphore* pWaitSemaphores;
	const RrPipelineStageFlags* pWaitDstStageMask;
	uint32_t commandBufferCount;
	const RrCommandBuffer* pCommandBuffers;
	uint32_t signalSemaphoreCount;
	const RrSemaphore* pSignalSemaphores;
} RrSubmitInfo;

RR_API RrError RrAllocateCommandBuffers(RrLogicalDevice device,
	RrCommandBufferAllocateInfo* pAllocInfo, RrCommandBuffer* pCommandBuffers);

RR_API RrError RrBeginCommandBuffer(
	RrCommandBuffer commandBuffer, RrCommandBufferBeginInfo* pBeginInfo);

RR_API void RrExecuteCommands(RrCommandBuffer commandBuffer, uint32_t secondaryCommandBufferCount,
	const RrCommandBuffer* pSecondaryCommandBuffers);

RR_API RrError RrQueueSubmit(
	RrQueue queue, uint32_t submitCount, const RrSubmitInfo* pSubmits, RrFence fence);

RR_API RrError RrQueueWaitIdle(RrQueue queue);

RR_API RrError RrEndCommandBuffer(RrCommandBuffer commandBuffer);

RR_API void RrFreeCommandBuffers(RrLogicalDevice device, RrCommandPool commandPool,
	uint32_t commandBufferCount, const RrCommandBuffer* pCommandBuffers);
