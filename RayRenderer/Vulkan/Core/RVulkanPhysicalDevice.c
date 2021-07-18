#include "Rpch.h"
#include "../Core/RPhysicalDevice.h"

#include "../Core/RUtils.h"


VkPhysicalDevice ChoosePhysicalDevice(uint32_t deviceCount, VkPhysicalDevice* pDevices,
	uint32_t deviceExtensionCount, const char* const* ppDeviceExtensions);
int32_t ScorePhysicalDevice(VkPhysicalDevice physicalDevice, uint32_t deviceExtensionCount,
	const char* const* ppDeviceExtensions);


RrError RrEnumeratePhysicalDevice(RrInstance instance,
	const RrPhysicalDeviceEnumerationInfo* pCreateInfo, RrPhysicalDevice* pPhysicalDevice)
{
	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices((VkInstance)instance, &physicalDeviceCount, NULL);
	if (physicalDeviceCount == 0)
	{
		LogError("Failed to find suitable GPU supporting Vulkan while enumerating all physical "
				 "devices");
		return RrErrorIncompatibleDriver;
	}

	VkPhysicalDevice* physicalDevices = malloc(sizeof(VkPhysicalDevice) * physicalDeviceCount);
	vkEnumeratePhysicalDevices((VkInstance)instance, &physicalDeviceCount, physicalDevices);

	*pPhysicalDevice = (RrPhysicalDevice)ChoosePhysicalDevice(physicalDeviceCount, physicalDevices,
		pCreateInfo->deviceExtensionCount, pCreateInfo->ppDeviceExtensions);
	if (!*pPhysicalDevice)
	{
		if (LogError("Failed to find suitable physical device"))
		{
			free(physicalDevices);
			return RrErrorIncompatibleDriver;
		}
	}

	free(physicalDevices);
	return RrErrorNone;
}

void RrGetPhysicalDeviceProperties(
	RrPhysicalDevice physicalDevice, RrPhysicalDeviceProperties* pProperties)
{
	vkGetPhysicalDeviceProperties(
		(VkPhysicalDevice)physicalDevice, (VkPhysicalDeviceProperties*)pProperties);
}

void RrGetPhysicalDeviceFeatures(
	RrPhysicalDevice physicalDevice, RrPhysicalDeviceFeatures* pFeatures)
{
	vkGetPhysicalDeviceFeatures(
		(VkPhysicalDevice)physicalDevice, (VkPhysicalDeviceFeatures*)pFeatures);
}

void RrGetPhysicalDeviceMemoryProperties(
	RrPhysicalDevice physicalDevice, RrPhysicalDeviceMemoryProperties* pProperties)
{
	vkGetPhysicalDeviceMemoryProperties(
		(VkPhysicalDevice)physicalDevice, (VkPhysicalDeviceMemoryProperties*)pProperties);
}

void RrGetPhysicalDeviceFormatProperties(
	RrPhysicalDevice physicalDevice, RrFormat format, RrFormatProperties* pProperties)
{
	vkGetPhysicalDeviceFormatProperties(
		(VkPhysicalDevice)physicalDevice, (VkFormat)format, (VkFormatProperties*)pProperties);
}

void RrGetPhysicalDeviceSurfaceCapabilitiesKHR(RrPhysicalDevice physicalDevice,
	RrSurfaceKHR surface, RrSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		(VkPhysicalDevice)physicalDevice, (VkSurfaceKHR)surface, &surfaceCapabilities);

	pSurfaceCapabilities->minImageCount = surfaceCapabilities.minImageCount;
	pSurfaceCapabilities->maxImageCount = surfaceCapabilities.maxImageCount;
	pSurfaceCapabilities->currentExtent.width = surfaceCapabilities.currentExtent.width;
	pSurfaceCapabilities->currentExtent.height = surfaceCapabilities.currentExtent.height;
	pSurfaceCapabilities->minImageExtent.width = surfaceCapabilities.minImageExtent.width;
	pSurfaceCapabilities->minImageExtent.height = surfaceCapabilities.minImageExtent.height;
	pSurfaceCapabilities->maxImageExtent.width = surfaceCapabilities.maxImageExtent.width;
	pSurfaceCapabilities->maxImageExtent.height = surfaceCapabilities.maxImageExtent.height;
	pSurfaceCapabilities->maxImageArrayLayers = surfaceCapabilities.maxImageArrayLayers;
	pSurfaceCapabilities->supportedTransforms = surfaceCapabilities.supportedTransforms;
	pSurfaceCapabilities->currentTransform = surfaceCapabilities.currentTransform;
	pSurfaceCapabilities->supportedCompositeAlpha = surfaceCapabilities.supportedCompositeAlpha;
	pSurfaceCapabilities->supportedUsageFlags = surfaceCapabilities.supportedUsageFlags;
}

void RrGetPhysicalDeviceSurfaceFormatsKHR(RrPhysicalDevice physicalDevice, RrSurfaceKHR surface,
	uint32_t* pSurfaceFormatCount, RrSurfaceFormatKHR* pSurfaceFormats)
{
	if (pSurfaceFormats == NULL)
	{
		vkGetPhysicalDeviceSurfaceFormatsKHR(
			(VkPhysicalDevice)physicalDevice, (VkSurfaceKHR)surface, pSurfaceFormatCount, NULL);
	}
	else
	{
		VkSurfaceFormatKHR* surfaceFormats =
			malloc(sizeof(VkSurfaceFormatKHR) * *pSurfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR((VkPhysicalDevice)physicalDevice,
			(VkSurfaceKHR)surface, pSurfaceFormatCount, surfaceFormats);

		for (uint32_t i = 0; i < *pSurfaceFormatCount; ++i)
		{
			pSurfaceFormats[i].format = surfaceFormats[i].format;
			pSurfaceFormats[i].colorSpace = surfaceFormats[i].colorSpace;
		}
		free(surfaceFormats);
	}
}

void RrGetPhysicalDeviceSurfacePresentModesKHR(RrPhysicalDevice physicalDevice,
	RrSurfaceKHR surface, uint32_t* pPresentModeCount, RrPresentModeKHR* pPresentModes)
{
	vkGetPhysicalDeviceSurfacePresentModesKHR((VkPhysicalDevice)physicalDevice,
		(VkSurfaceKHR)surface, pPresentModeCount, (VkPresentModeKHR*)pPresentModes);
}


VkPhysicalDevice ChoosePhysicalDevice(uint32_t deviceCount, VkPhysicalDevice* pDevices,
	uint32_t deviceExtensionCount, const char* const* ppDeviceExtensions)
{
	int32_t* scores = malloc(sizeof(int32_t) * deviceCount);
	memset(scores, 0, sizeof(int32_t) * deviceCount);
	VkPhysicalDevice bestDevice = NULL;

	for (int32_t i = 0; i < deviceCount; ++i)
		scores[i] = ScorePhysicalDevice(pDevices[i], deviceExtensionCount, ppDeviceExtensions);

	int32_t lowestScore = INT32_MIN;
	for (uint32_t i = 0; i < deviceCount; ++i)
	{
		if (scores[i] >= lowestScore)
		{
			lowestScore = scores[i];
			bestDevice = pDevices[i];
		}
	}

	free(scores);
	return bestDevice;
}

int32_t ScorePhysicalDevice(VkPhysicalDevice physicalDevice, uint32_t deviceExtensionCount,
	const char* const* ppDeviceExtensions)
{
	int32_t score = 0;

	// Check if requested extensions are supported.
	uint32_t extensionPropCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionPropCount, NULL);
	VkExtensionProperties* extensionProperties =
		malloc(sizeof(VkExtensionProperties) * extensionPropCount);
	vkEnumerateDeviceExtensionProperties(
		physicalDevice, NULL, &extensionPropCount, extensionProperties);

	for (uint32_t i = 0; i < deviceExtensionCount; ++i)
	{
		bool extFound = false;

		// Checks if the extension is in the available extensions.
		for (uint32_t j = 0; j < extensionPropCount; ++j)
		{
			if (strcmp(ppDeviceExtensions[i], extensionProperties[j].extensionName) == 0)
			{
				extFound = true;
				break;
			}
		}

		// Lowest score for devices that don't support a specific extension
		if (!extFound)
			score = INT32_MIN;
	}

	VkPhysicalDeviceProperties props;
	vkGetPhysicalDeviceProperties(physicalDevice, &props);

	// Higher score for externally added GPU
	if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;

	score += props.limits.maxImageDimension2D;

	free(extensionProperties);
	return score;
}
