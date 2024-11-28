#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"
#include "../../Core/RMath.h"

#include <vulkan/vulkan_core.h>
#include <vector>


namespace At0::Ray
{
	class RenderPass;
	class LogicalDevice;

	class RAY_EXPORT Framebuffer : NonCopyable
	{
	public:
		Framebuffer(const RenderPass& renderPass, const std::vector<VkImageView>& attachments,
			UInt2 extent);
		~Framebuffer();

		operator const VkFramebuffer&() const { return m_Framebuffer; }

	private:
		VkFramebuffer m_Framebuffer = VK_NULL_HANDLE;
		const LogicalDevice& m_Device;
	};
}  // namespace At0::Ray
