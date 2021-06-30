#include "Rpch.h"

#include <../Core/RLogicalDevice.h>
#include <../Core/RUtils.h>

RrError RrCreateLogicalDevice(RrPhysicalDevice physicalDevice,
	RrLogicalDeviceCreateInfo* pCreateInfo, RrLogicalDevice* pDevice)
{
	VkDeviceQueueCreateInfo* queueCreateInfos =
		malloc(pCreateInfo->queueCreateInfoCount * sizeof(VkDeviceQueueCreateInfo));
	for (uint32_t i = 0; i < pCreateInfo->queueCreateInfoCount; ++i)
	{
		queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfos[i].pNext = NULL;
		queueCreateInfos[i].flags = 0;
		queueCreateInfos[i].queueCount = pCreateInfo->pQueueCreateInfos[i].queueCount;
		queueCreateInfos[i].queueFamilyIndex = pCreateInfo->pQueueCreateInfos[i].queueFamilyIndex;
		queueCreateInfos[i].pQueuePriorities = pCreateInfo->pQueueCreateInfos[i].pQueuePriorities;
	}

	VkDeviceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = 0;
	createInfo.queueCreateInfoCount = pCreateInfo->queueCreateInfoCount;
	createInfo.pQueueCreateInfos = queueCreateInfos;
	createInfo.enabledLayerCount = pCreateInfo->enabledLayerCount;
	createInfo.ppEnabledLayerNames = pCreateInfo->ppEnabledLayerNames;
	createInfo.enabledExtensionCount = pCreateInfo->enabledExtensionCount;
	createInfo.ppEnabledExtensionNames = pCreateInfo->ppEnabledExtensionNames;
	createInfo.pEnabledFeatures = (const VkPhysicalDeviceFeatures*)pCreateInfo->pEnabledFeatures;

	RrError error = GetError(
		vkCreateDevice((VkPhysicalDevice)physicalDevice, &createInfo, NULL, (VkDevice*)pDevice));

	free(queueCreateInfos);
	return error;
}


void RrGetDeviceQueue(
	RrLogicalDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, RrQueue* pQueue)
{
	vkGetDeviceQueue((VkDevice)device, queueFamilyIndex, queueIndex, (VkQueue*)pQueue);
}

void RrDestroyLogicalDevice(RrLogicalDevice device)
{
	vkDestroyDevice((VkDevice)device, NULL);
}

RrPFNVoidFunction RrGetDeviceProcAddr(RrLogicalDevice device, const char* pName)
{
	return vkGetDeviceProcAddr((VkDevice)device, pName);
}

RrError RrDeviceWaitIdle(RrLogicalDevice device)
{
	return GetError(vkDeviceWaitIdle((VkDevice)device));
}
