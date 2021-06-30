#include "Rpch.h"
#include "RCommandBuffer.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "RCommandPool.h"

#include "RayBase/RException.h"
#include "Core/RRendererLoader.h"


namespace At0::Ray
{
	CommandBuffer::CommandBuffer(const CommandPool& commandPool, RrCommandBufferLevel bufferLevel)
		: m_CommandPool(&commandPool)
	{
		RrCommandBufferAllocateInfo allocInfo{};
		allocInfo.commandPool = *m_CommandPool;
		allocInfo.level = bufferLevel;
		allocInfo.commandBufferCount = 1;

		ThrowRenderError(RendererAPI::AllocateCommandBuffers(
							 Graphics::Get().GetDevice(), &allocInfo, &m_CommandBuffer),
			"[CommandBuffer] Failed to allocate command buffer");
	}

	CommandBuffer::~CommandBuffer()
	{
		RendererAPI::FreeCommandBuffers(
			Graphics::Get().GetDevice(), *m_CommandPool, 1, &m_CommandBuffer);
	}

	void CommandBuffer::Begin(RrCommandBufferUsageFlags usageFlags) const
	{
		RrCommandBufferBeginInfo beginInfo{};
		beginInfo.flags = usageFlags;
		beginInfo.pInheritanceInfo = GetInheritanceInfo();

		ThrowRenderError(RendererAPI::BeginCommandBuffer(m_CommandBuffer, &beginInfo),
			"[CommandBuffer] Failed to begin recording");
	}

	void CommandBuffer::End() const
	{
		ThrowRenderError(RendererAPI::EndCommandBuffer(m_CommandBuffer),
			"[CommandBuffer] Failed to end recording");
	}

	RrError CommandBuffer::Submit(RrQueue queue, RrFence fence) const
	{
		RrSubmitInfo submitInfo{};
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffer;

		return RendererAPI::QueueSubmit(queue, 1, &submitInfo, fence);
	}

	void CommandBuffer::Execute(const SecondaryCommandBuffer& secCmdBuff) const
	{
		RrCommandBuffer buff = secCmdBuff;
		RendererAPI::ExecuteCommands(m_CommandBuffer, 1, &buff);
	}

	SecondaryCommandBuffer& CommandBuffer::AddSecondary(
		RrCommandBufferInheritanceInfo inheritanceInfo)
	{
		return m_SecondaryCommandBuffers.emplace_back(*m_CommandPool, std::move(inheritanceInfo));
	}

	CommandBuffer& CommandBuffer::operator=(CommandBuffer&& other) noexcept
	{
		m_CommandBuffer = other.m_CommandBuffer;
		m_CommandPool = other.m_CommandPool;
		m_SecondaryCommandBuffers = std::move(other.m_SecondaryCommandBuffers);
		return *this;
	}

	CommandBuffer::CommandBuffer(CommandBuffer&& other) noexcept { *this = std::move(other); }

	// -----------------------------------------------------------------------------------------------
	// Secondary Command Buffer
	SecondaryCommandBuffer::SecondaryCommandBuffer(
		const CommandPool& commandPool, RrCommandBufferInheritanceInfo inheritanceInfo)
		: CommandBuffer(commandPool, RrcommandBufferLevelSecondary),
		  m_InheritanceInfo(std::move(inheritanceInfo))
	{
	}

	const RrCommandBufferInheritanceInfo* SecondaryCommandBuffer::GetInheritanceInfo() const
	{
		return &m_InheritanceInfo;
	}
}  // namespace At0::Ray
