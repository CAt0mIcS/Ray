#include "Rpch.h"
#include "RCommandBufferRecorder.h"

#include "RCommandPool.h"
#include "RCommandBuffer.h"

#include "../RenderPass/RRenderPass.h"

#include "Scene/RScene.h"
#include "Components/RMeshRenderer.h"
#include "UI/RImGui.h"


namespace At0::Ray
{
	CommandBufferRecorder::CommandBufferRecorder(uint32_t numThreads, uint32_t numPools)
		: m_ThreadPool(numThreads), m_VkCommandBuffers(numPools), m_WaitSemaphores(numPools),
		  m_SignalSemaphores(numPools), m_PresentWaitSemaphores(numPools)
	{
		m_CommandResources.resize(numPools);
		for (auto& resources : m_CommandResources)
			resources.resize(numThreads);

		for (auto& vkCmdBuffers : m_VkCommandBuffers)
			vkCmdBuffers.resize(numThreads);

		// TEMPORARY
		for (auto& waitSemaphores : m_WaitSemaphores)
			waitSemaphores.resize(1);

		// TEMPORARY
		for (auto& signalSemaphores : m_SignalSemaphores)
			signalSemaphores.resize(1);

		// for (auto& presentWaitSemaphores : m_PresentWaitSemaphores)
		//	presentWaitSemaphores.resize();

		for (uint32_t i = 0; i < numPools; ++i)
		{
			for (uint32_t j = 0; j < numThreads; ++j)
			{
				m_CommandResources[i][j] = Resources{
					MakeScope<CommandPool>(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT), nullptr
				};
				m_CommandResources[i][j].commandBuffer =
					MakeScope<CommandBuffer>(*m_CommandResources[i][j].commandPool);
				m_VkCommandBuffers[i][j] = *m_CommandResources[i][j].commandBuffer;
			}
		}
	}

	CommandBufferRecorder::~CommandBufferRecorder() {}

	void CommandBufferRecorder::Record(const RenderPass& renderPass, const Framebuffer& framebuffer,
		uint32_t imageIndex, const VkViewport& viewport, const VkRect2D& scissor)
	{
		for (uint32_t thread = 0; thread < m_ThreadPool.GetThreadCount(); ++thread)
		{
			const CommandBuffer& cmdBuff = *m_CommandResources[imageIndex][thread].commandBuffer;
			cmdBuff.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

			VkClearValue clearValues[2];
			clearValues[0].color = { 0.0137254f, 0.014117f, 0.0149019f };
			clearValues[1].depthStencil = { 1.0f, 0 };

			renderPass.Begin(cmdBuff, framebuffer, clearValues, std::size(clearValues));

			vkCmdSetViewport(cmdBuff, 0, 1, &viewport);
			vkCmdSetScissor(cmdBuff, 0, 1, &scissor);
			Scene::Get().CmdBind(cmdBuff);
		}

		static auto meshRendererView = Scene::Get().EntityView<MeshRenderer>();
		m_ThreadPool.SubmitLoop(
			0u, (uint32_t)meshRendererView.size(), [this, imageIndex](uint32_t i, uint32_t thread) {
				Entity{ meshRendererView[i] }.Get<MeshRenderer>().Render(
					*m_CommandResources[imageIndex][thread].commandBuffer);
			});
		m_ThreadPool.WaitForTasks();

		for (uint32_t thread = 0; thread < m_ThreadPool.GetThreadCount(); ++thread)
		{
			const CommandBuffer& cmdBuff = *m_CommandResources[imageIndex][thread].commandBuffer;
#if RAY_ENABLE_IMGUI
			ImGUI::Get().CmdBind(cmdBuff);
#endif

			renderPass.End(cmdBuff);

			cmdBuff.End();
		}
	}

	void CommandBufferRecorder::FillSubmitInfo(uint32_t imageIndex,
		VkSemaphore imageAvailableSemaphore, VkSemaphore renderFinishedSemaphore,
		VkSubmitInfo& submitInfo)
	{
		submitInfo.commandBufferCount = m_ThreadPool.GetThreadCount();
		m_WaitSemaphores[imageIndex][0] = imageAvailableSemaphore;
		m_SignalSemaphores[imageIndex][0] = renderFinishedSemaphore;

		submitInfo.waitSemaphoreCount = (uint32_t)m_WaitSemaphores[imageIndex].size();
		submitInfo.pWaitSemaphores = m_WaitSemaphores[imageIndex].data();
		submitInfo.pCommandBuffers = m_VkCommandBuffers[imageIndex].data();

		submitInfo.signalSemaphoreCount = (uint32_t)m_SignalSemaphores[imageIndex].size();
		submitInfo.pSignalSemaphores = m_SignalSemaphores[imageIndex].data();
	}

	const std::vector<VkSemaphore>& CommandBufferRecorder::GetPresentWaitSemaphores(
		uint32_t imageIndex) const
	{
		return m_PresentWaitSemaphores[imageIndex];
	}

	const std::vector<CommandBufferRecorder::Resources>& CommandBufferRecorder::GetCommandResources(
		uint32_t imageIndex) const
	{
		return m_CommandResources[imageIndex];
	}
}  // namespace At0::Ray
