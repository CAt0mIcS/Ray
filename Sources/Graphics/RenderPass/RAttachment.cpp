#include "Rpch.h"
#include "RAttachment.h"

#include "Graphics/Images/RImage.h"


namespace At0::Ray
{
	Attachment::Attachment(const Image& image, VkImageLayout finalLayout, LoadOp loadOp,
		StoreOp storeOp, LoadOp stencilLoadOp, StoreOp stencilStoreOp)
	{
		m_Description.format = image.GetFormat();
		m_Description.samples = VK_SAMPLE_COUNT_1_BIT;
		m_Description.loadOp = (VkAttachmentLoadOp)loadOp;
		m_Description.storeOp = (VkAttachmentStoreOp)storeOp;
		m_Description.stencilLoadOp = (VkAttachmentLoadOp)stencilLoadOp;
		m_Description.stencilStoreOp = (VkAttachmentStoreOp)stencilStoreOp;
		m_Description.initialLayout = image.GetImageLayout();
		m_Description.finalLayout = finalLayout;
	}

	Attachment::Attachment(VkFormat imageFormat, VkImageLayout finalLayout, LoadOp loadOp,
		StoreOp storeOp, LoadOp stencilLoadOp, StoreOp stencilStoreOp, VkImageLayout initialLayout)
	{
		m_Description.format = imageFormat;
		m_Description.samples = VK_SAMPLE_COUNT_1_BIT;
		m_Description.loadOp = (VkAttachmentLoadOp)loadOp;
		m_Description.storeOp = (VkAttachmentStoreOp)storeOp;
		m_Description.stencilLoadOp = (VkAttachmentLoadOp)stencilLoadOp;
		m_Description.stencilStoreOp = (VkAttachmentStoreOp)stencilStoreOp;
		m_Description.initialLayout = initialLayout;
		m_Description.finalLayout = finalLayout;
	}
}  // namespace At0::Ray
