#include "Rpch.h"

#include "Core/RCore.h"
#include "Core/RUtils.h"


#if RR_RENDERER_API == Vulkan

RrError RrInitialize(const RrInitializeInfo* pInitInfo, RrInstance* pInstance)
{
	VkInstanceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.flags = 0;
	createInfo.pApplicationInfo = NULL;

	createInfo.enabledLayerCount = pInitInfo->enabledLayerCount;
	createInfo.ppEnabledLayerNames = pInitInfo->ppEnabledLayers;
	createInfo.enabledExtensionCount = pInitInfo->enabledExtensionCount;
	createInfo.ppEnabledExtensionNames = pInitInfo->ppEnabledExtensions;

	createInfo.pNext = pInitInfo->pNext;

	return RrGetError(vkCreateInstance(&createInfo, NULL, (VkInstance*)pInstance));
}

#endif
