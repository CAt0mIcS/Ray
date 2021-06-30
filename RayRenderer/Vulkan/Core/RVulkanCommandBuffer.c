#include "Rpch.h"

#include <../Core/RCommandBuffer.h>
#include <../Core/RUtils.h>


RrError RrAllocateCommandBuffers(RrLogicalDevice device, RrCommandBufferAllocateInfo* pAllocInfo,
	RrCommandBuffer* pCommandBuffers)
{
	VkCommandBufferAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.pNext = NULL;
	allocInfo.commandPool = (VkCommandPool)pAllocInfo->commandPool;
	allocInfo.level = (RrCommandBufferLevel)pAllocInfo->level;
	allocInfo.commandBufferCount = pAllocInfo->commandBufferCount;

	return GetError(
		vkAllocateCommandBuffers((VkDevice)device, &allocInfo, (VkCommandBuffer*)pCommandBuffers));
}

RrError RrBeginCommandBuffer(RrCommandBuffer commandBuffer, RrCommandBufferBeginInfo* pBeginInfo)
{
	VkCommandBufferInheritanceInfo inheritanceInfo;
	if (pBeginInfo->pInheritanceInfo)
	{
		inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		inheritanceInfo.pNext = NULL;
		inheritanceInfo.renderPass = (VkRenderPass)pBeginInfo->pInheritanceInfo->renderPass;
		inheritanceInfo.subpass = pBeginInfo->pInheritanceInfo->subpass;
		inheritanceInfo.framebuffer = (VkFramebuffer)pBeginInfo->pInheritanceInfo->framebuffer;
		inheritanceInfo.occlusionQueryEnable = VK_FALSE;
		inheritanceInfo.queryFlags = 0;
		inheritanceInfo.pipelineStatistics = 0;
	}

	VkCommandBufferBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = NULL;
	beginInfo.flags = (VkCommandBufferUsageFlags)pBeginInfo->flags;
	if (pBeginInfo->pInheritanceInfo)
		beginInfo.pInheritanceInfo = &inheritanceInfo;
	else
		beginInfo.pInheritanceInfo = NULL;

	return GetError(vkBeginCommandBuffer((VkCommandBuffer)commandBuffer, &beginInfo));
}

void RrExecuteCommands(RrCommandBuffer commandBuffer, uint32_t secondaryCommandBufferCount,
	const RrCommandBuffer* secondaryCommandBuffers)
{
	vkCmdExecuteCommands((VkCommandBuffer)commandBuffer, secondaryCommandBufferCount,
		(const VkCommandBuffer*)secondaryCommandBuffers);
}

RrError RrQueueSubmit(
	RrQueue queue, uint32_t submitCount, const RrSubmitInfo* pSubmits, RrFence fence)
{
	VkSubmitInfo* submits = malloc(sizeof(VkSubmitInfo) * submitCount);
	for (uint32_t i = 0; i < submitCount; ++i)
	{
		submits[i].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submits[i].pNext = NULL;
		submits[i].waitSemaphoreCount = pSubmits[i].waitSemaphoreCount;
		submits[i].pWaitSemaphores = (const VkSemaphore*)pSubmits[i].pWaitSemaphores;
		submits[i].pWaitDstStageMask = (const VkPipelineStageFlags*)pSubmits[i].pWaitDstStageMask;
		submits[i].commandBufferCount = pSubmits[i].commandBufferCount;
		submits[i].pCommandBuffers = (const VkCommandBuffer*)pSubmits[i].pCommandBuffers;
		submits[i].signalSemaphoreCount = pSubmits[i].signalSemaphoreCount;
		submits[i].pSignalSemaphores = (const VkSemaphore*)pSubmits[i].pSignalSemaphores;
	}

	RrError error = vkQueueSubmit((VkQueue)queue, submitCount, submits, (VkFence)fence);
	free(submits);
	return error;
}

RrError RrQueueWaitIdle(RrQueue queue)
{
	return GetError(vkQueueWaitIdle((VkQueue)queue));
}

RrError RrEndCommandBuffer(RrCommandBuffer commandBuffer)
{
	return GetError(vkEndCommandBuffer((VkCommandBuffer)commandBuffer));
}

void RrFreeCommandBuffers(RrLogicalDevice device, RrCommandPool commandPool,
	uint32_t commandBufferCount, const RrCommandBuffer* pCommandBuffers)
{
	vkFreeCommandBuffers((VkDevice)device, (VkCommandPool)commandPool, commandBufferCount,
		(const VkCommandBuffer*)pCommandBuffers);
}
