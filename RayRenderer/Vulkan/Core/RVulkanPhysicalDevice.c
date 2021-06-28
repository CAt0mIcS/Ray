#include "Rpch.h"
#include "../Core/RPhysicalDevice.h"

#include "../Core/RUtils.h"


VkPhysicalDevice ChoosePhysicalDevice(uint32_t deviceCount, VkPhysicalDevice* pDevices,
	uint32_t deviceExtensionCount, const char* const* ppDeviceExtensions);
int32_t ScorePhysicalDevice(
	VkPhysicalDevice device, uint32_t deviceExtensionCount, const char* const* ppDeviceExtensions);


RrError RrEnumeratePhysicalDevice(RrInstance pInstance,
	const RrPhysicalDeviceEnumerationInfo* const pCreateInfo, RrPhysicalDevice* ppPhysicalDevice)
{
	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(pInstance, &physicalDeviceCount, NULL);
	if (physicalDeviceCount == 0)
	{
		LogError("Failed to find suitable GPU supporting Vulkan while enumerating all physical "
				 "devices");
		return RrErrorIncompatibleDriver;
	}

	VkPhysicalDevice* physicalDevices = malloc(sizeof(VkPhysicalDevice) * physicalDeviceCount);
	vkEnumeratePhysicalDevices(pInstance, &physicalDeviceCount, physicalDevices);

	*ppPhysicalDevice = ChoosePhysicalDevice(physicalDeviceCount, physicalDevices,
		pCreateInfo->deviceExtensionCount, pCreateInfo->ppDeviceExtensions);
	if (!*ppPhysicalDevice)
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
	RrPhysicalDevice pDevice, RrPhysicalDeviceProperties* pProperties)
{
	vkGetPhysicalDeviceProperties(pDevice, (VkPhysicalDeviceProperties*)pProperties);
}

void RrGetPhysicalDeviceFeatures(RrPhysicalDevice pDevice, RrPhysicalDeviceFeatures* pFeatures)
{
	vkGetPhysicalDeviceFeatures(pDevice, (VkPhysicalDeviceFeatures*)pFeatures);
}

void RrGetPhysicalDeviceMemoryProperties(
	RrPhysicalDevice pDevice, RrPhysicalDeviceMemoryProperties* pProperties)
{
	vkGetPhysicalDeviceMemoryProperties(pDevice, (VkPhysicalDeviceMemoryProperties*)pProperties);
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

int32_t ScorePhysicalDevice(
	VkPhysicalDevice device, uint32_t deviceExtensionCount, const char* const* ppDeviceExtensions)
{
	int32_t score = 0;

	// Check if requested extensions are supported.
	uint32_t extensionPropCount;
	vkEnumerateDeviceExtensionProperties(device, NULL, &extensionPropCount, NULL);
	VkExtensionProperties* extensionProperties =
		malloc(sizeof(VkExtensionProperties) * extensionPropCount);
	vkEnumerateDeviceExtensionProperties(device, NULL, &extensionPropCount, extensionProperties);

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
	vkGetPhysicalDeviceProperties(device, &props);

	// Higher score for externally added GPU
	if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;

	score += props.limits.maxImageDimension2D;

	free(extensionProperties);
	return score;
}
