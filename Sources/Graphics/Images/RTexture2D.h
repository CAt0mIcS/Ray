#pragma once

#include "RTexture.h"
#include "RImage2D.h"

#include <string_view>


namespace At0::Ray
{
	class RAY_EXPORT Texture2D : public Texture
	{
	public:
		Texture2D(std::string_view filepath);

		Texture2D& operator=(Texture2D&& other);
		Texture2D(Texture2D&& other);

	private:
		Image2D m_Image;
	};

}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Texture2D);
