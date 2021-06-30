#pragma once

#include "Ray/RBase.h"
#include "Ray/Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>
#include <RayRenderer/Core/RFramebuffer.h>
#include <vector>


namespace At0::Ray
{
	class RenderPass;

	class RAY_EXPORT Framebuffer : NonCopyable
	{
	public:
		Framebuffer(const RenderPass& renderPass, const std::vector<VkImageView>& attachments);
		~Framebuffer();

		operator VkFramebuffer() const { return (VkFramebuffer)m_Framebuffer; }
		operator RrFramebuffer() const { return m_Framebuffer; }

	private:
		RrFramebuffer m_Framebuffer = nullptr;
	};
}  // namespace At0::Ray
