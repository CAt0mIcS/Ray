#include "Rpch.h"
#include "RPhysicalDevice.h"

#include "Graphics/RGraphics.h"
#include "RLogicalDevice.h"
#include "RVulkanInstance.h"

#include "Utils/RException.h"


namespace At0::Ray
{
	PhysicalDevice::PhysicalDevice()
	{
		uint32_t physicalDeviceCount = 0;
		vkEnumeratePhysicalDevices(Graphics::Get().GetInstance(), &physicalDeviceCount, nullptr);
		if (physicalDeviceCount == 0)
			RAY_THROW_RUNTIME("[PhysicalDevice] Failed to find suitable GPU supporting Vulkan.");

		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		vkEnumeratePhysicalDevices(
			Graphics::Get().GetInstance(), &physicalDeviceCount, physicalDevices.data());

		m_Device = ChoosePhysicalDevice(physicalDevices);
		if (!m_Device)
			RAY_THROW_RUNTIME("[VulkanInstance] Failed to find suitable GPU");

		vkGetPhysicalDeviceFeatures(m_Device, &m_Features);
	}

	VkPhysicalDevice PhysicalDevice::ChoosePhysicalDevice(
		const std::vector<VkPhysicalDevice>& devices)
	{
		std::multimap<int32_t, VkPhysicalDevice> scoreMap;

		for (VkPhysicalDevice device : devices)
			scoreMap.emplace(ScorePhysicalDevice(device), device);

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