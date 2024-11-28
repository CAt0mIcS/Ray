#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class VulkanInstance;

	class RAY_EXPORT PhysicalDevice : NonCopyable
	{
	public:
		PhysicalDevice(const VulkanInstance& instance);
		operator VkPhysicalDevice() const { return m_Device; }

		const VkPhysicalDeviceFeatures& GetFeatures() const { return m_Features; }
		const VkPhysicalDeviceMemoryProperties& GetMemoryProperties() const;
		const VkPhysicalDeviceProperties& GetProperties() const { return m_Properties; }

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

		bool IsFormatLinearlyFilterable(VkFormat format, VkImageTiling imageTiling) const;

		/**
		 * @returns If the GPU supports a combination of memory properties
		 */
		bool HasMemoryProperties(VkMemoryPropertyFlags memProps) const;

	private:
		VkPhysicalDevice ChoosePhysicalDevice(const std::vector<VkPhysicalDevice>& devices);
		int32_t ScorePhysicalDevice(VkPhysicalDevice device);

	private:
		VkPhysicalDevice m_Device = VK_NULL_HANDLE;

		VkPhysicalDeviceFeatures m_Features{};
		VkPhysicalDeviceMemoryProperties m_MemoryProperties{};
		VkPhysicalDeviceProperties m_Properties;
	};
}  // namespace At0::Ray
