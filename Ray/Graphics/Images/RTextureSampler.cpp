#include "Rpch.h"
#include "RTextureSampler.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RLogicalDevice.h"

#include "RayBase/RException.h"


namespace At0::Ray
{
	TextureSampler::TextureSampler(VkSamplerAddressMode addressModeU,
		VkSamplerAddressMode addressModeV, VkSamplerAddressMode addressModeW, float maxLod)
	{
		VkSamplerCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		createInfo.magFilter = VK_FILTER_LINEAR;
		createInfo.minFilter = VK_FILTER_LINEAR;

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

		createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		createInfo.unnormalizedCoordinates = VK_FALSE;
		createInfo.compareEnable = VK_FALSE;
		createInfo.compareOp = VK_COMPARE_OP_ALWAYS;

		createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		createInfo.mipLodBias = 0.0f;
		createInfo.minLod = 0.0f;
		createInfo.maxLod = maxLod;

		ThrowVulkanError(
			vkCreateSampler(Graphics::Get().GetDevice(), &createInfo, nullptr, &m_Sampler),
			"[TextureSampler] Failed to create");
	}

	TextureSampler::~TextureSampler()
	{
		vkDestroySampler(Graphics::Get().GetDevice(), m_Sampler, nullptr);
	}
}  // namespace At0::Ray
