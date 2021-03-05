#pragma once

#include "RTexture.h"
#include "RTextureSampler.h"

#include <string_view>


namespace At0::Ray
{
	class Image2D;

	class RAY_EXPORT Texture2D : public Texture
	{
	public:
		Texture2D(std::string_view filepath);

		Texture2D& operator=(Texture2D&& other);
		Texture2D(Texture2D&& other);

		const TextureSampler& GetSampler() const { return m_Sampler; }
		const Image2D& GetImage() const { return *m_Image; }

	private:
		Scope<Image2D> m_Image;
		TextureSampler m_Sampler;
	};

}  // namespace At0::Ray
