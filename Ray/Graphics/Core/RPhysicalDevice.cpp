#include "Rpch.h"
#include "RPhysicalDevice.h"

#include "Graphics/RGraphics.h"
#include "RLogicalDevice.h"
#include "RRendererInstance.h"

#include "RayBase/RException.h"
#include "RayBase/RLogger.h"

#include "Ray/Core/RRendererLoader.h"


namespace At0::Ray
{
	PhysicalDevice::PhysicalDevice()
	{
		RrPhysicalDeviceEnumerationInfo enumInfo{};
		enumInfo.deviceExtensionCount = LogicalDevice::GetDeviceExtensions().size();
		enumInfo.ppDeviceExtensions = LogicalDevice::GetDeviceExtensions().data();

		ThrowRenderError(RendererAPI::EnumeratePhysicalDevice(Graphics::Get().GetInstance(),
							 &enumInfo, (RrPhysicalDevice*)&m_Device),
			"[PhysicalDevice] Failed to find suitable GPU.");

		vkGetPhysicalDeviceProperties(m_Device, &m_Properties);
		Log::Info("[PhysicalDevice] Graphics card info: ");
		Log::Info("[PhysicalDevice]\tAPI Version: {0}", m_Properties.apiVersion);
		Log::Info("[PhysicalDevice]\tDriver Version: {0}", m_Properties.driverVersion);
		Log::Info("[PhysicalDevice]\tVendor ID: {0}", m_Properties.vendorID);
		Log::Info("[PhysicalDevice]\tDevice ID: {0}", m_Properties.deviceID);
		Log::Info("[PhysicalDevice]\tDevice Type: {0}", String::Construct(m_Properties.deviceType));
		Log::Info("[PhysicalDevice]\tDevice Name: {0}", m_Properties.deviceName);

		vkGetPhysicalDeviceFeatures(m_Device, &m_Features);
		vkGetPhysicalDeviceMemoryProperties(m_Device, &m_MemoryProperties);
	}

	const VkPhysicalDeviceMemoryProperties& PhysicalDevice::GetMemoryProperties() const
	{
		return m_MemoryProperties;
	}

	uint32_t PhysicalDevice::FindMemoryType(
		uint32_t typeFilter, VkMemoryPropertyFlags properties) const
	{
		for (uint32_t i = 0; i < m_MemoryProperties.memoryTypeCount; ++i)
		{
			if (typeFilter & (1 << i) &&
				(m_MemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}

		ThrowRuntime("[PhysicalDevice] Failed to find suitable memory type");
		return 0;
	}

	bool PhysicalDevice::HasMemoryProperties(VkMemoryPropertyFlags memProps) const
	{
		for (uint32_t i = 0; i < m_MemoryProperties.memoryTypeCount; ++i)
		{
			if (m_MemoryProperties.memoryTypes[i].propertyFlags & memProps)
				return true;
		}

		return false;
	}
}  // namespace At0::Ray
