﻿#include "Rpch.h"
#include "RSubpass.h"

#include "Utils/RLogger.h"


namespace At0::Ray
{
	void Subpass::AddColorAttachment(const Attachment& attachment)
	{
		VkAttachmentReference reference{};
		reference.attachment = m_AttachmentIndex++;
		reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		m_ColorAttachments.emplace_back(reference);
		UpdateDescription();
	}

	void Subpass::AddDepthAttachment(const Attachment& attachment)
	{
#ifndef NDEBUG
		if (m_DepthAttachment.layout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL)
			Log::Warn("[Subpass] Depth attachment already set");
#endif

		m_DepthAttachment.attachment = m_AttachmentIndex++;
		m_DepthAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
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
		m_Description.pDepthStencilAttachment = &m_DepthAttachment;
		m_Description.preserveAttachmentCount = (uint32_t)m_PreserveAttachments.size();
		m_Description.pPreserveAttachments = m_PreserveAttachments.data();
	}
}  // namespace At0::Ray