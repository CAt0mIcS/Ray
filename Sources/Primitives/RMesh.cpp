#include "Rpch.h"
#include "RMesh.h"

#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Commands/RCommandBuffer.h"


namespace At0::Ray
{
	Mesh::Mesh()
	{
		std::vector<Float3> vertices;
		vertices.emplace_back(-0.5f, -0.5f, 0.0f);
		vertices.emplace_back(0.5f, -0.5f, 0.0f);
		vertices.emplace_back(0.0f, 0.5f, 0.0f);
		vertexBuffer = MakeScope<VertexBuffer>(vertices);

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
