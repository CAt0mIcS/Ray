#pragma once

#include "../../RBase.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RAY_EXPORT PhysicalDevice
	{
	public:
		PhysicalDevice();
		operator const VkPhysicalDevice&() const { return m_Device; }

		const VkPhysicalDeviceFeatures& GetFeatures() const { return m_Features; }

	private:
		VkPhysicalDevice ChoosePhysicalDevice(const std::vector<VkPhysicalDevice>& devices);
		int32_t ScorePhysicalDevice(VkPhysicalDevice device);

	private:
		VkPhysicalDevice m_Device;

		VkPhysicalDeviceFeatures m_Features;
	};
}  // namespace At0::Ray
