#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"
#include "../../Core/RMath.h"

#include <vulkan/vulkan_core.h>
#include <vector>


namespace At0::Ray
{
	class CommandBuffer;
	class Framebuffer;
	class RenderContext;

	class RAY_EXPORT RenderPass : NonCopyable
	{
	public:
		RenderPass(RenderContext& context, const std::vector<VkAttachmentDescription>& attachments,
			const std::vector<VkSubpassDescription>& subpasses,
			const std::vector<VkSubpassDependency>& dependencies);
		~RenderPass();

		void Begin(const CommandBuffer& cmdBuff, const Framebuffer& framebuffer,
			const VkClearValue clearValues[], uint32_t clearValueCount, UInt2 extent,
			VkSubpassContents subpassContents = VK_SUBPASS_CONTENTS_INLINE) const;
		void End(const CommandBuffer& cmdBuff) const;

		operator VkRenderPass() const { return m_Renderpass; }
		RenderContext& GetRenderContext() const { return m_Context; }

	private:
		VkRenderPass m_Renderpass;
		RenderContext& m_Context;
	};
}  // namespace At0::Ray
