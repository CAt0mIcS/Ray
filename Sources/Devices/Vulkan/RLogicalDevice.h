#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vector>
#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class VulkanInstance;
	class PhysicalDevice;
	class Surface;

	class RAY_EXPORT LogicalDevice : NonCopyable
	{
	public:
		/**
		 * Establishes the connection to the gpu
		 * @param instance The Vulkan Instance
		 * @param physicalDevice The physical GPU handle
		 * @param surface The render target
		 */
		LogicalDevice(const VulkanInstance* instance, const PhysicalDevice* physicalDevice,
			const Surface* surface);
		~LogicalDevice();

		/**
		 * Conversion operator to the underlying type
		 */
		operator const VkDevice&() const { return m_LogicalDevice; }

		/**
		 * @returns The device extensions in use
		 */
		static const std::vector<const char*>& DeviceExtensions() { return s_DeviceExtensions; }

	private:
		void CreateQueueIndices();
		void CreateLogicalDevice();

		const VulkanInstance* m_Instance;
		const PhysicalDevice* m_PhysicalDevice;
		const Surface* m_Surface;

		VkDevice m_LogicalDevice = VK_NULL_HANDLE;
		VkPhysicalDeviceFeatures m_EnabledFeatures{};

		VkQueueFlags m_SupportedQueues{};
		uint32_t m_GraphicsFamily = 0;
		uint32_t m_PresentFamily = 0;
		uint32_t m_ComputeFamily = 0;
		uint32_t m_TransferFamily = 0;

		VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
		VkQueue m_PresentQueue = VK_NULL_HANDLE;
		VkQueue m_ComputeQueue = VK_NULL_HANDLE;
		VkQueue m_TransferQueue = VK_NULL_HANDLE;

		static const std::vector<const char*> s_DeviceExtensions;
	};
}  // namespace At0::Ray
