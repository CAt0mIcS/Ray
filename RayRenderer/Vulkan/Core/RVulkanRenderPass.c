#include "Rpch.h"
#include <../Core/RRenderPass.h>
#include <../Core/RUtils.h>


RrError RrCreateRenderPass(
	RrLogicalDevice device, const RrRenderPassCreateInfo* pCreateInfo, RrRenderPass* pRenderPass)
{
	VkRenderPassCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = 0;
	createInfo.attachmentCount = pCreateInfo->attachmentCount;
	createInfo.subpassCount = pCreateInfo->subpassCount;
	createInfo.dependencyCount = pCreateInfo->dependencyCount;

	VkAttachmentDescription* pAttachments = NULL;
	if (createInfo.attachmentCount != 0)
		pAttachments = malloc(sizeof(VkAttachmentDescription) * createInfo.attachmentCount);
	VkSubpassDescription* pSubpasses = NULL;
	if (createInfo.subpassCount != 0)
		pSubpasses = malloc(sizeof(VkSubpassDescription) * createInfo.subpassCount);
	VkSubpassDependency* pDependencies = NULL;
	if (createInfo.dependencyCount != 0)
		pDependencies = malloc(sizeof(VkSubpassDependency) * createInfo.dependencyCount);

	if (pAttachments != NULL)
		for (uint32_t i = 0; i < createInfo.attachmentCount; ++i)
		{
			pAttachments[i].flags = pCreateInfo->pAttachments[i].flags;
			pAttachments[i].format = pCreateInfo->pAttachments[i].format;
			pAttachments[i].samples = pCreateInfo->pAttachments[i].samples;
			pAttachments[i].loadOp = pCreateInfo->pAttachments[i].loadOp;
			pAttachments[i].storeOp = pCreateInfo->pAttachments[i].storeOp;
			pAttachments[i].stencilLoadOp = pCreateInfo->pAttachments[i].stencilLoadOp;
			pAttachments[i].stencilStoreOp = pCreateInfo->pAttachments[i].stencilStoreOp;
			pAttachments[i].initialLayout = pCreateInfo->pAttachments[i].initialLayout;
			pAttachments[i].finalLayout = pCreateInfo->pAttachments[i].finalLayout;
		}

	if (pSubpasses != NULL)
		for (uint32_t i = 0; i < createInfo.subpassCount; ++i)
		{
			pSubpasses[i].flags = pCreateInfo->pSubpasses[i].flags;
			pSubpasses[i].pipelineBindPoint = pCreateInfo->pSubpasses[i].pipelineBindPoint;
			pSubpasses[i].inputAttachmentCount = pCreateInfo->pSubpasses[i].inputAttachmentCount;
			pSubpasses[i].pInputAttachments =
				(const VkAttachmentReference*)pCreateInfo->pSubpasses[i].pInputAttachments;
			pSubpasses[i].colorAttachmentCount = pCreateInfo->pSubpasses[i].colorAttachmentCount;
			pSubpasses[i].pColorAttachments =
				(const VkAttachmentReference*)pCreateInfo->pSubpasses[i].pColorAttachments;
			pSubpasses[i].pResolveAttachments =
				(const VkAttachmentReference*)pCreateInfo->pSubpasses[i].pResolveAttachments;
			pSubpasses[i].pDepthStencilAttachment =
				(const VkAttachmentReference*)pCreateInfo->pSubpasses[i].pDepthStencilAttachment;
			pSubpasses[i].preserveAttachmentCount =
				pCreateInfo->pSubpasses[i].preserveAttachmentCount;
			pSubpasses[i].pPreserveAttachments = pCreateInfo->pSubpasses[i].pPreserveAttachments;
		}

	if (pDependencies != NULL)
		for (uint32_t i = 0; i < createInfo.dependencyCount; ++i)
		{
			pDependencies[i].srcSubpass = pCreateInfo->pDependencies[i].srcSubpass;
			pDependencies[i].dstSubpass = pCreateInfo->pDependencies[i].dstSubpass;
			pDependencies[i].srcStageMask = pCreateInfo->pDependencies[i].srcStageMask;
			pDependencies[i].dstStageMask = pCreateInfo->pDependencies[i].dstStageMask;
			pDependencies[i].srcAccessMask = pCreateInfo->pDependencies[i].srcAccessMask;
			pDependencies[i].dstAccessMask = pCreateInfo->pDependencies[i].dstAccessMask;
			pDependencies[i].dependencyFlags = pCreateInfo->pDependencies[i].dependencyFlags;
		}

	createInfo.pAttachments = pAttachments;
	createInfo.pSubpasses = pSubpasses;
	createInfo.pDependencies = pDependencies;

	RrError error = GetError(
		vkCreateRenderPass((VkDevice)device, &createInfo, NULL, (VkRenderPass*)pRenderPass));

	if (pAttachments != NULL)
		free(pAttachments);
	if (pSubpasses != NULL)
		free(pSubpasses);
	if (pDependencies != NULL)
		free(pDependencies);
	return error;
}

void RrDestroyRenderPass(RrLogicalDevice device, RrRenderPass renderPass)
{
	vkDestroyRenderPass((VkDevice)device, (VkRenderPass)renderPass, NULL);
}

void RrCmdBeginRenderPass(RrCommandBuffer commandBuffer, const RrRenderPassBeginInfo* pBeginInfo,
	RrSubpassContents subpassContents)
{
	VkRenderPassBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	beginInfo.pNext = NULL;
	beginInfo.renderPass = (VkRenderPass)pBeginInfo->renderPass;
	beginInfo.framebuffer = (VkFramebuffer)pBeginInfo->framebuffer;

	VkRect2D renderArea;
	renderArea.extent.width = pBeginInfo->renderArea.extent.width;
	renderArea.extent.height = pBeginInfo->renderArea.extent.height;
	renderArea.offset.x = pBeginInfo->renderArea.offset.x;
	renderArea.offset.y = pBeginInfo->renderArea.offset.y;

	beginInfo.renderArea = renderArea;
	beginInfo.clearValueCount = pBeginInfo->clearValueCount;
	beginInfo.pClearValues = (const VkClearValue*)pBeginInfo->pClearValues;

	vkCmdBeginRenderPass((VkCommandBuffer)commandBuffer, &beginInfo, subpassContents);
}

void RrCmdEndRenderPass(RrCommandBuffer commandBuffer)
{
	vkCmdEndRenderPass((VkCommandBuffer)commandBuffer);
}
