#include "Rpch.h"
#include "RFramebuffer.h"

#include "Ray/Core/RRendererLoader.h"
#include "Ray/Graphics/Core/RLogicalDevice.h"
#include "Ray/Graphics/RGraphics.h"
#include "Ray/Graphics/Core/RSwapchain.h"
#include "Ray/Graphics/RenderPass/RRenderPass.h"

#include "Ray/Utils/RException.h"
#include "Ray/Utils/RLogger.h"


namespace At0::Ray
{
	Framebuffer::Framebuffer(
		const RenderPass& renderPass, const std::vector<RrImageView>& attachments)
	{
		RrFramebufferCreateInfo createInfo{};
		createInfo.renderPass = renderPass;
		createInfo.attachmentCount = (uint32_t)attachments.size();
		createInfo.pAttachments = attachments.data();
		createInfo.width = Graphics::Get().GetSwapchain().GetExtent().width;
		createInfo.height = Graphics::Get().GetSwapchain().GetExtent().height;
		createInfo.layers = 1;

		ThrowRenderError(RendererAPI::CreateFramebuffer(
							 Graphics::Get().GetDevice(), &createInfo, &m_Framebuffer),
			"[Framebuffer] Failed to create");
		Log::Info("[Framebuffer] Created with size [width={0}, height={1}]", createInfo.width,
			createInfo.height);
	}

	Framebuffer::~Framebuffer()
	{
		RendererAPI::DestroyFramebuffer(Graphics::Get().GetDevice(), m_Framebuffer);
	}
}  // namespace At0::Ray
