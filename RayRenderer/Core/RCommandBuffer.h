#pragma once

#include "RCore.h"
#include "RCommandPool.h"
#include "RRenderPass.h"
#include "RFramebuffer.h"
#include "../Synchronization/RFence.h"
#include "../Synchronization/RSemaphore.h"

RR_DEFINE_HANDLE(RrCommandBuffer);

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
typedef RrError (*RrPFNAllocateCommandBuffers)(
	RrLogicalDevice, RrCommandBufferAllocateInfo*, RrCommandBuffer*);

RR_API RrError RrBeginCommandBuffer(
	RrCommandBuffer commandBuffer, RrCommandBufferBeginInfo* pBeginInfo);
typedef RrError (*RrPFNBeginCommandBuffer)(RrCommandBuffer, RrCommandBufferBeginInfo*);

RR_API void RrExecuteCommands(RrCommandBuffer commandBuffer, uint32_t secondaryCommandBufferCount,
	const RrCommandBuffer* pSecondaryCommandBuffers);
typedef void (*RrPFNExecuteCommands)(RrCommandBuffer, uint32_t, const RrCommandBuffer*);

RR_API RrError RrQueueSubmit(
	RrQueue queue, uint32_t submitCount, const RrSubmitInfo* pSubmits, RrFence fence);
typedef RrError (*RrPFNQueueSubmit)(RrQueue, uint32_t, const RrSubmitInfo*, RrFence);

RR_API RrError RrQueueWaitIdle(RrQueue queue);
typedef RrError (*RrPFNQueueWaitIdle)(RrQueue);

RR_API RrError RrEndCommandBuffer(RrCommandBuffer commandBuffer);
typedef RrError (*RrPFNEndCommandBuffer)(RrCommandBuffer);

RR_API void RrFreeCommandBuffers(RrLogicalDevice device, RrCommandPool commandPool,
	uint32_t commandBufferCount, const RrCommandBuffer* pCommandBuffers);
typedef void (*RrPFNFreeCommandBuffers)(
	RrLogicalDevice, RrCommandPool, uint32_t, const RrCommandBuffer*);
