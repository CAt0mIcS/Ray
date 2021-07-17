#include "Rpch.h"
#include "RAttachment.h"

#include "Graphics/Images/RImage.h"


namespace At0::Ray
{
	Attachment::Attachment(const Image& image, RrImageLayout finalLayout, LoadOp loadOp,
		StoreOp storeOp, LoadOp stencilLoadOp, StoreOp stencilStoreOp)
	{
		m_Description.format = image.GetFormat();
		m_Description.samples = RrSampleCount1;
		m_Description.loadOp = (RrAttachmentLoadOp)loadOp;
		m_Description.storeOp = (RrAttachmentStoreOp)storeOp;
		m_Description.stencilLoadOp = (RrAttachmentLoadOp)stencilLoadOp;
		m_Description.stencilStoreOp = (RrAttachmentStoreOp)stencilStoreOp;
		m_Description.initialLayout = image.GetImageLayout();
		m_Description.finalLayout = finalLayout;
	}

	Attachment::Attachment(RrFormat imageFormat, RrImageLayout finalLayout, LoadOp loadOp,
		StoreOp storeOp, LoadOp stencilLoadOp, StoreOp stencilStoreOp, RrImageLayout initialLayout)
	{
		m_Description.format = imageFormat;
		m_Description.samples = RrSampleCount1;
		m_Description.loadOp = (RrAttachmentLoadOp)loadOp;
		m_Description.storeOp = (RrAttachmentStoreOp)storeOp;
		m_Description.stencilLoadOp = (RrAttachmentLoadOp)stencilLoadOp;
		m_Description.stencilStoreOp = (RrAttachmentStoreOp)stencilStoreOp;
		m_Description.initialLayout = initialLayout;
		m_Description.finalLayout = finalLayout;
	}
}  // namespace At0::Ray
