#include "Rpch.h"
#include <../Core/RFramebuffer.h>
#include <../Core/RUtils.h>


RrError RrCreateFramebuffer(
	RrLogicalDevice device, const RrFramebufferCreateInfo* pCreateInfo, RrFramebuffer* pFramebuffer)
{
	VkFramebufferCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = pCreateInfo->flags;
	createInfo.renderPass = (VkRenderPass)pCreateInfo->renderPass;
	createInfo.attachmentCount = pCreateInfo->attachmentCount;
	createInfo.pAttachments = (const VkImageView*)pCreateInfo->pAttachments;
	createInfo.width = pCreateInfo->width;
	createInfo.height = pCreateInfo->height;
	createInfo.layers = pCreateInfo->layers;

	return GetError(
		vkCreateFramebuffer((VkDevice)device, &createInfo, NULL, (VkFramebuffer*)pFramebuffer));
}

void RrDestroyFramebuffer(RrLogicalDevice device, RrFramebuffer framebuffer)
{
	vkDestroyFramebuffer((VkDevice)device, (VkFramebuffer)framebuffer, NULL);
}
