#pragma once

#include "RTextureSampler.h"
#include "RImage.h"

#include <string>


namespace At0::Ray
{
	class RAY_EXPORT Texture : public Image
	{
	public:
		Texture(UInt2 extent, VkImageType imageType, VkFormat format, VkImageTiling tiling,
			VkImageUsageFlags usage, VkMemoryPropertyFlags memProps,
			Scope<TextureSampler> sampler = TextureSampler::Builder().BuildScoped(),
			uint32_t mipLevels = 1, VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT,
			uint32_t arrayLayers = 1, VkImageCreateFlags createFlags = 0);

		static Ref<Texture> Acquire(UInt2 extent, VkImageType imageType, VkFormat format,
			VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memProps,
			Scope<TextureSampler> sampler = TextureSampler::Builder().BuildScoped(),
			uint32_t mipLevels = 1, VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT,
			uint32_t arrayLayers = 1, VkImageCreateFlags createFlags = 0);

		static Ref<Texture> Acquire(std::string_view filepath);
		Texture(std::string_view filepath);

		const TextureSampler& GetSampler() const { return *m_Sampler; }

		RAY_DEBUG_FLAG(std::string_view GetPath() const { return m_FilePath; })

	private:
		Scope<TextureSampler> m_Sampler = TextureSampler::Builder().BuildScoped();
		RAY_DEBUG_FLAG(std::string m_FilePath);

	public:
		class RAY_EXPORT Builder : public Image::BuilderBase<Texture::Builder>
		{
		public:
			Builder() = default;

			Builder& SetTextureSampler(Scope<TextureSampler> sampler);

			Ref<Texture> Build();
			Ref<Texture> Acquire();

		private:
			void ThrowIfInvalidArguments() const;

		private:
			Scope<TextureSampler> m_Sampler;
		};
	};

}  // namespace At0::Ray
