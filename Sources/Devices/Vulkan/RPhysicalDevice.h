#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>
#include <vector>


namespace At0::Ray
{
	class VulkanInstance;

	class RAY_EXPORT PhysicalDevice : NonCopyable
	{
	public:
		/**
		 * Finds the best GPU in the system
		 * @param instance The Vulkan Instance
		 */
		PhysicalDevice(const VulkanInstance* instance);

		/**
		 * Conversion operator to underlying type
		 */
		operator const VkPhysicalDevice&() const { return m_PhysicalDevice; }

		/**
		 * @returns The available device features
		 */
		VkPhysicalDeviceFeatures GetFeatures() const { return m_Features; }

	private:
		VkPhysicalDevice ChoosePhysicalDevice(const std::vector<VkPhysicalDevice>& devices);
		static uint32_t ScorePhysicalDevice(const VkPhysicalDevice& device);
		VkSampleCountFlagBits GetMaxUsableSampleCount() const;

	private:
		const VulkanInstance* m_Instance = nullptr;

		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkPhysicalDeviceProperties m_Properties{};
		VkPhysicalDeviceFeatures m_Features{};
		VkPhysicalDeviceMemoryProperties m_MemoryProperties{};
		VkSampleCountFlagBits m_MsaaSamples = VK_SAMPLE_COUNT_1_BIT;
	};
}  // namespace At0::Ray
