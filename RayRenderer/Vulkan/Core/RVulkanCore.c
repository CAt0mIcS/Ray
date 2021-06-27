#include "Rpch.h"

#include <RayRenderer/Core/RCore.h>
#include <RayRenderer/Core/RUtils.h>

extern bool (*RrpfnValidationCallback)(RrLogMessageSeverity, const char*);


static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: return LogDebug(pCallbackData->pMessage);
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: return LogInfo(pCallbackData->pMessage);
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		return LogWarning(pCallbackData->pMessage);
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: return LogError(pCallbackData->pMessage);
	}

	return VK_FALSE;
}


bool RrHasValidationLayers(uint32_t enabledLayerCount, const char* const* ppEnabledLayers);

RrError RrInitialize(
	RrInitializeInfo* const pInitInfo, RrInstance* pInstance, RrDebugMessenger* pDebugMessenger)
{
	RrpfnValidationCallback = pInitInfo->pfnValidationCallback;

	VkInstanceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.flags = 0;
	createInfo.pApplicationInfo = NULL;

	createInfo.enabledLayerCount = pInitInfo->enabledLayerCount;
	createInfo.ppEnabledLayerNames = pInitInfo->ppEnabledLayers;
	createInfo.enabledExtensionCount = pInitInfo->enabledExtensionCount;
	createInfo.ppEnabledExtensionNames = pInitInfo->ppEnabledExtensions;

	VkDebugUtilsMessengerCreateInfoEXT debugUtilsCreateInfo;
	debugUtilsCreateInfo.pNext = NULL;
	debugUtilsCreateInfo.pUserData = NULL;
	debugUtilsCreateInfo.flags = 0;
	debugUtilsCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilsCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
										   VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
										   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
										   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugUtilsCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
									   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
									   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugUtilsCreateInfo.pfnUserCallback = &DebugCallback;

	if (pInitInfo->enableValidationLayers &&
		RrHasValidationLayers(pInitInfo->enabledLayerCount, pInitInfo->ppEnabledLayers))
		createInfo.pNext = pInitInfo->pNext;
	else
	{
		LogInfo(
			"pInitInfo->enableValidationLayers: Validation layers disabled or not supported. False "
			"written to pInitInfo->enableValidationLayers.");
		pInitInfo->enableValidationLayers = false;
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = NULL;
	}

	VkResult error = vkCreateInstance(&createInfo, NULL, (VkInstance*)pInstance);
	if (error != VK_SUCCESS)
		return RrGetError(error);

	if (pInitInfo->enableValidationLayers)
	{
		PFN_vkCreateDebugUtilsMessengerEXT createDebugMessenger =
			(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
				(VkInstance)*pInstance, "vkCreateDebugUtilsMessengerEXT");
		if (createDebugMessenger == NULL)
		{
			LogError("Failed to get function \"vkCreateDebugUtilsMessengerEXT\" to create debug "
					 "messenger");
			goto debugMessengerError;
		}

		if (createDebugMessenger(*pInstance, &debugUtilsCreateInfo, NULL,
				(VkDebugUtilsMessengerEXT*)pDebugMessenger) != VK_SUCCESS)
		{
			LogError("Failed to create debug messenger");
			goto debugMessengerError;
		}
	}
	else
		*pDebugMessenger = NULL;

	return RrErrorNone;

debugMessengerError:
	*pDebugMessenger = NULL;
	pInitInfo->enableValidationLayers = false;
	return RrErrorLayerNotPresent;
}


bool RrHasValidationLayers(uint32_t enabledLayerCount, const char* const* ppEnabledLayers)
{
	uint32_t layerPropCount = 0;
	vkEnumerateInstanceLayerProperties(&layerPropCount, NULL);
	if (layerPropCount == 0)
		return false;

	VkLayerProperties* layerProps = malloc(layerPropCount * sizeof(VkLayerProperties));
	vkEnumerateInstanceLayerProperties(&layerPropCount, layerProps);

	for (uint32_t i = 0; i < enabledLayerCount; ++i)
		for (uint32_t j = 0; i < layerPropCount; ++j)
			if (strcmp(ppEnabledLayers[i], layerProps[j].layerName) == 0)
			{
				free(layerProps);
				return true;
			}

	free(layerProps);

	return false;
}


void RrDestroyInstance(RrInstance pInstance, RrDebugMessenger pDebugMessenger)
{
	if (pDebugMessenger)
	{
		PFN_vkDestroyDebugUtilsMessengerEXT destroyDebugMessenger =
			(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
				pInstance, "vkDestroyDebugUtilsMessengerEXT");
		destroyDebugMessenger(pInstance, pDebugMessenger, NULL);
	}

	vkDestroyInstance(pInstance, NULL);
}
