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

		m_VertexBuffer->CmdBind(cmdBuff);
		m_IndexBuffer->CmdBind(cmdBuff);
	}

	Mesh::~Mesh() {}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		m_VertexBuffer = std::move(other.m_VertexBuffer);
		m_IndexBuffer = std::move(other.m_IndexBuffer);

		m_Material = std::move(other.m_Material);
		m_Uniforms = std::move(other.m_Uniforms);

		m_GlobalUniformBufferOffset = other.m_GlobalUniformBufferOffset;

		if (other.EntitySet())
			SetEntity(other.GetEntity());
		return *this;
	}

	Mesh::Mesh(Mesh&& other) noexcept
		: m_VertexBuffer(std::move(other.m_VertexBuffer)),
		  m_IndexBuffer(std::move(other.m_IndexBuffer)), m_Material(std::move(other.m_Material)),
		  m_Uniforms(std::move(other.m_Uniforms)),
		  m_GlobalUniformBufferOffset(other.m_GlobalUniformBufferOffset)
	{
		if (other.EntitySet())
			SetEntity(other.GetEntity());
	}

	Mesh::Mesh(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Material material)
		: m_VertexBuffer(std::move(vertexBuffer)), m_IndexBuffer(std::move(indexBuffer)),
		  m_Material(std::move(material)), m_Uniforms(m_Material.GetGraphicsPipeline())
	{
		uint32_t alignment = Graphics::Get()
								 .GetPhysicalDevice()
								 .GetProperties()
								 .limits.minUniformBufferOffsetAlignment;
		BufferSynchronizer::Get().Emplace(sizeof(Matrix), alignment, &m_GlobalUniformBufferOffset);
	}
}  // namespace At0::Ray