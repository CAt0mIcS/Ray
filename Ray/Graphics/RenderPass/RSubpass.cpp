#include "Rpch.h"
#include "RSubpass.h"

#include "Ray/Utils/RLogger.h"


namespace At0::Ray
{
	void Subpass::AddInputAttachment(uint32_t index, const Attachment& attachment)
	{
		RrAttachmentReference reference{};
		reference.attachment = index;
		// Layout the image will have once the subpass is started
		reference.layout = RrImageLayoutShaderReadOnly;
		m_InputAttachments.emplace_back(reference);
		UpdateDescription();
	}

	void Subpass::AddColorAttachment(uint32_t index, const Attachment& attachment)
	{
		RrAttachmentReference reference{};
		reference.attachment = index;

		// Layout the image will have once the subpass is started
		reference.layout = RrImageLayoutColorAttachment;
		m_ColorAttachments.emplace_back(reference);
		UpdateDescription();
	}

	void Subpass::AddDepthAttachment(uint32_t index, const Attachment& attachment)
	{
#ifndef NDEBUG
		if (m_DepthAttachment)
			Log::Warn("[Subpass] Depth attachment already set");
#endif

		m_DepthAttachment = RrAttachmentReference{};
		m_DepthAttachment->attachment = index;
		// Layout the image will have once the subpass is started
		m_DepthAttachment->layout = RrImageLayoutDepthStencilAttachment;
		UpdateDescription();
	}

	void Subpass::AddPreserveAttachment(uint32_t index)
	{
		m_PreserveAttachments.emplace_back(index);
		UpdateDescription();
	}

	void Subpass::UpdateDescription()
	{
		m_Description.pipelineBindPoint = RrPipelineBindPointGraphics;
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
