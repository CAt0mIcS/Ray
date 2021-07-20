#include "Rpch.h"
#include "RTextureSampler.h"

#include "Ray/Core/RRendererLoader.h"
#include "Ray/Graphics/RGraphics.h"
#include "Ray/Graphics/Core/RPhysicalDevice.h"
#include "Ray/Graphics/Core/RLogicalDevice.h"

#include "Ray/Utils/RException.h"


namespace At0::Ray
{
	TextureSampler::TextureSampler(RrSamplerAddressMode addressModeU,
		RrSamplerAddressMode addressModeV, RrSamplerAddressMode addressModeW, float maxLod)
	{
		RrSamplerCreateInfo createInfo{};
		createInfo.magFilter = RrFilterLinear;
		createInfo.minFilter = RrFilterLinear;

		createInfo.addressModeU = addressModeU;
		createInfo.addressModeV = addressModeV;
		createInfo.addressModeW = addressModeW;

		if (Graphics::Get().GetDevice().IsEnabled(DeviceFeature::SamplerAnisotropy))
		{
			createInfo.anisotropyEnable = true;
			createInfo.maxAnisotropy =
				Graphics::Get().GetPhysicalDevice().GetProperties().limits.maxSamplerAnisotropy;
		}
		else
			createInfo.anisotropyEnable = false;

		createInfo.borderColor = RrBorderColorIntOpaqueBlack;
		createInfo.unnormalizedCoordinates = false;
		createInfo.compareEnable = false;
		createInfo.compareOp = RrCompareOpAlways;

		createInfo.mipmapMode = RrSamplerMipmapModeLinear;
		createInfo.mipLodBias = 0.0f;
		createInfo.minLod = 0.0f;
		createInfo.maxLod = maxLod;

		ThrowRenderError(
			RendererAPI::CreateSampler(Graphics::Get().GetDevice(), &createInfo, &m_Sampler),
			"[TextureSampler] Failed to create");
	}

	TextureSampler::~TextureSampler()
	{
		RendererAPI::DestroySampler(Graphics::Get().GetDevice(), m_Sampler);
	}
}  // namespace At0::Ray
