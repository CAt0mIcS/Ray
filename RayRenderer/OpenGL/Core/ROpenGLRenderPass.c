#include "Rpch.h"

#include "../Core/RRenderPass.h"
#include "../Core/RUtils.h"


RrError RrCreateRenderPass(
	RrLogicalDevice device, const RrRenderPassCreateInfo* pCreateInfo, RrRenderPass* pRenderPass)
{
	return RrErrorSuccess;
}

void RrDestroyRenderPass(RrLogicalDevice device, RrRenderPass renderPass) {}

void RrCmdBeginRenderPass(RrCommandBuffer commandBuffer, const RrRenderPassBeginInfo* pBeginInfo,
	RrSubpassContents subpassContents)
{
}

void RrCmdEndRenderPass(RrCommandBuffer commandBuffer) {}