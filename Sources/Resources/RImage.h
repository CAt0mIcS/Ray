#pragma once

#include "../RBase.h"
#include "../Graphics/Descriptors/RDescriptors.h"

#include <vulkan/vulkan_core.h>

namespace At0::Ray
{
	class RAY_EXPORT Image : public Descriptor
	{
	public:
		Image(VkFilter filter, VkSamplerAddressMode addressMode, VkSampleCountFlagBits samples,
			VkImageLayout layout, VkImageUsageFlags usage, VkFormat format, uint32_t mipLevels,
			uint32_t arrayLayers, const VkExtent3D& extent);

		const VkImageView& GetView() const { return m_View; }
		WriteDescriptorSet GetWriteDescriptor(uint32_t binding, VkDescriptorType descriptorType,
			const std::optional<OffsetSize>& offsetSize) const override;

		static void CreateImageView(const VkImage_T* image, VkImageView_T*& imageView,
			VkImageViewType type, VkFormat format, uint32_t imageAspect, uint32_t mipLevels,
			uint32_t baseMipLevel, uint32_t layerCount, uint32_t baseArrayLayer);

		static uint32_t GetMipLevels(const VkExtent3D& extent);

		static void CreateImage(VkImage& image, VkDeviceMemory& memory, const VkExtent3D& extent,
			VkFormat format, VkSampleCountFlagBits samples, VkImageTiling tiling,
			VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint32_t mipLevels,
			uint32_t arrayLayers, VkImageType type);

		static void CreateImageSampler(VkSampler& sampler, VkFilter filter,
			VkSamplerAddressMode addressMode, bool anisotropic, uint32_t mipLevels);

		static void TransitionImageLayout(const VkImage& image, VkFormat format,
			VkImageLayout srcImageLayout, VkImageLayout dstImageLayout,
			VkImageAspectFlags imageAspect, uint32_t mipLevels, uint32_t baseMipLevel,
			uint32_t layerCount, uint32_t baseArrayLayer);

		static void CreateMipmaps(const VkImage& image, const VkExtent3D& extent, VkFormat format,
			VkImageLayout dstImageLayout, uint32_t mipLevels, uint32_t baseArrayLayer,
			uint32_t layerCount);

		static VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates,
			VkImageTiling tiling, VkFormatFeatureFlags features);

		static bool HasStencil(VkFormat format);

		VkFormat GetFormat() const { return m_Format; }

	protected:
		VkExtent3D m_Extent;
		VkSampleCountFlagBits m_Samples;
		VkImageUsageFlags m_Usage;
		VkFormat m_Format = VK_FORMAT_UNDEFINED;
		uint32_t m_MipLevels = 0;
		uint32_t m_ArrayLayers;

		VkFilter m_Filter;
		VkSamplerAddressMode m_AddressMode;

		VkImageLayout m_Layout;

		VkImage m_Image = VK_NULL_HANDLE;
		VkDeviceMemory m_Memory = VK_NULL_HANDLE;
		VkSampler m_Sampler = VK_NULL_HANDLE;
		VkImageView m_View = VK_NULL_HANDLE;
	};
}  // namespace At0::Ray
