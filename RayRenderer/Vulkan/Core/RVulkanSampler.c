#include "Rpch.h"
#include <../Core/RSampler.h>
#include <../Core/RUtils.h>

RrError RrCreateSampler(
	RrLogicalDevice device, const RrSamplerCreateInfo* pCreateInfo, RrSampler* pSampler)
{
	VkSamplerCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = pCreateInfo->flags;
	createInfo.magFilter = pCreateInfo->magFilter;
	createInfo.minFilter = pCreateInfo->minFilter;
	createInfo.mipmapMode = pCreateInfo->mipmapMode;
	createInfo.addressModeU = pCreateInfo->addressModeU;
	createInfo.addressModeV = pCreateInfo->addressModeV;
	createInfo.addressModeW = pCreateInfo->addressModeW;
	createInfo.mipLodBias = pCreateInfo->mipLodBias;
	createInfo.anisotropyEnable = pCreateInfo->anisotropyEnable;
	createInfo.maxAnisotropy = pCreateInfo->maxAnisotropy;
	createInfo.compareEnable = pCreateInfo->compareEnable;
	createInfo.compareOp = pCreateInfo->compareOp;
	createInfo.minLod = pCreateInfo->minLod;
	createInfo.maxLod = pCreateInfo->maxLod;
	createInfo.borderColor = pCreateInfo->borderColor;
	createInfo.unnormalizedCoordinates = pCreateInfo->unnormalizedCoordinates;

	return GetError(vkCreateSampler((VkDevice)device, &createInfo, NULL, (VkSampler*)pSampler));
}

void RrDestroySampler(RrLogicalDevice device, RrSampler sampler)
{
	vkDestroySampler((VkDevice)device, (VkSampler)sampler, NULL);
}
