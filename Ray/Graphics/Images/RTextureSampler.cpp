#include "RTextureSampler.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RLogicalDevice.h"


namespace At0::Ray
{
	TextureSampler::TextureSampler(VkFilter magFilter, VkFilter minFilter,
		VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressModeU,
		VkSamplerAddressMode addressModeV, VkSamplerAddressMode addressModeW, float mipLodBias,
		VkBool32 compareEnable, VkCompareOp compareOp, float minLod, float maxLod,
		VkBorderColor borderColor, VkSamplerCreateFlags flags)
	{
		VkSamplerCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		createInfo.magFilter = magFilter;
		createInfo.minFilter = minFilter;

		createInfo.addressModeU = addressModeU;
		createInfo.addressModeV = addressModeV;
		createInfo.addressModeW = addressModeW;

		if (Graphics::Get().GetDevice().IsEnabled(DeviceFeature::SamplerAnisotropy))
		{
			createInfo.anisotropyEnable = VK_TRUE;
			createInfo.maxAnisotropy =
				Graphics::Get().GetPhysicalDevice().GetProperties().limits.maxSamplerAnisotropy;
		}
		else
			createInfo.anisotropyEnable = VK_FALSE;

		createInfo.borderColor = borderColor;
		createInfo.unnormalizedCoordinates = VK_FALSE;
		createInfo.compareEnable = compareEnable;
		createInfo.compareOp = compareOp;

		createInfo.mipmapMode = mipmapMode;
		createInfo.mipLodBias = mipLodBias;
		createInfo.minLod = minLod;
		createInfo.maxLod = maxLod;

		createInfo.flags = flags;

		ThrowVulkanError(
			vkCreateSampler(Graphics::Get().GetDevice(), &createInfo, nullptr, &m_Sampler),
			"[TextureSampler] Failed to create");
	}

	TextureSampler::~TextureSampler()
	{
		vkDestroySampler(Graphics::Get().GetDevice(), m_Sampler, nullptr);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////// BUILDER //////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TextureSampler::Builder& TextureSampler::Builder::SetMagFilter(VkFilter magFilter)
	{
		m_MagFilter = magFilter;
		return *this;
	}
	TextureSampler::Builder& TextureSampler::Builder::SetMinFilter(VkFilter minFilter)
	{
		m_MinFilter = minFilter;
		return *this;
	}
	TextureSampler::Builder& TextureSampler::Builder::SetMipmapMode(VkSamplerMipmapMode mipmapMode)
	{
		m_MipmapMode = mipmapMode;
		return *this;
	}
	TextureSampler::Builder& TextureSampler::Builder::SetAddressModeU(
		VkSamplerAddressMode addressModeU)
	{
		m_AddressModeU = addressModeU;
		return *this;
	}
	TextureSampler::Builder& TextureSampler::Builder::SetAddressModeV(
		VkSamplerAddressMode addressModeV)
	{
		m_AddressModeV = addressModeV;
		return *this;
	}
	TextureSampler::Builder& TextureSampler::Builder::SetAddressModeW(
		VkSamplerAddressMode addressModeW)
	{
		m_AddressModeW = addressModeW;
		return *this;
	}
	TextureSampler::Builder& TextureSampler::Builder::SetMipLodBias(float mipLodBias)
	{
		m_MipLodBias = mipLodBias;
		return *this;
	}
	TextureSampler::Builder& TextureSampler::Builder::SetCompareEnable(VkBool32 compareEnable)
	{
		m_CompareEnable = compareEnable;
		return *this;
	}
	TextureSampler::Builder& TextureSampler::Builder::SetCompareOp(VkCompareOp compareOp)
	{
		m_CompareOp = compareOp;
		return *this;
	}
	TextureSampler::Builder& TextureSampler::Builder::SetMinLod(float minLod)
	{
		m_MinLod = minLod;
		return *this;
	}
	TextureSampler::Builder& TextureSampler::Builder::SetMaxLod(float maxLod)
	{
		m_MaxLod = maxLod;
		return *this;
	}
	TextureSampler::Builder& TextureSampler::Builder::SetBorderColor(VkBorderColor borderColor)
	{
		m_BorderColor = borderColor;
		return *this;
	}
	TextureSampler::Builder& TextureSampler::Builder::SetSamplerCreateFlags(
		VkSamplerCreateFlags flags)
	{
		m_Flags = flags;
		return *this;
	}

	Scope<TextureSampler> TextureSampler::Builder::BuildScoped()
	{
		return MakeScope<TextureSampler>(m_MagFilter, m_MinFilter, m_MipmapMode, m_AddressModeU,
			m_AddressModeV, m_AddressModeW, m_MipLodBias, m_CompareEnable, m_CompareOp, m_MinLod,
			m_MaxLod, m_BorderColor, m_Flags);
	}

}  // namespace At0::Ray
