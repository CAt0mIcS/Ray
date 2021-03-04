#include "Rpch.h"
#include "RTextureSampler.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RLogicalDevice.h"

#include "Utils/RException.h"


namespace At0::Ray
{
	TextureSampler::TextureSampler()
	{
		VkSamplerCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		createInfo.magFilter = VK_FILTER_LINEAR;
		createInfo.minFilter = VK_FILTER_LINEAR;

		createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

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
		createInfo.maxLod = 0.0f;

		RAY_VK_THROW_FAILED(
			vkCreateSampler(Graphics::Get().GetDevice(), &createInfo, nullptr, &m_Sampler),
			"[TextureSampler] Failed to creaate.");
	}

	TextureSampler::~TextureSampler()
	{
		vkDestroySampler(Graphics::Get().GetDevice(), m_Sampler, nullptr);
	}
}  // namespace At0::Ray
