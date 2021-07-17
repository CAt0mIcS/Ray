#pragma once

#include "Ray/RBase.h"

#include <RayRenderer/Core/RRenderPass.h>
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

		operator const RrSubpassDescription&() const { return m_Description; }

	private:
		void UpdateDescription();

	private:
		RrSubpassDescription m_Description{};

		std::vector<RrAttachmentReference> m_InputAttachments;
		std::vector<RrAttachmentReference> m_ColorAttachments;
		std::vector<RrAttachmentReference> m_ResolveAttachments;
		std::optional<RrAttachmentReference> m_DepthAttachment;
		std::vector<uint32_t> m_PreserveAttachments;
	};
}  // namespace At0::Ray
