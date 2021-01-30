#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class VulkanInstance;
	class PhysicalDevice;

	class RAY_EXPORT Surface : NonCopyable
	{
	public:
		/**
		 * Creates the surface to draw on
		 * @param instance The Vulkan instance
		 * @param physicalDevice The physical GPU handle
		 */
		Surface(const VulkanInstance* instance, const PhysicalDevice* physicalDevice);
		~Surface();

		/**
		 * Conversion operator to underlying type
		 */
		operator const VkSurfaceKHR&() const { return m_Surface; }

		const VkSurfaceCapabilitiesKHR& GetCapabilities() const { return m_Capabilities; }
		const VkSurfaceFormatKHR& GetFormat() const { return m_Format; }

	private:
		const VulkanInstance* m_Instance;
		const PhysicalDevice* m_PhysicalDevice;

		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
		VkSurfaceCapabilitiesKHR m_Capabilities{};
		VkSurfaceFormatKHR m_Format{};
	};
}  // namespace At0::Ray
