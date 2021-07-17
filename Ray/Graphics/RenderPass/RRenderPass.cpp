#include "Rpch.h"
#include "RRenderPass.h"

#include "Ray/Core/RRendererLoader.h"
#include "Ray/Graphics/RGraphics.h"
#include "Ray/Graphics/Core/RLogicalDevice.h"
#include "Ray/Graphics/Core/RSwapchain.h"
#include "Ray/Graphics/Buffers/RFramebuffer.h"
#include "Ray/Graphics/Commands/RCommandBuffer.h"

#include "Ray/Utils/RException.h"

namespace At0::Ray
{
	RenderPass::RenderPass(const std::vector<RrAttachmentDescription>& attachments,
		const std::vector<RrSubpassDescription>& subpasses,
		const std::vector<RrSubpassDependency>& dependencies)
	{
		RrRenderPassCreateInfo createInfo{};
		createInfo.attachmentCount = (uint32_t)attachments.size();
		createInfo.pAttachments = attachments.data();
		createInfo.subpassCount = (uint32_t)subpasses.size();
		createInfo.pSubpasses = subpasses.data();
		createInfo.dependencyCount = (uint32_t)dependencies.size();
		createInfo.pDependencies = dependencies.data();

		ThrowRenderError(
			RendererAPI::CreateRenderPass(Graphics::Get().GetDevice(), &createInfo, &m_Renderpass),
			"[RenderPass] Failed to create");
	}

	RenderPass::~RenderPass()
	{
		vkDestroyRenderPass(Graphics::Get().GetDevice(), (VkRenderPass)m_Renderpass, nullptr);
	}

	void RenderPass::Begin(const CommandBuffer& cmdBuff, const Framebuffer& framebuffer,
		const RrClearValue clearValues[], uint32_t clearValueCount,
		RrSubpassContents subpassContents) const
	{
		RrRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.renderPass = m_Renderpass;
		renderPassInfo.framebuffer = framebuffer;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = Graphics::Get().GetSwapchain().GetExtent();
		renderPassInfo.clearValueCount = clearValueCount;
		renderPassInfo.pClearValues = clearValues;

		RendererAPI::CmdBeginRenderPass(cmdBuff, &renderPassInfo, subpassContents);
	}

	void RenderPass::End(const CommandBuffer& cmdBuff) const
	{
		RendererAPI::CmdEndRenderPass(cmdBuff);
	}
}  // namespace At0::Ray
