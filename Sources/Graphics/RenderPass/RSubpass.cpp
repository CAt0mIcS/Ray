#include "Rpch.h"
#include "RSubpass.h"

#include "Utils/RLogger.h"


namespace At0::Ray
{
	void Subpass::AddInputAttachment(uint32_t index, const Attachment& attachment)
	{
		VkAttachmentReference reference{};
		reference.attachment = index;
		reference.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_InputAttachments.emplace_back(reference);
		UpdateDescription();
	}

	void Subpass::AddColorAttachment(uint32_t index, const Attachment& attachment)
	{
		VkAttachmentReference reference{};
		reference.attachment = index;
		reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		m_ColorAttachments.emplace_back(reference);
		UpdateDescription();
	}

	void Subpass::AddDepthAttachment(uint32_t index, const Attachment& attachment)
	{
#ifndef NDEBUG
		if (m_DepthAttachment)
			Log::Warn("[Subpass] Depth attachment already set");
#endif

		m_DepthAttachment = VkAttachmentReference{};
		m_DepthAttachment->attachment = index;
		m_DepthAttachment->layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		UpdateDescription();
	}

	void Subpass::AddPreserveAttachment(uint32_t index)
	{
		m_PreserveAttachments.emplace_back(index);
		UpdateDescription();
	}

	void Subpass::UpdateDescription()
	{
		m_Description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		m_Description.inputAttachmentCount = (uint32_t)m_InputAttachments.size();
		m_Description.pInputAttachments = m_InputAttachments.data();
		m_Description.colorAttachmentCount = (uint32_t)m_ColorAttachments.size();
		m_Description.pColorAttachments = m_ColorAttachments.data();
		m_Description.pResolveAttachments = m_ResolveAttachments.data();
		m_Description.pDepthStencilAttachment = m_DepthAttachment ? &*m_DepthAttachment : nullptr;
		m_Description.preserveAttachmentCount = (uint32_t)m_PreserveAttachments.size();
		m_Description.pPreserveAttachments = m_PreserveAttachments.data();
	}
}  // namespace At0::Ray
