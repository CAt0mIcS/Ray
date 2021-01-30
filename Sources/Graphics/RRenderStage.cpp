#include "Rpch.h"
#include "RRenderStage.h"

#include "Devices/RWindow.h"
#include "Core/RTime.h"
#include "Graphics/RGraphics.h"
#include "Debug/RLogger.h"
#include "Resources/RImageDepth.h"
#include "Renderpass/RFramebuffers.h"
#include "Renderpass/RRenderpass.h"
#include "Descriptors/RDescriptors.h"

#include "Devices/Vulkan/RPhysicalDevice.h"
#include "Devices/Vulkan/RSurface.h"
#include "Graphics/Renderpass/RSwapchain.h"


namespace At0::Ray
{
	RenderStage::RenderStage(std::vector<Attachment> images, std::vector<SubpassType> subpasses,
		const Viewport& viewport)
		: m_Attachments(std::move(images)), m_Subpasses(std::move(subpasses)), m_Viewport(viewport),
		  m_SubpassAttachmentCount(m_Subpasses.size()), m_SubpassMultisampled(m_Subpasses.size())
	{
		for (const Attachment& image : m_Attachments)
		{
			VkClearValue clearValue{};

			switch (image.GetType())
			{
			case Attachment::Type::Image:
			{
				clearValue.color = { { image.GetClearColour().r, image.GetClearColour().g,
					image.GetClearColour().b, image.GetClearColour().a } };

				for (const SubpassType& subpass : m_Subpasses)
				{
					if (auto subpassBindings = subpass.GetAttachmentBindings();
						std::find(subpassBindings.begin(), subpassBindings.end(),
							image.GetBinding()) != subpassBindings.end())
					{
						m_SubpassAttachmentCount[subpass.GetBinding()]++;

						if (image.IsMultisampled())
							m_SubpassMultisampled[subpass.GetBinding()] = true;
					}
				}
				break;
			}
			case Attachment::Type::Depth:
			{
				clearValue.depthStencil = { 1.0f, 0 };
				m_DepthAttachment = image;
				break;
			}
			case Attachment::Type::Swapchain:
			{
				clearValue.color = { { image.GetClearColour().r, image.GetClearColour().g,
					image.GetClearColour().b, image.GetClearColour().a } };
				m_SwapchainAttachment = image;
				break;
			}
			}

			m_ClearValues.emplace_back(clearValue);
		}
	}

	RenderStage::~RenderStage() {}

	void RenderStage::Update()
	{
		RenderArea lastRenderArea = m_RenderArea;
		m_RenderArea.SetOffset(m_Viewport.GetOffset());

		if (m_Viewport.GetSize())
		{
			m_RenderArea.SetExtent(
				{ (uint32_t)m_Viewport.GetScale().x * (uint32_t)m_Viewport.GetSize()->x,
					(uint32_t)m_Viewport.GetScale().y * (uint32_t)m_Viewport.GetSize()->y });
		}
		else
		{
			m_RenderArea.SetExtent(
				{ (uint32_t)m_Viewport.GetScale().x * (uint32_t)Window::Get().GetSize().x,
					(uint32_t)m_Viewport.GetScale().y * (uint32_t)Window::Get().GetSize().y });
		}

		m_RenderArea.SetAspectRatio(
			float(m_RenderArea.GetExtent().x) / float(m_RenderArea.GetExtent().y));
		m_RenderArea.SetExtent({ m_RenderArea.GetExtent().x + m_RenderArea.GetOffset().x,
			m_RenderArea.GetExtent().y + m_RenderArea.GetOffset().y });

		m_OutOfDate = m_RenderArea != lastRenderArea;
	}

	void RenderStage::Rebuild(const Swapchain& swapchain)
	{
#ifndef NDEBUG
		Time startTime = Time::Now();
#endif

		Update();

		const PhysicalDevice& device = Graphics::Get().GetPhysicalDevice();
		const Surface& surface = Graphics::Get().GetSurface();

		auto msaaSamples = device.GetMsaaSamples();

		// RAY_TODO: Possible bug:
		// m_DepthStecil but !m_Renderpass

		if (m_DepthAttachment)
			m_DepthStencil = MakeScope<ImageDepth>(m_RenderArea.GetExtent(),
				m_DepthAttachment->IsMultisampled() ? msaaSamples : VK_SAMPLE_COUNT_1_BIT);

		if (!m_Renderpass)
			m_Renderpass = MakeScope<Renderpass>(*this,
				m_DepthStencil ? m_DepthStencil->GetFormat() : VK_FORMAT_UNDEFINED,
				surface.GetFormat().format, msaaSamples);

		m_Framebuffers = MakeScope<Framebuffers>(m_RenderArea.GetExtent(), *this, *m_Renderpass,
			swapchain, *m_DepthStencil, msaaSamples);
		m_OutOfDate = false;

		m_Descriptors.clear();

		for (const Attachment& image : m_Attachments)
		{
			if (image.GetType() == Attachment::Type::Depth)
				m_Descriptors.emplace(image.GetName(), m_DepthStencil.get());
			else
				m_Descriptors.emplace(
					image.GetName(), m_Framebuffers->GetAttachment(image.GetBinding()));
		}

#ifndef NDEBUG
		Log::Debug("[RenderStage] Render stage was rebuilt in {0}ms",
			(Time::Now() - startTime).AsMilliseconds());
#endif
	}

	std::optional<Attachment> RenderStage::GetAttachment(std::string_view name) const
	{
		auto it = std::find_if(m_Attachments.begin(), m_Attachments.end(),
			[name](const Attachment& a) { return a.GetName() == name; });

		if (it != m_Attachments.end())
			return *it;
		return std::nullopt;
	}

	std::optional<Attachment> RenderStage::GetAttachment(uint32_t binding) const
	{
		auto it = std::find_if(m_Attachments.begin(), m_Attachments.end(),
			[binding](const Attachment& a) { return a.GetBinding() == binding; });

		if (it != m_Attachments.end())
			return *it;
		return std::nullopt;
	}

	const Descriptor* RenderStage::GetDescriptor(std::string_view name) const
	{
		auto it = m_Descriptors.find(name.data());

		if (it != m_Descriptors.end())
			return it->second;
		return nullptr;
	}

	const VkFramebuffer_T*& RenderStage::GetActiveFramebuffer(uint32_t activeSwapchainImage) const
	{
		if (activeSwapchainImage > m_Framebuffers->GetFramebuffers().size())
			return (const VkFramebuffer_T*&)m_Framebuffers->GetFramebuffers()[0];

		return (const VkFramebuffer_T*&)m_Framebuffers->GetFramebuffers()[activeSwapchainImage];
	}
}  // namespace At0::Ray
