#include "Rpch.h"
#include "RPhysicalDevice.h"
#include "RInstance.h"
#include "RLogicalDevice.h"

#include "Debug/RLogger.h"
#include "Debug/RAssert.h"
#include "Debug/RException.h"

#include <vulkan/vulkan.h>


namespace At0::Ray
{
	static const std::vector<VkSampleCountFlagBits> g_StageFlagBits = { VK_SAMPLE_COUNT_64_BIT,
		VK_SAMPLE_COUNT_32_BIT, VK_SAMPLE_COUNT_16_BIT, VK_SAMPLE_COUNT_8_BIT,
		VK_SAMPLE_COUNT_4_BIT, VK_SAMPLE_COUNT_2_BIT };

	PhysicalDevice::PhysicalDevice(const VulkanInstance* instance) : m_Instance{ instance }
	{
		// Get the number of available physical devices
		uint32_t physicalDeviceCount;
		vkEnumeratePhysicalDevices(*m_Instance, &physicalDeviceCount, nullptr);

		// Allocate memory for the physical devices and fill the buffer
		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		vkEnumeratePhysicalDevices(*m_Instance, &physicalDeviceCount, physicalDevices.data());

		m_PhysicalDevice = ChoosePhysicalDevice(physicalDevices);
		if (!m_PhysicalDevice)
			RAY_THROW_RUNTIME("[VulkanPhysicalDevice] Failed to find suitable GPU.");

		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_Properties);
		vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &m_Features);
		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &m_MemoryProperties);
		m_MsaaSamples = GetMaxUsableSampleCount();

		Log::Info("[PhysicalDevice] Selected Physical Device {0}: {1}", m_Properties.deviceID,
			m_Properties.deviceName);
	}

	VkPhysicalDevice PhysicalDevice::ChoosePhysicalDevice(
		const std::vector<VkPhysicalDevice>& devices)
	{
		// Sorted by rank
		std::multimap<uint32_t, VkPhysicalDevice> rankedDevices;

		// Rate device suitability
		for (const VkPhysicalDevice& device : devices)
			rankedDevices.emplace(ScorePhysicalDevice(device), device);

		// Checks to make sure the best candidate scored higher than 0
		if (rankedDevices.rbegin()->first > 0)
			return rankedDevices.rbegin()->second;

		return nullptr;
	}

	uint32_t PhysicalDevice::ScorePhysicalDevice(const VkPhysicalDevice& device)
	{
		uint32_t score = 0;

		// Check if requrested extensions are supported.
		uint32_t extensionPropCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropCount, nullptr);
		std::vector<VkExtensionProperties> extensionProperties(extensionPropCount);
		vkEnumerateDeviceExtensionProperties(
			device, nullptr, &extensionPropCount, extensionProperties.data());

		// Iterate through all requested extensions.
		for (const char* currentExt : LogicalDevice::DeviceExtensions())
		{
			bool extFound = false;

			// Checks if the extension is in the available extensions.
			for (const auto& extension : extensionProperties)
			{
				if (strcmp(currentExt, extension.extensionName) == 0)
				{
					extFound = true;
					break;
				}
			}

			// Returns a score of 0 if this device is missing a required extension.
			if (!extFound)
				return 0;
		}

		// Get device features and properties of the current device being rated.
		VkPhysicalDeviceProperties deviceProps;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProps);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		// Increase score of dedicated GPU
		if (deviceProps.deviceType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 1000;

		// Increase score for devices with higher max texture size
		score += deviceProps.limits.maxImageDimension2D;
		return score;
	}

	VkSampleCountFlagBits PhysicalDevice::GetMaxUsableSampleCount() const
	{
		VkPhysicalDeviceProperties deviceProps;
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &deviceProps);

		VkSampleCountFlags counts = std::min(deviceProps.limits.framebufferColorSampleCounts,
			deviceProps.limits.framebufferDepthSampleCounts);

		for (const auto& sampleFlag : g_StageFlagBits)
		{
			if (counts & sampleFlag)
				return sampleFlag;
		}

		return VK_SAMPLE_COUNT_1_BIT;
	}

}  // namespace At0::Ray
