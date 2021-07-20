#pragma once

#include "Ray/RBase.h"
#include "Ray/Utils/RNonCopyable.h"

#include <RayRenderer/Core/RImageView.h>


namespace At0::Ray
{
	class Image;

	class ImageView : NonCopyable
	{
	public:
		ImageView(const Image& image);
		ImageView(RrImage image, RrImageViewType viewType, RrFormat format, uint32_t mipLevels = 1,
			RrImageAspectFlags aspectFlags = RrImageAspectColor, uint32_t layerCount = 1);
		~ImageView();

		operator RrImageView() const { return m_View; }

	private:
		void Setup(RrImage image, RrImageViewType viewType, RrFormat format, uint32_t mipLevels,
			RrImageAspectFlags aspectFlags, uint32_t layerCount);

	private:
		RrImageView m_View = nullptr;
	};
}  // namespace At0::Ray
