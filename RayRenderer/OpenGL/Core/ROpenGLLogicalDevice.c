#include "Rpch.h"

#include "../Core/RLogicalDevice.h"
#include "../Core/RUtils.h"


RrError RrCreateLogicalDevice(RrPhysicalDevice physicalDevice,
	RrLogicalDeviceCreateInfo* pCreateInfo, RrLogicalDevice* pDevice)
{
	return RrErrorSuccess;
}

void RrGetDeviceQueue(
	RrLogicalDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, RrQueue* pQueue)
{
}

void RrDestroyLogicalDevice(RrLogicalDevice device) {}

RrPFNVoidFunction RrGetDeviceProcAddr(RrLogicalDevice device, const char* pName)
{
	return NULL;
}

RrError RrDeviceWaitIdle(RrLogicalDevice device)
{
	return RrErrorSuccess;
}