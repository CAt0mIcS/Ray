﻿#include "RFramebuffer.h"

#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/RGraphics.h"
#include "Graphics/RenderPass/RRenderPass.h"


namespace At0::Ray
{
	Framebuffer::Framebuffer(
		const RenderPass& renderPass, const std::vector<VkImageView>& attachments, UInt2 extent)
	{
		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.flags = 0;
		createInfo.renderPass = renderPass;
		createInfo.attachmentCount = (uint32_t)attachments.size();
		createInfo.pAttachments = attachments.data();
		createInfo.width = extent.x;
		createInfo.height = extent.y;
		createInfo.layers = 1;

		ThrowVulkanError(
			vkCreateFramebuffer(Graphics::Get().GetDevice(), &createInfo, nullptr, &m_Framebuffer),
			"[Framebuffer] Failed to create");
		Log::Info("[Framebuffer] Created with size [width={0}, height={1}]", createInfo.width,
			createInfo.height);
	}

	Framebuffer::~Framebuffer()
	{
		vkDestroyFramebuffer(Graphics::Get().GetDevice(), m_Framebuffer, nullptr);
	}
}  // namespace At0::Ray
