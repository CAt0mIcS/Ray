#pragma once

#include "Ray/RBase.h"
#include "Ray/Utils/RNonCopyable.h"

#include <RayRenderer/Core/RLogicalDevice.h>
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
		operator VkDevice() const { return (VkDevice)m_Device; }
		operator RrLogicalDevice() const { return m_Device; }

		uint32_t GetGraphicsFamily() const { return m_GraphicsFamily; }
		uint32_t GetPresentFamily() const { return m_PresentFamily; }
		uint32_t GetComputeFamily() const { return m_ComputeFamily; }
		uint32_t GetTransferFamily() const { return m_TransferFamily; }

		RrQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
		RrQueue GetPresentQueue() const { return m_PresentQueue; }
		RrQueue GetComputeQueue() const { return m_ComputeQueue; }
		RrQueue GetTransferQueue() const { return m_TransferQueue; }

	private:
		void CreateQueueIndices();
		void CreateLogicalDevice();

	private:
		static const std::vector<const char*> s_DeviceExtensions;

		RrLogicalDevice m_Device = nullptr;

		uint32_t m_GraphicsFamily = 0;
		uint32_t m_PresentFamily = 0;
		uint32_t m_ComputeFamily = 0;
		uint32_t m_TransferFamily = 0;

		RrQueue m_GraphicsQueue = nullptr;
		RrQueue m_PresentQueue = nullptr;
		RrQueue m_ComputeQueue = nullptr;
		RrQueue m_TransferQueue = nullptr;

		RrQueueFlags m_SupportedQueues = 0;

		std::unordered_map<DeviceFeature, bool> m_DeviceFeatures;
	};
}  // namespace At0::Ray
