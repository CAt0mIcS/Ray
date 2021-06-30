#include "Rpch.h"
#include "RLogicalDevice.h"

#include "Graphics/RGraphics.h"
#include "RRendererInstance.h"
#include "RPhysicalDevice.h"
#include "RSurface.h"

#include "RayBase/RAssert.h"
#include "RayBase/RLogger.h"
#include "RayBase/RException.h"

#include "Core/RRendererLoader.h"


namespace At0::Ray
{
	const std::vector<const char*> LogicalDevice::s_DeviceExtensions{ "VK_KHR_swapchain" };

	LogicalDevice::LogicalDevice()
	{
		CreateQueueIndices();
		CreateLogicalDevice();
	}

	LogicalDevice::~LogicalDevice()
	{
		WaitIdle();
		RendererAPI::DestroyLogicalDevice(m_Device);
	}

	PFN_vkVoidFunction LogicalDevice::LoadFunction(const char* fName) const
	{
		return RendererAPI::GetDeviceProcAddr(m_Device, fName);
	}

	void LogicalDevice::WaitIdle() const
	{
		ThrowRenderError(RendererAPI::DeviceWaitIdle(m_Device),
			"Failed to wait for the logical device to finish work");
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
		VkPhysicalDevice physicalDevice = VkPhysicalDevice(
			Graphics::Get().GetPhysicalDevice().operator const RrPhysicalDevice&());

		uint32_t familyPropCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &familyPropCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilyProperties(familyPropCount);
		vkGetPhysicalDeviceQueueFamilyProperties(
			physicalDevice, &familyPropCount, queueFamilyProperties.data());

		std::optional<uint32_t> graphicsFamily, presentFamily, computeFamily, transferFamily;

		for (uint32_t i = 0; i < familyPropCount; ++i)
		{
			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphicsFamily = i;
				m_GraphicsFamily = i;
				m_SupportedQueues |= VK_QUEUE_GRAPHICS_BIT;
				Log::Info("[LogicalDevice] Graphics queue family {0} found", m_GraphicsFamily);
			}

			VkBool32 presentSupport;
			vkGetPhysicalDeviceSurfaceSupportKHR(
				physicalDevice, i, Graphics::Get().GetSurface(), &presentSupport);

			if (queueFamilyProperties[i].queueCount > 0 && presentSupport)
			{
				presentFamily = i;
				m_PresentFamily = i;
				Log::Info("[LogicalDevice] Present queue family {0} found", m_PresentFamily);
			}

			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				computeFamily = i;
				m_ComputeFamily = i;
				m_SupportedQueues |= VK_QUEUE_COMPUTE_BIT;
				Log::Info("[LogicalDevice] Compute queue family {0} found", m_ComputeFamily);
			}

			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				transferFamily = i;
				m_TransferFamily = i;
				m_SupportedQueues |= VK_QUEUE_TRANSFER_BIT;
				Log::Info("[LogicalDevice] Transfer queue family {0} found", m_TransferFamily);
			}

			// Search until all queues are found
			if (graphicsFamily && presentFamily && computeFamily && transferFamily)
				break;
		}

		if (!graphicsFamily)
			ThrowRuntime("[LogicalDevice] Failed to find graphics queue family");
	}

	void LogicalDevice::CreateLogicalDevice()
	{
		RrPhysicalDeviceFeatures physicalDeviceFeatures =
			Graphics::Get().GetPhysicalDevice().GetFeatures();
		std::vector<RrDeviceQueueCreateInfo> queueCreateInfos;
		float queuePriorities[] = { 0.0f };

		// Fill vector with correct create info for the queues.
		if (m_SupportedQueues & VK_QUEUE_GRAPHICS_BIT)
		{
			RrDeviceQueueCreateInfo graphicsQueueCreateInfo{};
			graphicsQueueCreateInfo.queueFamilyIndex = m_GraphicsFamily;
			graphicsQueueCreateInfo.queueCount = 1;
			graphicsQueueCreateInfo.pQueuePriorities = queuePriorities;
			queueCreateInfos.push_back(std::move(graphicsQueueCreateInfo));
			Log::Info("[LogicalDevice] Creating graphics queue");
		}
		else
			m_GraphicsFamily = 0;

		if (m_SupportedQueues & VK_QUEUE_COMPUTE_BIT && m_ComputeFamily != m_GraphicsFamily)
		{
			RrDeviceQueueCreateInfo computeQueueCreateInfo{};
			computeQueueCreateInfo.queueFamilyIndex = m_ComputeFamily;
			computeQueueCreateInfo.queueCount = 1;
			computeQueueCreateInfo.pQueuePriorities = queuePriorities;
			queueCreateInfos.push_back(std::move(computeQueueCreateInfo));
			Log::Info("[LogicalDevice] Creating compute queue");
		}
		else
			m_ComputeFamily = m_GraphicsFamily;

		if (m_SupportedQueues & VK_QUEUE_TRANSFER_BIT && m_TransferFamily != m_GraphicsFamily &&
			m_TransferFamily != m_ComputeFamily)
		{
			RrDeviceQueueCreateInfo transferQueueCreateInfo{};
			transferQueueCreateInfo.queueFamilyIndex = m_TransferFamily;
			transferQueueCreateInfo.queueCount = 1;
			transferQueueCreateInfo.pQueuePriorities = queuePriorities;
			queueCreateInfos.push_back(std::move(transferQueueCreateInfo));
			Log::Info("[LogicalDevice] Creating transfer queue");
		}
		else
			m_TransferFamily = m_GraphicsFamily;

		RrPhysicalDeviceFeatures enabledFeatures{};

		// Enable sample rate shading filtering if supported.
		if (physicalDeviceFeatures.sampleRateShading)
		{
			enabledFeatures.sampleRateShading = true;
			m_DeviceFeatures.emplace(DeviceFeature::SampleRateShading, true);
			Log::Info("[LogicalDevice] Enabling sample rate shading filtering feature");
		}

		// Fill mode non solid is required for wireframe display.
		if (physicalDeviceFeatures.fillModeNonSolid)
		{
			enabledFeatures.fillModeNonSolid = true;
			m_DeviceFeatures.emplace(DeviceFeature::FillModeNonSolid, true);
			Log::Info("[LogicalDevice] Enabling non solid fill mode feature");
		}

		// Wide lines must be present for line width > 1.0f.
		if (physicalDeviceFeatures.wideLines)
		{
			enabledFeatures.wideLines = true;
			m_DeviceFeatures.emplace(DeviceFeature::WiderLines, true);
			Log::Info("[LogicalDevice] Enabling wider lines feature");
		}
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support wireframe pipelines");

		if (physicalDeviceFeatures.samplerAnisotropy)
		{
			enabledFeatures.samplerAnisotropy = true;
			m_DeviceFeatures.emplace(DeviceFeature::SamplerAnisotropy, true);
			Log::Info("[LogicalDevice] Enabling sampler anisotropy feature");
		}
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support sampler anisotropy");

		if (physicalDeviceFeatures.textureCompressionBC)
		{
			enabledFeatures.textureCompressionBC = true;
			m_DeviceFeatures.emplace(DeviceFeature::BCTextureCompression, true);
			Log::Info("[LogicalDevice] Enabling BC texture compression feature");
		}
		else if (physicalDeviceFeatures.textureCompressionASTC_LDR)
		{
			enabledFeatures.textureCompressionASTC_LDR = true;
			m_DeviceFeatures.emplace(DeviceFeature::ASTC_LDRTextureCompression, true);
			Log::Info("[LogicalDevice] Enabling ASTC_LDR texture compression feature");
		}
		else if (physicalDeviceFeatures.textureCompressionETC2)
		{
			enabledFeatures.textureCompressionETC2 = true;
			m_DeviceFeatures.emplace(DeviceFeature::ETC2TextureCompression, true);
			Log::Info("[LogicalDevice] Enabling ETC2 texture compression feature");
		}

		if (physicalDeviceFeatures.vertexPipelineStoresAndAtomics)
		{
			enabledFeatures.vertexPipelineStoresAndAtomics = true;
			m_DeviceFeatures.emplace(DeviceFeature::VertexPipelineStoresAndAtomics, true);
			Log::Info("[LogicalDevice] Enabling vertex pipeline stores and atomics feature");
		}
		else
			Log::Warn(
				"[LogicalDevice] Selected GPU does not support vertex pipeline stores and atomics");

		if (physicalDeviceFeatures.fragmentStoresAndAtomics)
		{
			enabledFeatures.fragmentStoresAndAtomics = true;
			m_DeviceFeatures.emplace(DeviceFeature::FragmentStoresAndAtomics, true);
			Log::Info("[LogicalDevice] Enabling fragment stores and atomics feature");
		}
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support fragment stores and atomics");

		if (physicalDeviceFeatures.shaderStorageImageExtendedFormats)
		{
			enabledFeatures.shaderStorageImageExtendedFormats = true;
			m_DeviceFeatures.emplace(DeviceFeature::ShaderStorageImageExtendedFormats, true);
			Log::Info("[LogicalDevice] Enabling shader storage image extended formats feature");
		}
		else
			Log::Warn(
				"[LogicalDevice] Selected GPU does not support m_Shader storage extended formats");

		if (physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat)
		{
			enabledFeatures.shaderStorageImageWriteWithoutFormat = true;
			m_DeviceFeatures.emplace(DeviceFeature::ShaderStorageImageWriteWithoutFormat, true);
			Log::Info("[LogicalDevice] Enabling shader storage image write without format feature");
		}
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support shader storage write without "
					  "format");

		if (physicalDeviceFeatures.geometryShader)
		{
			enabledFeatures.geometryShader = true;
			m_DeviceFeatures.emplace(DeviceFeature::GeometryShader, true);
			Log::Info("[LogicalDevice] Enabling geometry shader feature");
		}
		else
			Log::Warn("[LogicalDeivce] Selected GPU does not support geometry shaders");

		if (physicalDeviceFeatures.tessellationShader)
		{
			enabledFeatures.tessellationShader = true;
			m_DeviceFeatures.emplace(DeviceFeature::TesselationShader, true);
			Log::Info("[LogicalDevice] Enabling tesselation shader feature");
		}
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support tesselation shaders");

		if (physicalDeviceFeatures.multiViewport)
		{
			enabledFeatures.multiViewport = true;
			m_DeviceFeatures.emplace(DeviceFeature::MultiViewport, true);
			Log::Info("[LogicalDevice] Enabling multi-viewport feature");
		}
		else
			Log::Warn("[LogicalDevice] Selected GPU does not support multiple viewports");

		// Finally create the logical device
		RrLogicalDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

		if (Graphics::Get().GetInstance().ValidationLayersEnabled())
		{
			deviceCreateInfo.enabledLayerCount =
				(uint32_t)RendererInstance::GetValidationLayers().size();
			deviceCreateInfo.ppEnabledLayerNames = RendererInstance::GetValidationLayers().data();
		}

		deviceCreateInfo.enabledExtensionCount = (uint32_t)GetDeviceExtensions().size();
		deviceCreateInfo.ppEnabledExtensionNames = GetDeviceExtensions().data();
		deviceCreateInfo.pEnabledFeatures = &enabledFeatures;
		ThrowRenderError(RendererAPI::CreateLogicalDevice(
							 Graphics::Get().GetPhysicalDevice(), &deviceCreateInfo, &m_Device),
			"[LogicalDevice] Failed to create logical device");

		RendererAPI::GetDeviceQueue(m_Device, m_GraphicsFamily, 0, &m_GraphicsQueue);
		RendererAPI::GetDeviceQueue(m_Device, m_PresentFamily, 0, &m_PresentQueue);
		RendererAPI::GetDeviceQueue(m_Device, m_ComputeFamily, 0, &m_ComputeQueue);
		RendererAPI::GetDeviceQueue(m_Device, m_TransferFamily, 0, &m_TransferQueue);
	}
}  // namespace At0::Ray
