#pragma once

#include "Ray/RBase.h"
#include "Ray/Utils/RNonCopyable.h"

#include <RayRenderer/Core/RRenderPass.h>
#include <vector>


namespace At0::Ray
{
	class CommandBuffer;
	class Framebuffer;

	class RAY_EXPORT RenderPass : NonCopyable
	{
	public:
		RenderPass(const std::vector<RrAttachmentDescription>& attachments,
			const std::vector<RrSubpassDescription>& subpasses,
			const std::vector<RrSubpassDependency>& dependencies);
		~RenderPass();

		void Begin(const CommandBuffer& cmdBuff, const Framebuffer& framebuffer,
			const RrClearValue clearValues[], uint32_t clearValueCount,
			RrSubpassContents subpassContents = RrSubpassContentsInline) const;
		void End(const CommandBuffer& cmdBuff) const;

		operator RrRenderPass() const { return m_Renderpass; }

	private:
		RrRenderPass m_Renderpass;
	};
}  // namespace At0::Ray
