#pragma once

#include "../../RBase.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RAY_EXPORT TextureSampler
	{
	public:
		TextureSampler() = default;
		TextureSampler(VkFilter magFilter, VkFilter minFilter, VkSamplerMipmapMode mipmapMode,
			VkSamplerAddressMode addressModeU, VkSamplerAddressMode addressModeV,
			VkSamplerAddressMode addressModeW, float mipLodBias, VkBool32 compareEnable,
			VkCompareOp compareOp, float minLod, float maxLod, VkBorderColor borderColor,
			VkSamplerCreateFlags flags);
		~TextureSampler();

		operator const VkSampler&() const { return m_Sampler; }

	private:
		VkSampler m_Sampler = VK_NULL_HANDLE;

	public:
		class RAY_EXPORT Builder
		{
		public:
			Builder() = default;

			Builder& SetMagFilter(VkFilter magFilter);
			Builder& SetMinFilter(VkFilter minFilter);
			Builder& SetMipmapMode(VkSamplerMipmapMode mipmapMode);
			Builder& SetAddressModeU(VkSamplerAddressMode addressModeU);
			Builder& SetAddressModeV(VkSamplerAddressMode addressModeV);
			Builder& SetAddressModeW(VkSamplerAddressMode addressModeW);
			Builder& SetMipLodBias(float mipLodBias);
			Builder& SetCompareEnable(VkBool32 compareEnable);
			Builder& SetCompareOp(VkCompareOp compareOp);
			Builder& SetMinLod(float minLod);
			Builder& SetMaxLod(float maxLod);
			Builder& SetBorderColor(VkBorderColor borderColor);
			Builder& SetSamplerCreateFlags(VkSamplerCreateFlags flags);

			Scope<TextureSampler> BuildScoped();

		private:
			VkFilter m_MagFilter = VK_FILTER_LINEAR;
			VkFilter m_MinFilter = VK_FILTER_LINEAR;
			VkSamplerMipmapMode m_MipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			VkSamplerAddressMode m_AddressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			VkSamplerAddressMode m_AddressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			VkSamplerAddressMode m_AddressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			float m_MipLodBias = 0.0f;
			VkBool32 m_CompareEnable = VK_FALSE;
			VkCompareOp m_CompareOp = VK_COMPARE_OP_ALWAYS;
			float m_MinLod = 0.0f;
			float m_MaxLod = 0.0f;
			VkBorderColor m_BorderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			VkSamplerCreateFlags m_Flags = 0;
		};
	};
}  // namespace At0::Ray
