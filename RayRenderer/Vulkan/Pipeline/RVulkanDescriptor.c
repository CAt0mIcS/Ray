#include "Rpch.h"
#include <../Pipeline/RDescriptor.h>
#include <../Core/RUtils.h>


RrError RrCreateDescriptorSetLayout(RrLogicalDevice device,
	const RrDescriptorSetLayoutCreateInfo* pCreateInfo, RrDescriptorSetLayout* pLayout)
{
	VkDescriptorSetLayoutBinding* bindings =
		malloc(sizeof(VkDescriptorSetLayoutBinding) * pCreateInfo->bindingCount);
	for (uint32_t i = 0; i < pCreateInfo->bindingCount; ++i)
	{
		bindings[i].binding = pCreateInfo->pBindings[i].binding;
		bindings[i].descriptorType = pCreateInfo->pBindings[i].descriptorType;
		bindings[i].descriptorCount = pCreateInfo->pBindings[i].descriptorCount;
		bindings[i].stageFlags = pCreateInfo->pBindings[i].stageFlags;
		bindings[i].pImmutableSamplers =
			(const VkSampler*)pCreateInfo->pBindings[i].pImmutableSamplers;
	}

	VkDescriptorSetLayoutCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = pCreateInfo->flags;
	createInfo.bindingCount = pCreateInfo->bindingCount;
	createInfo.pBindings = bindings;

	RrError error = GetError(vkCreateDescriptorSetLayout(
		(VkDevice)device, &createInfo, NULL, (VkDescriptorSetLayout*)pLayout));

	free(bindings);
	return error;
}

RrError RrCreateDescriptorPool(
	RrLogicalDevice device, const RrDescriptorPoolCreateInfo* pCreateInfo, RrDescriptorPool* pPool)
{
	VkDescriptorPoolSize* poolSizes =
		malloc(sizeof(VkDescriptorPoolSize) * pCreateInfo->poolSizeCount);
	for (uint32_t i = 0; i < pCreateInfo->poolSizeCount; ++i)
	{
		poolSizes[i].type = pCreateInfo->pPoolSizes[i].type;
		poolSizes[i].descriptorCount = pCreateInfo->pPoolSizes[i].descriptorCount;
	}

	VkDescriptorPoolCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = pCreateInfo->flags;
	createInfo.maxSets = pCreateInfo->maxSets;
	createInfo.poolSizeCount = pCreateInfo->poolSizeCount;
	createInfo.pPoolSizes = poolSizes;

	RrError error = GetError(
		vkCreateDescriptorPool((VkDevice)device, &createInfo, NULL, (VkDescriptorPool*)pPool));

	free(poolSizes);
	return error;
}

RrError RrAllocateDescriptorSets(RrLogicalDevice device,
	const RrDescriptorSetAllocateInfo* pAllocInfo, RrDescriptorSet* pDescriptorSets)
{
	VkDescriptorSetAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.pNext = NULL;
	allocInfo.descriptorPool = (VkDescriptorPool)pAllocInfo->descriptorPool;
	allocInfo.descriptorSetCount = pAllocInfo->descriptorSetCount;
	allocInfo.pSetLayouts = (VkDescriptorSetLayout*)pAllocInfo->pSetLayouts;

	return GetError(
		vkAllocateDescriptorSets((VkDevice)device, &allocInfo, (VkDescriptorSet*)pDescriptorSets));
}

void RrUpdateDescriptorSets(RrLogicalDevice device, uint32_t descriptorWriteCount,
	const RrWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount,
	const RrCopyDescriptorSet* pDescriptorCopies)
{
	VkWriteDescriptorSet* writeDescs = malloc(sizeof(VkWriteDescriptorSet) * descriptorWriteCount);
	VkDescriptorImageInfo* imageInfos =
		malloc(sizeof(VkDescriptorImageInfo) * descriptorWriteCount);
	VkDescriptorBufferInfo* bufferInfos =
		malloc(sizeof(VkDescriptorBufferInfo) * descriptorWriteCount);
	memset(writeDescs, 0, sizeof(VkWriteDescriptorSet) * descriptorWriteCount);
	memset(imageInfos, 0, sizeof(VkDescriptorImageInfo) * descriptorWriteCount);
	memset(bufferInfos, 0, sizeof(VkDescriptorBufferInfo) * descriptorWriteCount);

	for (uint32_t i = 0; i < descriptorWriteCount; ++i)
	{
		writeDescs[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescs[i].pNext = NULL;
		writeDescs[i].dstSet = (VkDescriptorSet)pDescriptorWrites[i].dstSet;
		writeDescs[i].dstBinding = pDescriptorWrites[i].dstBinding;
		writeDescs[i].dstArrayElement = pDescriptorWrites[i].dstArrayElement;
		writeDescs[i].descriptorCount = pDescriptorWrites[i].descriptorCount;
		writeDescs[i].descriptorType = pDescriptorWrites[i].descriptorType;
		writeDescs[i].pTexelBufferView = (const VkBufferView*)pDescriptorWrites[i].pTexelBufferView;

		if (pDescriptorWrites[i].pImageInfo)
		{
			imageInfos[i].sampler = (VkSampler)pDescriptorWrites[i].pImageInfo->sampler;
			imageInfos[i].imageLayout = (VkImageLayout)pDescriptorWrites[i].pImageInfo->imageLayout;
			imageInfos[i].imageView = (VkImageView)pDescriptorWrites[i].pImageInfo->imageView;
			writeDescs[i].pImageInfo = (const VkDescriptorImageInfo*)(imageInfos + i);
		}
		if (pDescriptorWrites[i].pBufferInfo)
		{
			bufferInfos[i].buffer = (VkBuffer)pDescriptorWrites[i].pBufferInfo->buffer;
			bufferInfos[i].offset = pDescriptorWrites[i].pBufferInfo->offset;
			bufferInfos[i].range = pDescriptorWrites[i].pBufferInfo->range;
			writeDescs[i].pBufferInfo = (const VkDescriptorBufferInfo*)(bufferInfos + i);
		}
	}

	VkCopyDescriptorSet* descCopies = NULL;
	if (descriptorCopyCount != 0)
	{
		descCopies = malloc(sizeof(VkCopyDescriptorSet) * descriptorCopyCount);
		for (uint32_t i = 0; i < descriptorCopyCount; ++i)
		{
			descCopies[i].sType = VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET;
			descCopies[i].pNext = NULL;
			descCopies[i].srcSet = (VkDescriptorSet)pDescriptorCopies[i].srcSet;
			descCopies[i].srcBinding = pDescriptorCopies[i].srcBinding;
			descCopies[i].srcArrayElement = pDescriptorCopies[i].srcArrayElement;
			descCopies[i].dstSet = (VkDescriptorSet)pDescriptorCopies[i].dstSet;
			descCopies[i].dstBinding = pDescriptorCopies[i].dstBinding;
			descCopies[i].dstArrayElement = pDescriptorCopies[i].dstArrayElement;
			descCopies[i].descriptorCount = pDescriptorCopies[i].descriptorCount;
		}
	}

	vkUpdateDescriptorSets(
		(VkDevice)device, descriptorWriteCount, writeDescs, descriptorCopyCount, descCopies);

	free(writeDescs);
	free(imageInfos);
	free(bufferInfos);
	if (descriptorCopyCount != 0)
		free(descCopies);
}

void RrDestroyDescriptorPool(RrLogicalDevice device, RrDescriptorPool pool)
{
	vkDestroyDescriptorPool((VkDevice)device, (VkDescriptorPool)pool, NULL);
}

void RrDestroyDescriptorSetLayout(RrLogicalDevice device, RrDescriptorSetLayout layout)
{
	vkDestroyDescriptorSetLayout((VkDevice)device, (VkDescriptorSetLayout)layout, NULL);
}


void RrCmdBindDescriptorSets(RrCommandBuffer commandBuffer, RrPipelineBindPoint pipelineBindPoint,
	RrPipelineLayout pipelineLayout, uint32_t firstSet, uint32_t descriptorSetCount,
	const RrDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount,
	const uint32_t* pDynamicOffsets)
{
	vkCmdBindDescriptorSets((VkCommandBuffer)commandBuffer, (VkPipelineBindPoint)pipelineBindPoint,
		(VkPipelineLayout)pipelineLayout, firstSet, descriptorSetCount,
		(const VkDescriptorSet*)pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}
