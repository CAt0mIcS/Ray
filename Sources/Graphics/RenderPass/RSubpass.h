#pragma once

#include "../../RBase.h"

#include <vulkan/vulkan_core.h>
#include <vector>


namespace At0::Ray
{
	class Attachment;

	class RAY_EXPORT Subpass
	{
	public:
		Subpass() = default;

		void AddInputAttachment(const Attachment& attachment);
		void AddColorAttachment(const Attachment& attachment);
		// void AddResolveAttachment(const Attachment& attachment);
		void AddDepthAttachment(const Attachment& attachment);
		void AddPreserveAttachment(uint32_t index);

		operator const VkSubpassDescription&() const { return m_Description; }

	private:
		void UpdateDescription();

	private:
		VkSubpassDescription m_Description{};

		std::vector<VkAttachmentReference> m_InputAttachments;
		std::vector<VkAttachmentReference> m_ColorAttachments;
		std::vector<VkAttachmentReference> m_ResolveAttachments;
		VkAttachmentReference m_DepthAttachment{};
		std::vector<uint32_t> m_PreserveAttachments;

		uint32_t m_AttachmentIndex = 0;
	};
}  // namespace At0::Ray
