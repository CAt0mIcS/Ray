#pragma once

#include "../../RBase.h"

#include <vulkan/vulkan_core.h>
#include <vector>
#include <optional>


namespace At0::Ray
{
	class Attachment;

	class RAY_EXPORT Subpass
	{
	public:
		Subpass() = default;

		void AddInputAttachment(uint32_t index, const Attachment& attachment);
		void AddColorAttachment(uint32_t index, const Attachment& attachment);
		// void AddResolveAttachment(cuint32_t index, onst Attachment& attachment);
		void AddDepthAttachment(uint32_t index, const Attachment& attachment);
		void AddPreserveAttachment(uint32_t index);

		operator const VkSubpassDescription&() const { return m_Description; }

	private:
		void UpdateDescription();

	private:
		VkSubpassDescription m_Description{};

		std::vector<VkAttachmentReference> m_InputAttachments;
		std::vector<VkAttachmentReference> m_ColorAttachments;
		std::vector<VkAttachmentReference> m_ResolveAttachments;
		std::optional<VkAttachmentReference> m_DepthAttachment;
		std::vector<uint32_t> m_PreserveAttachments;
	};
}  // namespace At0::Ray
