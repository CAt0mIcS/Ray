#include "Rpch.h"
#include "RLogicalDevice.h"
#include "RInstance.h"
#include "RPhysicalDevice.h"
#include "RSurface.h"
#include "Debug/RException.h"

#include "Debug/RLogger.h"

#include <vulkan/vulkan.h>


namespace At0::Ray
{
	const std::vector<const char*> LogicalDevice::s_DeviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};	// VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME


	LogicalDevice::LogicalDevice(const VulkanInstance* instance,
		const PhysicalDevice* physicalDevice, const Surface* surface)
		: m_Instance{ instance }, m_PhysicalDevice{ physicalDevice }, m_Surface{ surface }
	{
		CreateQueueIndices();
		CreateLogicalDevice();
	}

	LogicalDevice::~LogicalDevice()
	{
		if (!vkDeviceWaitIdle(m_LogicalDevice))
			Log::Error("[~LogicalDevice] Failed to wait for logical device.");
		vkDestroyDevice(m_LogicalDevice, nullptr);
	}

	void LogicalDevice::CreateQueueIndices()
	{
		// Query queue families
		uint32_t familyPropCount;
		vkGetPhysicalDeviceQueueFamilyProperties(*m_PhysicalDevice, &familyPropCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilyProperties(familyPropCount);
		vkGetPhysicalDeviceQueueFamilyProperties(
			*m_PhysicalDevice, &familyPropCount, queueFamilyProperties.data());

		std::optional<uint32_t> graphicsFamily, presentFamily, computeFamily, transferFamily;

		for (uint32_t i = 0; i < familyPropCount; ++i)
		{
			// Check for graphics support.
			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphicsFamily = i;
				m_GraphicsFamily = i;
				m_SupportedQueues |= VK_QUEUE_GRAPHICS_BIT;
			}

			// Check for presentation supoprt.
			VkBool32 presentSupport;
			vkGetPhysicalDeviceSurfaceSupportKHR(*m_PhysicalDevice, i, *m_Surface, &presentSupport);

			if (queueFamilyProperties[i].queueCount > 0 && presentSupport)
			{
				presentFamily = i;
				m_PresentFamily = i;
			}

			// Check for compute support
			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				computeFamily = i;
				m_ComputeFamily = i;
				m_SupportedQueues |= VK_QUEUE_COMPUTE_BIT;
			}

			// Check for transfer support
			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				transferFamily = i;
				m_TransferFamily = i;
				m_SupportedQueues |= VK_QUEUE_TRANSFER_BIT;
			}

			// Search until all queues are found
			if (graphicsFamily && presentFamily && computeFamily && transferFamily)
				break;
		}

		// We need a graphics queue
		if (!graphicsFamily)
			RAY_THROW_RUNTIME("[VulkanLogicalDevice] Failed to find a graphics family.");
	}

	void LogicalDevice::CreateLogicalDevice()
	{
		auto physicalDeviceFeatures = m_PhysicalDevice->GetFeatures();

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		float queuePriorities[] = { 0.0f };

		// Fill vector with correct create info for the queues.
		if (m_SupportedQueues & VK_QUEUE_GRAPHICS_BIT)
		{
			VkDeviceQueueCreateInfo graphicsQueueCreateInfo{};
			graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			graphicsQueueCreateInfo.queueFamilyIndex = m_GraphicsFamily;
			graphicsQueueCreateInfo.queueCount = 1;
			graphicsQueueCreateInfo.pQueuePriorities = queuePriorities;
			queueCreateInfos.push_back(std::move(graphicsQueueCreateInfo));
		}
		else
			m_GraphicsFamily = 0;

		if (m_SupportedQueues & VK_QUEUE_COMPUTE_BIT && m_ComputeFamily != m_GraphicsFamily)
		{
			VkDeviceQueueCreateInfo computeQueueCreateInfo{};
			computeQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			computeQueueCreateInfo.queueFamilyIndex = m_ComputeFamily;
			computeQueueCreateInfo.queueCount = 1;
			computeQueueCreateInfo.pQueuePriorities = queuePriorities;
			queueCreateInfos.push_back(std::move(computeQueueCreateInfo));
		}
		else
			m_ComputeFamily = m_GraphicsFamily;

		if (m_SupportedQueues & VK_QUEUE_TRANSFER_BIT && m_TransferFamily != m_GraphicsFamily &&
			m_TransferFamily != m_ComputeFamily)
		{
			VkDeviceQueueCreateInfo transferQueueCreateInfo{};
			transferQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			transferQueueCreateInfo.queueFamilyIndex = m_TransferFamily;
			transferQueueCreateInfo.queueCount = 1;
			transferQueueCreateInfo.pQueuePriorities = queuePriorities;
			queueCreateInfos.push_back(std::move(transferQueueCreateInfo));
		}
		else
			m_TransferFamily = m_GraphicsFamily;

		VkPhysicalDeviceFeatures enabledFeatures{};

		// Enable sample rate shading filtering if supported.
		if (physicalDeviceFeatures.sampleRateShading)
			enabledFeatures.sampleRateShading = VK_TRUE;

		// Fill mode non solid is required for wireframe display.
		if (physicalDeviceFeatures.fillModeNonSolid)
			enabledFeatures.fillModeNonSolid = VK_TRUE;

		// Wide lines must be present for line width > 1.0f.
		if (physicalDeviceFeatures.wideLines)
			enabledFeatures.wideLines = VK_TRUE;
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support wireframe pipelines.");

		if (physicalDeviceFeatures.samplerAnisotropy)
			enabledFeatures.samplerAnisotropy = VK_TRUE;
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support sampler anisotropy.");

		if (physicalDeviceFeatures.textureCompressionBC)
			enabledFeatures.textureCompressionBC = VK_TRUE;
		else if (physicalDeviceFeatures.textureCompressionASTC_LDR)
			enabledFeatures.textureCompressionASTC_LDR = VK_TRUE;
		else if (physicalDeviceFeatures.textureCompressionETC2)
			enabledFeatures.textureCompressionETC2 = VK_TRUE;

		if (physicalDeviceFeatures.vertexPipelineStoresAndAtomics)
			enabledFeatures.vertexPipelineStoresAndAtomics = VK_TRUE;
		else
			Log::Warn(
				"[LogicalDevice] Selected GPU does not support vertex pipeline stores and atomics");

		if (physicalDeviceFeatures.fragmentStoresAndAtomics)
			enabledFeatures.fragmentStoresAndAtomics = VK_TRUE;
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support fragment stores and atomics.");

		if (physicalDeviceFeatures.shaderStorageImageExtendedFormats)
			enabledFeatures.shaderStorageImageExtendedFormats = VK_TRUE;
		else
			Log::Warn(
				"[LogicalDevice] Selected GPU does not support shader storage extended formats.");

		if (physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat)
			enabledFeatures.shaderStorageImageWriteWithoutFormat = VK_TRUE;
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support shader storage write without "
					  "format.");

		if (physicalDeviceFeatures.geometryShader)
			enabledFeatures.geometryShader = VK_TRUE;
		else
			Log::Warn("[LogicalDeivce] Selected GPU does not support geometry shaders.");

		if (physicalDeviceFeatures.tessellationShader)
			enabledFeatures.tessellationShader = VK_TRUE;
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support tesselation shaders.");

		if (physicalDeviceFeatures.multiViewport)
			enabledFeatures.multiViewport = VK_TRUE;
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support multiple viewports.");

		// Finally create the logical device
		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

		if (m_Instance->ValidationLayersEnabled())
		{
			deviceCreateInfo.enabledLayerCount =
				(uint32_t)VulkanInstance::ValidationLayers().size();
			deviceCreateInfo.ppEnabledLayerNames = VulkanInstance::ValidationLayers().data();
		}

		deviceCreateInfo.enabledExtensionCount = (uint32_t)DeviceExtensions().size();
		deviceCreateInfo.ppEnabledExtensionNames = DeviceExtensions().data();
		deviceCreateInfo.pEnabledFeatures = &enabledFeatures;
		RAY_VK_THROW_FAILED(
			vkCreateDevice(*m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_LogicalDevice),
			"[VulkanLogicalDevice] Failed to create logical device.");

		vkGetDeviceQueue(m_LogicalDevice, m_GraphicsFamily, 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_LogicalDevice, m_PresentFamily, 0, &m_PresentQueue);
		vkGetDeviceQueue(m_LogicalDevice, m_ComputeFamily, 0, &m_ComputeQueue);
		vkGetDeviceQueue(m_LogicalDevice, m_TransferFamily, 0, &m_TransferQueue);
	}
}  // namespace At0::Ray
