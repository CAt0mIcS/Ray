#include "RCommandBufferRecorder.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RSwapchain.h"

#include "RCommandBuffer.h"

#include "../RenderPass/RRenderPass.h"
#include "../Buffers/RFramebuffer.h"

#include "Scene/RScene.h"
#include "Components/RMeshRenderer.h"
#include "Components/RMesh.h"
#include "Utils/RImGui.h"


namespace At0::Ray
{
	CommandBufferRecorder::CommandBufferRecorder(uint32_t numThreads, uint32_t numPools,
		const RenderPass& renderPass, uint32_t subpassID,
		const std::vector<Scope<Framebuffer>>& framebuffers)
		: m_ThreadPool(numThreads), m_Context(renderPass.GetRenderContext())
	{
		m_MainCommandResources.resize(numPools);
		for (uint32_t i = 0; i < numPools; ++i)
		{
			m_MainCommandResources[i] =
				PrimaryResources{ MakeScope<CommandPool>(m_Context), nullptr };
			m_MainCommandResources[i].commandBuffer =
				MakeScope<CommandBuffer>(*m_MainCommandResources[i].commandPool);
		}

		m_CommandResources.resize(numPools);
		for (auto& resources : m_CommandResources)
			resources.resize(numThreads);

		for (uint32_t i = 0; i < numPools; ++i)
		{
			for (uint32_t j = 0; j < numThreads; ++j)
			{
				m_CommandResources[i][j] =
					SecondaryResources{ MakeScope<CommandPool>(m_Context), nullptr };

				VkCommandBufferInheritanceInfo inheritanceInfo{};
				inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
				inheritanceInfo.renderPass = renderPass;
				inheritanceInfo.subpass = subpassID;
				inheritanceInfo.framebuffer = *framebuffers[i];

				m_CommandResources[i][j].commandBuffer = MakeScope<SecondaryCommandBuffer>(
					*m_CommandResources[i][j].commandPool, std::move(inheritanceInfo));
			}
		}
	}

	CommandBufferRecorder::~CommandBufferRecorder() {}

	void CommandBufferRecorder::Record(const RenderPass& renderPass, const Framebuffer& framebuffer,
		uint32_t imageIndex, const VkViewport& viewport, const VkRect2D& scissor,
		UInt2 swapchainExtent)
	{
		const CommandBuffer& mainCmdBuff = *m_MainCommandResources[imageIndex].commandBuffer;

		mainCmdBuff.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		VkClearValue clearValues[2];
		// clearValues[0].color = { 0.0137254f, 0.014117f, 0.0149019f };
		clearValues[0].color = { 0.f, 0.f, 0.f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPass.Begin(mainCmdBuff, framebuffer, clearValues, std::size(clearValues),
			swapchainExtent, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

		// Start secondary command buffers
		for (const auto& [commandPool, commandBuffer] : m_CommandResources[imageIndex])
		{
			commandBuffer->Begin(VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT |
								 VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
			vkCmdSetViewport(*commandBuffer, 0, 1, &viewport);
			vkCmdSetScissor(*commandBuffer, 0, 1, &scissor);
			// Scene::Get().CmdBind(*commandBuffer);
		}

		// RAY_TODO: Test group vs view performance
		auto meshRendererView = Scene::Get().GetRegistry().group<MeshRenderer>(entt::get<Mesh>);
		m_ThreadPool.SubmitLoop(0u, (uint32_t)meshRendererView.size(),
			[this, imageIndex, &meshRendererView](uint32_t i, uint32_t thread)
			{
				const auto& [meshRenderer, mesh] =
					meshRendererView.get<MeshRenderer, Mesh>(meshRendererView[i]);

				SecondaryCommandBuffer* cmdBuf =
					m_CommandResources[imageIndex][thread].commandBuffer.get();
				meshRenderer.Render(*cmdBuf);
				mesh.CmdBind(*cmdBuf);
			});

		// Wait for secondary command buffers to finish recording draw calls
		m_ThreadPool.WaitForTasks();

		for (const auto& [commandPool, commandBuffer] : m_CommandResources[imageIndex])
		{
			// Scene::Get().EntityView<TextComponent>().each(
			//	[&commandBuffer](TextComponent& renderer) { renderer.Render(*commandBuffer); });
#if RAY_ENABLE_IMGUI
			ImGUI::Get().CmdBind(*commandBuffer);
#endif
		}

		// End and execute secondary commmand buffers
		for (const auto& [commandPool, commandBuffer] : m_CommandResources[imageIndex])
		{
			commandBuffer->End();
			mainCmdBuff.Execute(*commandBuffer);
		}

		renderPass.End(mainCmdBuff);
		mainCmdBuff.End();
	}

	void CommandBufferRecorder::ResetCommandPools(uint32_t imageIndex) const
	{
		for (uint32_t thread = 0; thread < m_ThreadPool.GetThreadCount(); ++thread)
			vkResetCommandPool(
				m_Context.device, *m_CommandResources[imageIndex][thread].commandPool, 0);
		vkResetCommandPool(m_Context.device, *m_MainCommandResources[imageIndex].commandPool, 0);
	}

	const CommandBufferRecorder::PrimaryResources& CommandBufferRecorder::GetMainCommandResources(
		uint32_t imgIdx) const
	{
		return m_MainCommandResources[imgIdx];
	}
}  // namespace At0::Ray
