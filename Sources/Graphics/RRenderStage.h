#pragma once

#include "../RBase.h"
#include "../Core/RColor.h"
#include "../Core/RMath.h"

#include <optional>
#include <vector>
#include <string>

enum VkFormat;
struct VkFramebuffer_T;

namespace At0::Ray
{
	class RAY_EXPORT Attachment
	{
	public:
		enum class Type
		{
			Image,
			Depth,
			Swapchain
		};

		/**
		 * Creates a new attachment that represents a object in the render pipeline.
		 * @param binding The index the attachment is bound to in the render pass.
		 * @param name The unique name given to the object for all render passes.
		 * @param type The attachment type this represents
		 */
		Attachment(uint32_t binding, std::string_view name, Type type, bool multisampled = false,
			VkFormat format = (VkFormat)37, const Color& clearColor = Color::Black)
			: m_Binding(binding), m_Name(name), m_Type(type), m_Multisampled(multisampled),
			  m_Format(format), m_ClearColor(clearColor)
		{
		}

		uint32_t GetBinding() const { return m_Binding; }
		std::string_view GetName() const { return m_Name; }
		Type GetType() const { return m_Type; }
		bool IsMultisampled() const { return m_Multisampled; }
		VkFormat GetFormat() const { return m_Format; }
		const Color& GetClearColour() const { return m_ClearColor; }

	private:
		uint32_t m_Binding;
		std::string m_Name;
		Type m_Type;
		bool m_Multisampled;
		VkFormat m_Format;
		Color m_ClearColor;
	};


	class RAY_EXPORT SubpassType
	{
	public:
		SubpassType(uint32_t binding, std::vector<uint32_t> attachmentBindings)
			: m_Binding(binding), m_AttachmentBindings(attachmentBindings)
		{
		}

		uint32_t GetBinding() const { return m_Binding; }
		const std::vector<uint32_t>& GetAttachmentBindings() const { return m_AttachmentBindings; }

	private:
		uint32_t m_Binding;
		std::vector<uint32_t> m_AttachmentBindings;
	};


	class RAY_EXPORT RenderArea
	{
	public:
		RenderArea(const UInt2& extent = {}, const Int2& offset = {})
			: m_Extent(extent), m_Offset(offset)
		{
		}

		bool operator==(const RenderArea& rhs) const
		{
			return m_Extent == rhs.m_Extent && m_Offset == rhs.m_Offset;
		}

		bool operator!=(const RenderArea& rhs) const { return !operator==(rhs); }

		const UInt2& GetExtent() const { return m_Extent; }
		void SetExtent(const UInt2& extent) { m_Extent = extent; }

		const Int2& GetOffset() const { return m_Offset; }
		void SetOffset(const Int2& offset) { m_Offset = offset; }

		/**
		 * Gets the aspect ratio between the render stages width and height.
		 * @return The aspect ratio.
		 */
		float GetAspectRatio() const { return m_AspectRatio; }
		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }

	private:
		UInt2 m_Extent;
		Int2 m_Offset;
		float m_AspectRatio = 1.0f;
	};


	class RAY_EXPORT Viewport
	{
	public:
		Viewport() = default;
		Viewport(const UInt2& size) : m_Size(size) {}

		const Float2& GetScale() const { return m_Scale; }
		void SetScale(const Float2& scale) { m_Scale = scale; }

		const std::optional<UInt2>& GetSize() const { return m_Size; }
		void SetSize(const std::optional<UInt2>& size) { m_Size = size; }

		const Int2& GetOffset() const { return m_Offset; }
		void SetOffset(const Int2& offset) { m_Offset = offset; }

	private:
		Float2 m_Scale{ 1.0f, 1.0f };
		std::optional<UInt2> m_Size;
		Int2 m_Offset;
	};


	class Swapchain;
	class Descriptor;
	class Renderpass;
	class ImageDepth;
	class Framebuffers;
	class RAY_EXPORT RenderStage
	{
	public:
		RenderStage(std::vector<Attachment> images = {}, std::vector<SubpassType> subpasses = {},
			const Viewport& viewport = Viewport());
		~RenderStage();

		void Update();
		void Rebuild(const Swapchain& swapchain);

		std::optional<Attachment> GetAttachment(std::string_view name) const;
		std::optional<Attachment> GetAttachment(uint32_t binding) const;

		const Descriptor* GetDescriptor(std::string_view name) const;
		const VkFramebuffer_T*& GetActiveFramebuffer(uint32_t activeSwapchainImage) const;

		const std::vector<Attachment>& GetAttachments() const { return m_Attachments; }
		const std::vector<SubpassType>& GetSubpasses() const { return m_Subpasses; }

		Viewport& GetViewport() { return m_Viewport; }
		void SetViewport(const Viewport& viewport) { m_Viewport = viewport; }

		/**
		 * Gets the render stage viewport.
		 * @return The the render stage viewport.
		 */
		const RenderArea& GetRenderArea() const { return m_RenderArea; }

		/**
		 * Gets if the width or height has changed between the last update and now.
		 * @return If the width or height has changed.
		 */
		bool IsOutOfDate() const { return m_OutOfDate; }

		const Renderpass& GetRenderpass() const { return *m_Renderpass.get(); }
		const ImageDepth& GetDepthStencil() const { return *m_DepthStencil.get(); }
		const Framebuffers& GetFramebuffers() const { return *m_Framebuffers.get(); }
		const std::vector<VkClearValue>& GetClearValues() const { return m_ClearValues; }
		uint32_t GetAttachmentCount(uint32_t subpass) const
		{
			return m_SubpassAttachmentCount[subpass];
		}
		bool HasDepth() const { return m_DepthAttachment.has_value(); }
		bool HasSwapchain() const { return m_SwapchainAttachment.has_value(); }
		bool IsMultisampled(uint32_t subpass) const { return m_SubpassMultisampled[subpass]; }

	private:
		std::vector<Attachment> m_Attachments;
		std::vector<SubpassType> m_Subpasses;

		Viewport m_Viewport;

		Scope<Renderpass> m_Renderpass;
		Scope<ImageDepth> m_DepthStencil;
		Scope<Framebuffers> m_Framebuffers;

		std::map<std::string, const Descriptor*> m_Descriptors;

		std::vector<VkClearValue> m_ClearValues;
		std::vector<uint32_t> m_SubpassAttachmentCount;
		std::optional<Attachment> m_DepthAttachment;
		std::optional<Attachment> m_SwapchainAttachment;
		std::vector<bool> m_SubpassMultisampled;

		RenderArea m_RenderArea;
		bool m_OutOfDate = false;
	};
}  // namespace At0::Ray
