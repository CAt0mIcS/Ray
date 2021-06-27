#include "Rpch.h"
#include "RPhysicalDevice.h"

#include "Graphics/RGraphics.h"
#include "RLogicalDevice.h"
#include "RRendererInstance.h"

#include "RayBase/RException.h"
#include "RayBase/RLogger.h"


namespace At0::Ray
{
	PhysicalDevice::PhysicalDevice()
	{
		uint32_t physicalDeviceCount = 0;
		vkEnumeratePhysicalDevices(Graphics::Get().GetInstance(), &physicalDeviceCount, nullptr);
		if (physicalDeviceCount == 0)
			ThrowRuntime("[PhysicalDevice] Failed to find suitable GPU supporting Vulkan");

		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		vkEnumeratePhysicalDevices(
			Graphics::Get().GetInstance(), &physicalDeviceCount, physicalDevices.data());

		m_Device = ChoosePhysicalDevice(physicalDevices);
		if (!m_Device)
			ThrowRuntime("[RendererInstance] Failed to find suitable GPU");

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

	VkPhysicalDevice PhysicalDevice::ChoosePhysicalDevice(
		const std::vector<VkPhysicalDevice>& devices)
	{
		std::multimap<int32_t, VkPhysicalDevice> scoreMap;

		for (VkPhysicalDevice device : devices)
			scoreMap.emplace(ScorePhysicalDevice(device), device);

		if (scoreMap.size() == 0)
			return nullptr;

		if (scoreMap.rbegin()->first > 0)
			return scoreMap.rbegin()->second;

		return nullptr;
	}

	int32_t PhysicalDevice::ScorePhysicalDevice(VkPhysicalDevice device)
	{
		int32_t score = 0;

		// CHeck if requested extensions are supported.
		uint32_t extensionPropCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropCount, nullptr);
		std::vector<VkExtensionProperties> extensionProperties(extensionPropCount);
		vkEnumerateDeviceExtensionProperties(
			device, nullptr, &extensionPropCount, extensionProperties.data());

		for (const char* extName : LogicalDevice::GetDeviceExtensions())
		{
			bool extFound = false;

			// Checks if the extension is in the available extensions.
			for (const auto& extension : extensionProperties)
			{
				if (strcmp(extName, extension.extensionName) == 0)
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

		return score;
	}
}  // namespace At0::Ray
