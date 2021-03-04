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
		Texture2D(Entity& entity, std::string_view filepath);

		Texture2D& operator=(Texture2D&& other);
		Texture2D(Texture2D&& other);

	private:
		Scope<Image2D> m_Image;
		TextureSampler m_Sampler;
	};

}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Texture2D);
