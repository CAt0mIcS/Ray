#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>
#include <vector>

namespace At0::Ray
{
	class CommandBuffer;
	class Framebuffer;

	class RAY_EXPORT RenderPass : NonCopyable
	{
	public:
		RenderPass(const std::vector<VkAttachmentDescription>& attachments,
			const std::vector<VkSubpassDescription>& subpasses,
			const std::vector<VkSubpassDependency>& dependencies);
		~RenderPass();

		void Begin(const CommandBuffer& cmdBuff, const Framebuffer& framebuffer,
			const std::vector<VkClearValue>& clearValues) const;
		void End(const CommandBuffer& cmdBuff) const;

		operator const VkRenderPass&() const { return m_Renderpass; }

	private:
		VkRenderPass m_Renderpass;
	};
}  // namespace At0::Ray