#include "Rpch.h"
#include "RLogicalDevice.h"

#include "Graphics/RGraphics.h"
#include "RVulkanInstance.h"
#include "RPhysicalDevice.h"
#include "RSurface.h"

#include "Utils/RAssert.h"
#include "Utils/RLogger.h"
#include "Utils/RException.h"


namespace At0::Ray
{
	LogicalDevice::LogicalDevice()
	{
		CreateQueueIndices();
		CreateLogicalDevice();
	}

	LogicalDevice::~LogicalDevice()
	{
		WaitIdle();
		vkDestroyDevice(m_Device, nullptr);
	}

	PFN_vkVoidFunction LogicalDevice::LoadFunction(const char* fName) const
	{
		return vkGetDeviceProcAddr(m_Device, fName);
	}

	void LogicalDevice::WaitIdle() const
	{
		RAY_VK_THROW_FAILED(
			vkDeviceWaitIdle(m_Device), "Failed to wait for the logica device to finish work");
	}

	bool LogicalDevice::IsEnabled(DeviceFeature feature) const
	{
		if (auto it = m_DeviceFeatures.find(feature); it != m_DeviceFeatures.end())
			return it->second;
		else
			return false;
	}

	void LogicalDevice::CreateQueueIndices()
	{
		uint32_t familyPropCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(
			Graphics::Get().GetPhysicalDevice(), &familyPropCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilyProperties(familyPropCount);
		vkGetPhysicalDeviceQueueFamilyProperties(
			Graphics::Get().GetPhysicalDevice(), &familyPropCount, queueFamilyProperties.data());

		std::optional<uint32_t> graphicsFamily, presentFamily, computeFamily, transferFamily;

		for (uint32_t i = 0; i < familyPropCount; ++i)
		{
			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphicsFamily = i;
				m_GraphicsFamily = i;
				m_SupportedQueues |= VK_QUEUE_GRAPHICS_BIT;
				Log::Info("[LogicalDevice] Graphics queue family {0} found.", m_GraphicsFamily);
			}

			VkBool32 presentSupport;
			vkGetPhysicalDeviceSurfaceSupportKHR(Graphics::Get().GetPhysicalDevice(), i,
				Graphics::Get().GetSurface(), &presentSupport);

			if (queueFamilyProperties[i].queueCount > 0 && presentSupport)
			{
				presentFamily = i;
				m_PresentFamily = i;
				Log::Info("[LogicalDevice] Present queue family {0} found.", m_PresentFamily);
			}

			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				computeFamily = i;
				m_ComputeFamily = i;
				m_SupportedQueues |= VK_QUEUE_COMPUTE_BIT;
				Log::Info("[LogicalDevice] Compute queue family {0} found.", m_ComputeFamily);
			}

			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				transferFamily = i;
				m_TransferFamily = i;
				m_SupportedQueues |= VK_QUEUE_TRANSFER_BIT;
				Log::Info("[LogicalDevice] Transfer queue family {0} found.", m_TransferFamily);
			}

			// Search until all queues are found
			if (graphicsFamily && presentFamily && computeFamily && transferFamily)
				break;
		}

		if (!graphicsFamily)
			RAY_THROW_RUNTIME("[LogicalDevice] Failed to find graphics queue family.");
	}

	void LogicalDevice::CreateLogicalDevice()
	{
		VkPhysicalDeviceFeatures physicalDeviceFeatures =
			Graphics::Get().GetPhysicalDevice().GetFeatures();
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
			Log::Info("[LogicalDevice] Creating graphics queue.");
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
			Log::Info("[LogicalDevice] Creating compute queue.");
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
			Log::Info("[LogicalDevice] Creating transfer queue.");
		}
		else
			m_TransferFamily = m_GraphicsFamily;

		VkPhysicalDeviceFeatures enabledFeatures{};

		// Enable sample rate shading filtering if supported.
		if (physicalDeviceFeatures.sampleRateShading)
		{
			enabledFeatures.sampleRateShading = VK_TRUE;
			m_DeviceFeatures.emplace(DeviceFeature::SampleRateShading, true);
			Log::Info("[LogicalDevice] Enabling sample rate shading filtering feature.");
		}

		// Fill mode non solid is required for wireframe display.
		if (physicalDeviceFeatures.fillModeNonSolid)
		{
			enabledFeatures.fillModeNonSolid = VK_TRUE;
			m_DeviceFeatures.emplace(DeviceFeature::FillModeNonSolid, true);
			Log::Info("[LogicalDevice] Enabling non solid fill mode feature.");
		}

		// Wide lines must be present for line width > 1.0f.
		if (physicalDeviceFeatures.wideLines)
		{
			enabledFeatures.wideLines = VK_TRUE;
			m_DeviceFeatures.emplace(DeviceFeature::WiderLines, true);
			Log::Info("[LogicalDevice] Enabling wider lines feature.");
		}
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support wireframe pipelines.");

		if (physicalDeviceFeatures.samplerAnisotropy)
		{
			enabledFeatures.samplerAnisotropy = VK_TRUE;
			m_DeviceFeatures.emplace(DeviceFeature::SamplerAnisotropy, true);
			Log::Info("[LogicalDevice] Enabling sampler anisotropy feature.");
		}
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support sampler anisotropy.");

		if (physicalDeviceFeatures.textureCompressionBC)
		{
			enabledFeatures.textureCompressionBC = VK_TRUE;
			m_DeviceFeatures.emplace(DeviceFeature::BCTextureCompression, true);
			Log::Info("[LogicalDevice] Enabling BC texture compression feature.");
		}
		else if (physicalDeviceFeatures.textureCompressionASTC_LDR)
		{
			enabledFeatures.textureCompressionASTC_LDR = VK_TRUE;
			m_DeviceFeatures.emplace(DeviceFeature::ASTC_LDRTextureCompression, true);
			Log::Info("[LogicalDevice] Enabling ASTC_LDR texture compression feature.");
		}
		else if (physicalDeviceFeatures.textureCompressionETC2)
		{
			enabledFeatures.textureCompressionETC2 = VK_TRUE;
			m_DeviceFeatures.emplace(DeviceFeature::ETC2TextureCompression, true);
			Log::Info("[LogicalDevice] Enabling ETC2 texture compression feature.");
		}

		if (physicalDeviceFeatures.vertexPipelineStoresAndAtomics)
		{
			enabledFeatures.vertexPipelineStoresAndAtomics = VK_TRUE;
			m_DeviceFeatures.emplace(DeviceFeature::VertexPipelineStoresAndAtomics, true);
			Log::Info("[LogicalDevice] Enabling vertex pipeline stores andd atomics feature.");
		}
		else
			Log::Warn(
				"[LogicalDevice] Selected GPU does not support vertex pipeline stores and atomics");

		if (physicalDeviceFeatures.fragmentStoresAndAtomics)
		{
			enabledFeatures.fragmentStoresAndAtomics = VK_TRUE;
			m_DeviceFeatures.emplace(DeviceFeature::FragmentStoresAndAtomics, true);
			Log::Info("[LogicalDevice] Enabling fragment pipeline stores andd atomics feature.");
		}
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support fragment stores and atomics.");

		if (physicalDeviceFeatures.shaderStorageImageExtendedFormats)
		{
			enabledFeatures.shaderStorageImageExtendedFormats = VK_TRUE;
			m_DeviceFeatures.emplace(DeviceFeature::ShaderStorageImageExtendedFormats, true);
			Log::Info("[LogicalDevice] Enabling shader storage image extended formats feature.");
		}
		else
			Log::Warn(
				"[LogicalDevice] Selected GPU does not support m_Shader storage extended formats.");

		if (physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat)
		{
			enabledFeatures.shaderStorageImageWriteWithoutFormat = VK_TRUE;
			m_DeviceFeatures.emplace(DeviceFeature::ShaderStorageImageWriteWithoutFormat, true);
			Log::Info(
				"[LogicalDevice] Enabling shader storage image write without format feature.");
		}
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support shader storage write without "
					  "format.");

		if (physicalDeviceFeatures.geometryShader)
		{
			enabledFeatures.geometryShader = VK_TRUE;
			m_DeviceFeatures.emplace(DeviceFeature::GeometryShader, true);
			Log::Info("[LogicalDevice] Enabling geometry shader feature.");
		}
		else
			Log::Warn("[LogicalDeivce] Selected GPU does not support geometry shaders.");

		if (physicalDeviceFeatures.tessellationShader)
		{
			enabledFeatures.tessellationShader = VK_TRUE;
			m_DeviceFeatures.emplace(DeviceFeature::TesselationShader, true);
			Log::Info("[LogicalDevice] Enabling tesselation shader feature.");
		}
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support tesselation shaders.");

		if (physicalDeviceFeatures.multiViewport)
		{
			enabledFeatures.multiViewport = VK_TRUE;
			m_DeviceFeatures.emplace(DeviceFeature::MultiViewport, true);
			Log::Info("[LogicalDevice] Enabling multi-viewport feature.");
		}
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support multiple viewports.");

		// Finally create the logical device
		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

		if (Graphics::Get().GetInstance().ValidationLayersEnabled())
		{
			deviceCreateInfo.enabledLayerCount =
				(uint32_t)VulkanInstance::GetValidationLayers().size();
			deviceCreateInfo.ppEnabledLayerNames = VulkanInstance::GetValidationLayers().data();
		}

		deviceCreateInfo.enabledExtensionCount = (uint32_t)GetDeviceExtensions().size();
		deviceCreateInfo.ppEnabledExtensionNames = GetDeviceExtensions().data();
		deviceCreateInfo.pEnabledFeatures = &enabledFeatures;
		RAY_VK_THROW_FAILED(vkCreateDevice(Graphics::Get().GetPhysicalDevice(), &deviceCreateInfo,
								nullptr, &m_Device),
			"[LogicalDevice] Failed to create logical device.");

		vkGetDeviceQueue(m_Device, m_GraphicsFamily, 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, m_PresentFamily, 0, &m_PresentQueue);
		vkGetDeviceQueue(m_Device, m_ComputeFamily, 0, &m_ComputeQueue);
		vkGetDeviceQueue(m_Device, m_TransferFamily, 0, &m_TransferQueue);
	}
}  // namespace At0::Ray
