#include "Rpch.h"
#include "RRenderPass.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RSwapchain.h"
#include "Graphics/Buffers/RFramebuffer.h"

#include "Graphics/Commands/RCommandBuffer.h"

#include "Utils/RException.h"

namespace At0::Ray
{
	RenderPass::RenderPass(const std::vector<VkAttachmentDescription>& attachments,
		const std::vector<VkSubpassDescription>& subpasses,
		const std::vector<VkSubpassDependency>& dependencies)
	{
		VkRenderPassCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = (uint32_t)attachments.size();
		createInfo.pAttachments = attachments.data();
		createInfo.subpassCount = (uint32_t)subpasses.size();
		createInfo.pSubpasses = subpasses.data();
		createInfo.dependencyCount = (uint32_t)dependencies.size();
		createInfo.pDependencies = dependencies.data();

		RAY_VK_THROW_FAILED(
			vkCreateRenderPass(Graphics::Get().GetDevice(), &createInfo, nullptr, &m_Renderpass),
			"[RenderPass] Failed to create.");
	}

	RenderPass::~RenderPass()
	{
		vkDestroyRenderPass(Graphics::Get().GetDevice(), m_Renderpass, nullptr);
	}

	void RenderPass::Begin(const CommandBuffer& cmdBuff, const Framebuffer& framebuffer,
		const std::vector<VkClearValue>& clearValues) const
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_Renderpass;
		renderPassInfo.framebuffer = framebuffer;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = Graphics::Get().GetSwapchain().GetExtent();
		renderPassInfo.clearValueCount = (uint32_t)clearValues.size();
		renderPassInfo.pClearValues = clearValues.data();
		vkCmdBeginRenderPass(cmdBuff, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void RenderPass::End(const CommandBuffer& cmdBuff) const { vkCmdEndRenderPass(cmdBuff); }
}  // namespace At0::Ray