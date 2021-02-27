#include "Rpch.h"
#include "RMesh.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RCodex.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/RUniformAccess.h"
#include "Graphics/Buffers/RBufferSynchronizer.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Commands/RCommandBuffer.h"

#include "Core/REntity.h"
#include "Core/RVertex.h"

#include "RTransform.h"


namespace At0::Ray
{
	Mesh Mesh::Triangle(Material material)
	{
		VertexInput vertexInput(material.GetVertexLayout());
		vertexInput.Emplace(Float3{ -0.5f, -0.5f, 0.0f });
		vertexInput.Emplace(Float3{ 0.5f, -0.5f, 0.0f });
		vertexInput.Emplace(Float3{ 0.0f, 0.5f, 0.0f });

		std::vector<IndexBuffer::Type> indices{ 0, 1, 2 };

		Ref<VertexBuffer> vertexBuffer = Codex::Resolve<VertexBuffer>("Triangle", vertexInput);
		Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>("012", indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	void Mesh::Update(Delta ts)
	{
		m_Uniforms.Resolve<Shader::Stage::Vertex>("Transforms", "model")
			.Update(GetEntity().Get<Transform>().ToMatrix(), m_GlobalUniformBufferOffset);
	}

	void Mesh::Bind(const CommandBuffer& cmdBuff) const
	{
		m_Material.GetGraphicsPipeline().CmdBind(cmdBuff);

		m_DescriptorSet.CmdBind(cmdBuff);
		m_VertexBuffer->CmdBind(cmdBuff);
		m_IndexBuffer->CmdBind(cmdBuff);
	}

	void Mesh::Render(const CommandBuffer& cmdBuff) const
	{
		vkCmdDrawIndexed(cmdBuff, m_IndexBuffer->GetNumberOfIndices(), 1, 0, 0, 0);
	}

	Mesh::~Mesh() {}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		m_VertexBuffer = std::move(other.m_VertexBuffer);
		m_IndexBuffer = std::move(other.m_IndexBuffer);

		m_Material = std::move(other.m_Material);
		m_Uniforms = std::move(other.m_Uniforms);
		m_DescriptorSet = std::move(other.m_DescriptorSet);

		m_GlobalUniformBufferOffset = other.m_GlobalUniformBufferOffset;

		if (other.EntitySet())
			SetEntity(other.GetEntity());
		return *this;
	}

	Mesh::Mesh(Mesh&& other) noexcept
		: m_VertexBuffer(std::move(other.m_VertexBuffer)),
		  m_IndexBuffer(std::move(other.m_IndexBuffer)), m_Material(std::move(other.m_Material)),
		  m_Uniforms(std::move(other.m_Uniforms)),
		  m_GlobalUniformBufferOffset(other.m_GlobalUniformBufferOffset),
		  m_DescriptorSet(std::move(other.m_DescriptorSet))
	{
		if (other.EntitySet())
			SetEntity(other.GetEntity());
	}

	Mesh::Mesh(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Material material)
		: m_VertexBuffer(std::move(vertexBuffer)), m_IndexBuffer(std::move(indexBuffer)),
		  m_Material(std::move(material)), m_Uniforms(m_Material.GetGraphicsPipeline()),
		  m_DescriptorSet(m_Material.GetGraphicsPipeline(), DescriptorSet::Frequency::PerObject)
	{
		// Allocate uniform buffer memory
		uint32_t alignment = Graphics::Get()
								 .GetPhysicalDevice()
								 .GetProperties()
								 .limits.minUniformBufferOffsetAlignment;
		BufferSynchronizer::Get().Emplace(sizeof(Matrix), alignment, &m_GlobalUniformBufferOffset);

		// Update descriptor set
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer =
			BufferSynchronizer::Get().GetUniformBuffer().GetBuffer(m_GlobalUniformBufferOffset);
		bufferInfo.offset =
			BufferSynchronizer::Get().GetUniformBuffer().GetOffset(m_GlobalUniformBufferOffset);
		bufferInfo.range = sizeof(Matrix);

		VkWriteDescriptorSet descWrite{};
		descWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descWrite.dstSet = m_DescriptorSet;
		descWrite.dstBinding = (uint32_t)m_DescriptorSet.GetFrequency();
		descWrite.dstArrayElement = 0;
		descWrite.descriptorCount = 1;
		descWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descWrite.pImageInfo = nullptr;
		descWrite.pBufferInfo = &bufferInfo;
		descWrite.pTexelBufferView = nullptr;

		DescriptorSet::Update({ descWrite });
	}
}  // namespace At0::Ray
