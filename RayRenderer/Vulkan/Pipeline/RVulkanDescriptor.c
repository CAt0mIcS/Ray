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

void RrDestroyDescriptorPool(RrLogicalDevice device, RrDescriptorPool pool)
{
	vkDestroyDescriptorPool((VkDevice)device, (VkDescriptorPool)pool, NULL);
}

void RrDestroyDescriptorSetLayout(RrLogicalDevice device, RrDescriptorSetLayout layout)
{
	vkDestroyDescriptorSetLayout((VkDevice)device, (VkDescriptorSetLayout)layout, NULL);
}
