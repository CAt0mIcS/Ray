#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>
#include <vector>


namespace At0::Ray
{
	class RenderPass;

	class RAY_EXPORT Framebuffer : NonCopyable
	{
	public:
		Framebuffer(const RenderPass& renderPass, const std::vector<VkImageView>& attachments);
		~Framebuffer();

		operator const VkFramebuffer&() const { return m_Framebuffer; }

	private:
		VkFramebuffer m_Framebuffer = VK_NULL_HANDLE;
	};
}  // namespace At0::Ray
