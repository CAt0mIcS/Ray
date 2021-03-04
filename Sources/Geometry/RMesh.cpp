#include "Rpch.h"
#include "RMesh.h"

#include "Graphics/RGraphics.h"
#include "Graphics/RCodex.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/RUniformAccess.h"
#include "Graphics/Buffers/RBufferSynchronizer.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Commands/RCommandBuffer.h"

#include "Registry/RGeometricPrimitives.h"
#include "Scene/REntity.h"
#include "Graphics/RVertex.h"


namespace At0::Ray
{
	Mesh::Mesh(Entity& entity, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer,
		Material material)
		: Component(entity), m_VertexBuffer(std::move(vertexBuffer)),
		  m_IndexBuffer(std::move(indexBuffer)), m_Material(std::move(material)),
		  m_DescriptorSet(m_Material.GetGraphicsPipeline().GetDescriptorPool(),
			  m_Material.GetGraphicsPipeline()
				  .GetDescriptorSetLayouts()[(size_t)DescriptorSet::Frequency::PerObject],
			  m_Material.GetGraphicsPipeline().GetBindPoint(),
			  m_Material.GetGraphicsPipeline().GetLayout(), DescriptorSet::Frequency::PerObject)
	{
		Setup();
	}

	Mesh::Mesh(Entity& entity, const MeshData& data)
		: Component(entity), m_VertexBuffer(std::move(data.vertexBuffer)),
		  m_IndexBuffer(std::move(data.indexBuffer)), m_Material(std::move(data.material)),
		  m_DescriptorSet(m_Material.GetGraphicsPipeline().GetDescriptorPool(),
			  m_Material.GetGraphicsPipeline()
				  .GetDescriptorSetLayouts()[(size_t)DescriptorSet::Frequency::PerObject],
			  m_Material.GetGraphicsPipeline().GetBindPoint(),
			  m_Material.GetGraphicsPipeline().GetLayout(), DescriptorSet::Frequency::PerObject)
	{
		Setup();
	}

	Mesh::MeshData Mesh::Triangle(Material material)
	{
		IndexedTriangleList triangle = IndexedTriangleList::Triangle(material.GetVertexLayout());

		Ref<VertexBuffer> vertexBuffer =
			Codex::Resolve<VertexBuffer>(triangle.vertexTag, triangle.vertices);
		Ref<IndexBuffer> indexBuffer =
			Codex::Resolve<IndexBuffer>(triangle.indexTag, triangle.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	Mesh::MeshData Mesh::Plane(Material material)
	{
		IndexedTriangleList plane = IndexedTriangleList::Plane(material.GetVertexLayout());

		Ref<VertexBuffer> vertexBuffer =
			Codex::Resolve<VertexBuffer>(plane.vertexTag, plane.vertices);
		Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(plane.indexTag, plane.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	// Mesh::MeshData Mesh::Circle(Material material, int segments, float radius)
	//{
	// material.Create();
	//	IndexedTriangleList circle =
	//		IndexedTriangleList::Circle(material.GetVertexLayout(), segments, radius);

	//	Ref<VertexBuffer> vertexBuffer =
	//		Codex::Resolve<VertexBuffer>(circle.vertexTag, circle.vertices);
	//	Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(circle.indexTag, circle.indices);

	//	return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	//}

	Mesh::MeshData Mesh::Cube(Material material)
	{
		IndexedTriangleList cube = IndexedTriangleList::Cube(material.GetVertexLayout());

		Ref<VertexBuffer> vertexBuffer =
			Codex::Resolve<VertexBuffer>(cube.vertexTag, cube.vertices);
		Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(cube.indexTag, cube.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	// Mesh::MeshData Mesh::IcoSphere(Material material) { return Mesh(); }

	// Mesh::MeshData Mesh::UVSphere(Material material) { return Mesh(); }

	void Mesh::Update(Delta ts, const Transform& parentTransform)
	{
		m_Uniforms.Resolve<Shader::Stage::Vertex>("PerObjectData", "model") =
			(m_Transform + parentTransform).AsMatrix();
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

		m_Transform = std::move(other.m_Transform);
		return *this;
	}

	Mesh::Mesh(Mesh&& other) noexcept
		: Component(*other.m_Entity), m_VertexBuffer(std::move(other.m_VertexBuffer)),
		  m_IndexBuffer(std::move(other.m_IndexBuffer)), m_Material(std::move(other.m_Material)),
		  m_Uniforms(std::move(other.m_Uniforms)),
		  m_DescriptorSet(std::move(other.m_DescriptorSet)),
		  m_Transform(std::move(other.m_Transform))
	{
	}

	void Mesh::Setup()
	{
		// Allocate uniform buffer memory
		uint32_t alignment = Graphics::Get()
								 .GetPhysicalDevice()
								 .GetProperties()
								 .limits.minUniformBufferOffsetAlignment;
		uint32_t globalUniformBufferOffset = 0;
		BufferSynchronizer::Get().Emplace(sizeof(Matrix), alignment, &globalUniformBufferOffset);

		m_Uniforms = UniformAccess(m_Material.GetGraphicsPipeline(), globalUniformBufferOffset);

		// Update descriptor set
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer =
			BufferSynchronizer::Get().GetUniformBuffer().GetBuffer(globalUniformBufferOffset);
		bufferInfo.offset =
			BufferSynchronizer::Get().GetUniformBuffer().GetOffset(globalUniformBufferOffset);
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
