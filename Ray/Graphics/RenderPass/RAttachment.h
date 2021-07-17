#pragma once

#include "Ray/RBase.h"

#include <RayRenderer/Core/RRenderPass.h>


namespace At0::Ray
{
	class Image;

	class RAY_EXPORT Attachment
	{
	public:
		enum class LoadOp
		{
			Load = RrAttachmentLoadOpLoad,
			Clear = RrAttachmentLoadOpClear,
			Undefined = RrAttachmentLoadOpDontCare
		};

		enum class StoreOp
		{
			Store = RrAttachmentStoreOpStore,
			Undefined = RrAttachmentStoreOpDontCare
		};

	public:
		Attachment(const Image& image, RrImageLayout finalLayout, LoadOp loadOp, StoreOp storeOp,
			LoadOp stencilLoadOp, StoreOp stencilStoreOp);
		Attachment(RrFormat imageFormat, RrImageLayout finalLayout, LoadOp loadOp, StoreOp storeOp,
			LoadOp stencilLoadOp, StoreOp stencilStoreOp,
			RrImageLayout initialLayout = RrImageLayoutUndefined);

		operator const RrAttachmentDescription&() const { return m_Description; }

	private:
		RrAttachmentDescription m_Description{};
	};
}  // namespace At0::Ray
