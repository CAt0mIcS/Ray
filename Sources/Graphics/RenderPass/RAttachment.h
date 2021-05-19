#pragma once

#include "../../RBase.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class Image;

	class RAY_EXPORT Attachment
	{
	public:
		enum class LoadOp
		{
			Load = VK_ATTACHMENT_LOAD_OP_LOAD,
			Clear = VK_ATTACHMENT_LOAD_OP_CLEAR,
			Undefined = VK_ATTACHMENT_LOAD_OP_DONT_CARE
		};

		enum class StoreOp
		{
			Store = VK_ATTACHMENT_STORE_OP_STORE,
			Undefined = VK_ATTACHMENT_STORE_OP_DONT_CARE
		};

	public:
		Attachment(const Image& image, VkImageLayout finalLayout, LoadOp loadOp, StoreOp storeOp,
			LoadOp stencilLoadOp, StoreOp stencilStoreOp);
		Attachment(VkFormat imageFormat, VkImageLayout finalLayout, LoadOp loadOp, StoreOp storeOp,
			LoadOp stencilLoadOp, StoreOp stencilStoreOp,
			VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED);

		operator const VkAttachmentDescription&() const { return m_Description; }

	private:
		VkAttachmentDescription m_Description{};
	};
}  // namespace At0::Ray
