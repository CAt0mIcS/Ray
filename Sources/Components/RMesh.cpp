#include "Rpch.h"
#include "RMesh.h"

#include "Graphics/RGraphics.h"
#include "Graphics/RCodex.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Buffers/RBufferSynchronizer.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Images/RTexture2D.h"
#include "Graphics/Pipelines/Uniforms/RDescriptor.h"
#include "Graphics/Pipelines/Uniforms/RBufferUniform.h"

#include "Utils/RGeometricPrimitives.h"
#include "Scene/REntity.h"
#include "Utils/RException.h"


namespace At0::Ray
{
	const Mesh::Shaders Mesh::s_DefaultShaders = { "Resources/Shaders/DefaultShader.vert",
		"Resources/Shaders/DefaultShader.frag" };

	Mesh::Mesh(Entity entity, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer,
		Ref<Material> material, std::vector<MeshData> children)
		: Component(entity), m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer),
		  m_Material(std::move(material))
	{
		Setup(std::move(children));
	}

	Mesh::Mesh(Entity entity, MeshData data)
		: Component(entity), m_VertexBuffer(data.vertexBuffer), m_IndexBuffer(data.indexBuffer),
		  m_Material(std::move(data.material))
	{
		Setup(std::move(data.children));
	}

	// MeshData Mesh::Import(std::string_view filepath)
	//{
	//	return { Codex::Resolve<Model>(filepath)->GetMesh() };
	//}

	void Mesh::Update(Delta ts)
	{
		(*m_Material->GetBufferUniform("PerObjectData"))["Model"] = m_Transform.AsMatrix();

		for (Mesh& child : m_Children)
			child.Update(ts, m_Transform);
	}

	void Mesh::Update(Delta ts, const Transform& parentTransform)
	{
		// Calculate it raw here to avoid the cache check in Transform::AsMatrix
		(*m_Material->GetBufferUniform("PerObjectData"))["Model"] =
			MatrixTranslation(m_Transform.Translation() + parentTransform.Translation()) *
			MatrixRotation(m_Transform.Rotation() + parentTransform.Rotation()) *
			MatrixScale(m_Transform.Scale() * parentTransform.Scale());

		for (Mesh& child : m_Children)
			child.Update(ts, m_Transform);
	}

	void Mesh::Render(const CommandBuffer& cmdBuff) const
	{
		m_Material->CmdBind(cmdBuff);

		m_VertexBuffer->CmdBind(cmdBuff);
		m_IndexBuffer->CmdBind(cmdBuff);

		vkCmdDrawIndexed(cmdBuff, m_IndexBuffer->GetNumberOfIndices(), 1, 0, 0, 0);

		for (const Mesh& child : m_Children)
			child.Render(cmdBuff);
	}

	Mesh::~Mesh() {}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		m_VertexBuffer = std::move(other.m_VertexBuffer);
		m_IndexBuffer = std::move(other.m_IndexBuffer);

		m_Material = std::move(other.m_Material);
		m_Entity = std::move(other.m_Entity);

		m_Transform = std::move(other.m_Transform);
		m_Children = std::move(other.m_Children);
		return *this;
	}

	Mesh::Mesh(Mesh&& other) noexcept : Component(other.m_Entity) { *this = std::move(other); }

	void Mesh::Setup(std::vector<MeshData> children)
	{
		for (MeshData& child : children)
		{
			m_Children.emplace_back(GetEntity(), child);
		}
	}
}  // namespace At0::Ray
