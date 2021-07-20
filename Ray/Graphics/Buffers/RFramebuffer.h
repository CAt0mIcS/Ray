#pragma once

#include "Ray/RBase.h"
#include "Ray/Utils/RNonCopyable.h"

#include <RayRenderer/Core/RFramebuffer.h>
#include <vector>


namespace At0::Ray
{
	class RenderPass;

	class RAY_EXPORT Framebuffer : NonCopyable
	{
	public:
		Framebuffer(const RenderPass& renderPass, const std::vector<RrImageView>& attachments);
		~Framebuffer();

		operator RrFramebuffer() const { return m_Framebuffer; }

	private:
		RrFramebuffer m_Framebuffer = nullptr;
	};
}  // namespace At0::Ray
