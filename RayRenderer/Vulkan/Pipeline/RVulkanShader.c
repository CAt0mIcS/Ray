#include "Rpch.h"
#include <../Pipeline/RShader.h>
#include <../Core/RUtils.h>


RrError RrCreateShaderModule(RrLogicalDevice device, const RrShaderModuleCreateInfo* pCreateInfo,
	RrShaderModule* pShaderModule)
{
	VkShaderModuleCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = 0;
	createInfo.codeSize = pCreateInfo->codeSize;
	createInfo.pCode = pCreateInfo->pCode;

	return GetError(
		vkCreateShaderModule((VkDevice)device, &createInfo, NULL, (VkShaderModule*)pShaderModule));
}
