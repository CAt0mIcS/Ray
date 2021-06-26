#pragma once

#include "Ray/RBase.h"
#include "RayBase/RNonCopyable.h"

#include <vulkan/vulkan_core.h>
#include <vector>


namespace At0::Ray
{
	enum class DeviceFeature
	{
		SampleRateShading,
		FillModeNonSolid,
		WiderLines,
		SamplerAnisotropy,
		BCTextureCompression,
		ASTC_LDRTextureCompression,
		ETC2TextureCompression,
		VertexPipelineStoresAndAtomics,
		FragmentStoresAndAtomics,
		ShaderStorageImageExtendedFormats,
		ShaderStorageImageWriteWithoutFormat,
		GeometryShader,
		TesselationShader,
		MultiViewport
	};

	class RAY_EXPORT LogicalDevice : NonCopyable
	{
	public:
		LogicalDevice();
		~LogicalDevice();

		/**
		 * Loads a device extension function with name and returns it
		 */
		PFN_vkVoidFunction LoadFunction(const char* fName) const;

		/**
		 * Waits for the logical device to finish work
		 */
		void WaitIdle() const;

		/**
		 * @returns If a specified gpu feature was enabled
		 */
		bool IsEnabled(DeviceFeature feature) const;

		static const std::vector<const char*>& GetDeviceExtensions() { return s_DeviceExtensions; }
		operator const VkDevice&() const { return m_Device; }

		uint32_t GetGraphicsFamily() const { return m_GraphicsFamily; }
		uint32_t GetPresentFamily() const { return m_PresentFamily; }
		uint32_t GetComputeFamily() const { return m_ComputeFamily; }
		uint32_t GetTransferFamily() const { return m_TransferFamily; }

		VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
		VkQueue GetPresentQueue() const { return m_PresentQueue; }
		VkQueue GetComputeQueue() const { return m_ComputeQueue; }
		VkQueue GetTransferQueue() const { return m_TransferQueue; }

	private:
		void CreateQueueIndices();
		void CreateLogicalDevice();

	private:
		static const std::vector<const char*> s_DeviceExtensions;

		VkDevice m_Device = VK_NULL_HANDLE;

		uint32_t m_GraphicsFamily = 0;
		uint32_t m_PresentFamily = 0;
		uint32_t m_ComputeFamily = 0;
		uint32_t m_TransferFamily = 0;

		VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
		VkQueue m_PresentQueue = VK_NULL_HANDLE;
		VkQueue m_ComputeQueue = VK_NULL_HANDLE;
		VkQueue m_TransferQueue = VK_NULL_HANDLE;

		VkQueueFlags m_SupportedQueues = 0;

		std::unordered_map<DeviceFeature, bool> m_DeviceFeatures;
	};
}  // namespace At0::Ray
