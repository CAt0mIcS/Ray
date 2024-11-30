#pragma once

#include "RImage.h"

#include <string_view>


namespace At0::Ray
{
	class TextureSampler;
	class RenderContext;
	class CommandPool;

	class RAY_EXPORT TextureCubemap : public Image
	{
	public:
		TextureCubemap(RenderContext& context, Ref<CommandPool> transientCommandPool,
			std::string_view filepath);
		~TextureCubemap();

		TextureCubemap(TextureCubemap&& other) noexcept;
		TextureCubemap& operator=(TextureCubemap&& other) noexcept;

	private:
		Scope<TextureSampler> m_Sampler;
	};

}  // namespace At0::Ray
