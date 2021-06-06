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
		: m_ThreadPool(numThreads)
	{
		m_CommandResources.reserve(numThreads * numPools);
		for (uint32_t i = 0; i < numPools; ++i)
		{
			for (uint32_t j = 0; j < m_ThreadPool.GetThreadCount(); ++j)
			{
				m_CommandResources.emplace_back(Resources{
					MakeScope<CommandPool>(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT),
					nullptr });
			}
		}

		m_ThreadPool.SubmitLoop(0u, (uint32_t)m_CommandResources.size(), [this](uint32_t i) {
			m_CommandResources[i].commandBuffer =
				MakeScope<CommandBuffer>(*m_CommandResources[i].commandPool);
		});
		m_ThreadPool.WaitForTasks();
	}

	CommandBufferRecorder::~CommandBufferRecorder() {}

	void CommandBufferRecorder::Record(const RenderPass& renderPass, const Framebuffer& framebuffer,
		uint32_t imageIndex, const VkViewport& viewport, const VkRect2D& scissor)
	{
		const CommandBuffer& cmdBuff = *m_CommandResources[imageIndex].commandBuffer;
		m_ThreadPool.Submit([&cmdBuff, &renderPass, &framebuffer, &viewport, &scissor]() {
			cmdBuff.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

			VkClearValue clearValues[2];
			clearValues[0].color = { 0.0137254f, 0.014117f, 0.0149019f };
			clearValues[1].depthStencil = { 1.0f, 0 };

			renderPass.Begin(cmdBuff, framebuffer, clearValues, std::size(clearValues));

			vkCmdSetViewport(cmdBuff, 0, 1, &viewport);
			vkCmdSetScissor(cmdBuff, 0, 1, &scissor);

			Scene::Get().CmdBind(cmdBuff);
			Scene::Get().EntityView<MeshRenderer>().each(
				[&cmdBuff](MeshRenderer& mesh) { mesh.Render(cmdBuff); });

#if RAY_ENABLE_IMGUI
			ImGUI::Get().CmdBind(cmdBuff);
#endif

			renderPass.End(cmdBuff);

			cmdBuff.End();
		});
	}
}  // namespace At0::Ray
