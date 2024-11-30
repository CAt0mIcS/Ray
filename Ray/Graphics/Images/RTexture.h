#pragma once

#include "RTextureSampler.h"
#include "RImage.h"

#include <string>


namespace At0::Ray
{
	class CommandPool;

	class RAY_EXPORT Texture : public Image
	{
	public:
		Texture(RenderContext& context, Ref<CommandPool> transientCommandPool, UInt2 extent,
			VkImageType imageType, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags memProps, Scope<TextureSampler> sampler, uint32_t mipLevels = 1,
			VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT, uint32_t arrayLayers = 1,
			VkImageCreateFlags createFlags = 0);

		Texture(
			RenderContext& context, Ref<CommandPool> transientCommandPool, std::string_view filepath);

		const TextureSampler& GetSampler() const { return *m_Sampler; }

		RAY_DEBUG_FLAG(std::string_view GetPath() const { return m_FilePath; })

	private:
		Scope<TextureSampler> m_Sampler;
		RAY_DEBUG_FLAG(std::string m_FilePath);

	public:
		class RAY_EXPORT Builder : public Image::BuilderBase<Texture::Builder>
		{
		public:
			Builder(RenderContext& context, Ref<CommandPool> transientCommandPool);

			Builder& SetTextureSampler(Scope<TextureSampler> sampler);

			Ref<Texture> Build();

		private:
			void ThrowIfInvalidArguments() const;

		private:
			Scope<TextureSampler> m_Sampler;
		};
	};

	template class RAY_EXPORT Image::BuilderBase<Texture::Builder>;
}  // namespace At0::Ray
