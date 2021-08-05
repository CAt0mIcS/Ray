#include "Rpch.h"

#include "../Pipeline/RDescriptor.h"
#include "../Core/RUtils.h"


RrError RrCreateDescriptorSetLayout(RrLogicalDevice device,
	const RrDescriptorSetLayoutCreateInfo* pCreateInfo, RrDescriptorSetLayout* pLayout)
{
	return RrErrorSuccess;
}

RrError RrCreateDescriptorPool(
	RrLogicalDevice device, const RrDescriptorPoolCreateInfo* pCreateInfo, RrDescriptorPool* pPool)
{
	return RrErrorSuccess;
}

RrError RrAllocateDescriptorSets(RrLogicalDevice device,
	const RrDescriptorSetAllocateInfo* pAllocInfo, RrDescriptorSet* pDescriptorSets)
{
	return RrErrorSuccess;
}

void RrUpdateDescriptorSets(RrLogicalDevice device, uint32_t descriptorWriteCount,
	const RrWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount,
	const RrCopyDescriptorSet* pDescriptorCopies)
{
}

void RrDestroyDescriptorPool(RrLogicalDevice device, RrDescriptorPool pool) {}

void RrDestroyDescriptorSetLayout(RrLogicalDevice device, RrDescriptorSetLayout layout) {}


void RrCmdBindDescriptorSets(RrCommandBuffer commandBuffer, RrPipelineBindPoint pipelineBindPoint,
	RrPipelineLayout pipelineLayout, uint32_t firstSet, uint32_t descriptorSetCount,
	const RrDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount,
	const uint32_t* pDynamicOffsets)
{
}