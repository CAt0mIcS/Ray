#include "Rpch.h"
#include "RFramebuffer.h"

#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Core/RSwapchain.h"
#include "Graphics/RenderPass/RRenderPass.h"

#include "RayBase/RException.h"
#include "RayBase/RLogger.h"


namespace At0::Ray
{
	Framebuffer::Framebuffer(
		const RenderPass& renderPass, const std::vector<VkImageView>& attachments)
	{
		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.flags = 0;
		createInfo.renderPass = renderPass;
		createInfo.attachmentCount = (uint32_t)attachments.size();
		createInfo.pAttachments = attachments.data();
		createInfo.width = Graphics::Get().GetSwapchain().GetExtent().width;
		createInfo.height = Graphics::Get().GetSwapchain().GetExtent().height;
		createInfo.layers = 1;

		ThrowRenderError(vkCreateFramebuffer(Graphics::Get().GetDevice(), &createInfo, nullptr,
							 (VkFramebuffer*)&m_Framebuffer),
			"[Framebuffer] Failed to create");
		Log::Info("[Framebuffer] Created with size [width={0}, height={1}]", createInfo.width,
			createInfo.height);
	}

	Framebuffer::~Framebuffer()
	{
		vkDestroyFramebuffer(Graphics::Get().GetDevice(), (VkFramebuffer)m_Framebuffer, nullptr);
	}
}  // namespace At0::Ray
