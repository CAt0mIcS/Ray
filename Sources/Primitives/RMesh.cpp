#include "Rpch.h"
#include "RMesh.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Commands/RCommandBuffer.h"

#include "Core/RVertex.h"


namespace At0::Ray
{
	Mesh::Mesh()
	{
		graphicsPipeline = MakeScope<GraphicsPipeline>(Graphics::Get().GetRenderPass(),
			std::vector<std::string>{
				"Resources/Shaders/DefaultShader.vert", "Resources/Shaders/DefaultShader.frag" });

		VertexInput vertexInput(graphicsPipeline->GetVertexLayout());
		// clang-format off
		vertexInput.Emplace(Float3{ -0.5f, -0.5f, 0.0f }, Float3{ 1.0f, 0.0f, 0.0f });
		vertexInput.Emplace(Float3{  0.5f, -0.5f, 0.0f }, Float3{ 0.0f, 1.0f, 0.0f });
		vertexInput.Emplace(Float3{  0.0f,  0.5f, 0.0f }, Float3{ 0.0f, 0.0f, 1.0f });
		// clang-format on

		vertexBuffer = MakeScope<VertexBuffer>(vertexInput);

		std::vector<IndexBuffer::Type> indices{ 0, 1, 2 };
		indexBuffer = MakeScope<IndexBuffer>(indices);
	}

	Mesh::~Mesh() {}

	void Mesh::CmdBind(const CommandBuffer& cmdBuff)
	{
		VkDeviceSize offsets[] = { 0 };
		VkBuffer vbuff = *vertexBuffer;
		VkBuffer idxBuff = *indexBuffer;
		vkCmdBindVertexBuffers(cmdBuff, 0, 1, &vbuff, offsets);
		vkCmdBindIndexBuffer(cmdBuff, idxBuff, 0, VK_INDEX_TYPE_UINT16);
	}

	void Mesh::CmdDraw(const CommandBuffer& cmdBuff)
	{
		vkCmdDrawIndexed(cmdBuff, indexBuffer->GetNumberOfIndices(), 1, 0, 0, 0);
	}
}  // namespace At0::Ray
