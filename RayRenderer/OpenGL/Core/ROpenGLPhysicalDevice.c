#include "Rpch.h"

#include "../Core/RPhysicalDevice.h"
#include "../Core/RUtils.h"


RrError RrEnumeratePhysicalDevice(RrInstance instance,
	const RrPhysicalDeviceEnumerationInfo* pCreateInfo, RrPhysicalDevice* pPhysicalDevice)
{
	return RrErrorSuccess;
}

void RrGetPhysicalDeviceProperties(
	RrPhysicalDevice physicalDevice, RrPhysicalDeviceProperties* pProperties)
{
	// driver or api?
	const GLubyte* version = glGetString(GL_VERSION);

	const GLubyte* vendor = glGetString(GL_VENDOR);
	strcpy(pProperties->deviceName, glGetString(GL_RENDERER));
}

void RrGetPhysicalDeviceFeatures(
	RrPhysicalDevice physicalDevice, RrPhysicalDeviceFeatures* pFeatures)
{
}

void RrGetPhysicalDeviceMemoryProperties(
	RrPhysicalDevice physicalDevice, RrPhysicalDeviceMemoryProperties* pProperties)
{
}

void RrGetPhysicalDeviceFormatProperties(
	RrPhysicalDevice physicalDevice, RrFormat format, RrFormatProperties* pProperties)
{
}


void RrGetPhysicalDeviceSurfaceCapabilitiesKHR(RrPhysicalDevice physicalDevice,
	RrSurfaceKHR surface, RrSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
}

void RrGetPhysicalDeviceSurfaceFormatsKHR(RrPhysicalDevice physicalDevice, RrSurfaceKHR surface,
	uint32_t* pSurfaceFormatCount, RrSurfaceFormatKHR* pSurfaceFormats)
{
}

void RrGetPhysicalDeviceSurfacePresentModesKHR(RrPhysicalDevice physicalDevice,
	RrSurfaceKHR surface, uint32_t* pPresentModeCount, RrPresentModeKHR* pPresentModes)
{
}

void RrGetPhysicalDeviceQueueFamilyProperties(RrPhysicalDevice physicalDevice,
	uint32_t* pQueueFamilyPropertyCount, RrQueueFamilyProperties* pQueueFamilyProperties)
{
}

RrError RrGetPhysicalDeviceSurfaceSupportKHR(RrPhysicalDevice physicalDevice,
	uint32_t queueFamilyIndex, RrSurfaceKHR surface, RrBool32* pSupported)
{
	return RrErrorSuccess;
}
