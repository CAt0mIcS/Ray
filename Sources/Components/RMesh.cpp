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

#include "Utils/RGeometricPrimitives.h"
#include "Scene/REntity.h"
#include "Utils/RException.h"
#include "Graphics/RVertex.h"


namespace At0::Ray
{
	const Mesh::Shaders Mesh::s_DefaultShaders = { "Resources/Shaders/DefaultShader.vert",
		"Resources/Shaders/DefaultShader.frag" };

	Mesh::Mesh(Entity entity, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer,
		Material material, std::vector<MeshData> children)
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

	MeshData Mesh::Triangle(Material material)
	{
		IndexedTriangleList triangle = IndexedTriangleList::Triangle(material.GetVertexLayout());

		Ref<VertexBuffer> vertexBuffer =
			Codex::Resolve<VertexBuffer>(triangle.tag, triangle.vertices);
		Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(triangle.tag, triangle.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	MeshData Mesh::Plane(Material material, Vertex::Flags vertexFlags)
	{
		IndexedTriangleList plane =
			IndexedTriangleList::Plane(material.GetVertexLayout(), vertexFlags);

		Ref<VertexBuffer> vertexBuffer = Codex::Resolve<VertexBuffer>(plane.tag, plane.vertices);
		Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(plane.tag, plane.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	// Mesh::MeshData Mesh::HalfCircle(int segments, float radius, const Shaders& shaders)
	//{
	//	Material::Config matConfig{};
	//	matConfig.shaders = shaders;
	//	matConfig.cullMode = VK_CULL_MODE_NONE;

	//	Material material(matConfig);

	//	IndexedTriangleList circle =
	//		IndexedTriangleList::HalfCircle(material.GetVertexLayout(), segments, radius);

	//	Ref<VertexBuffer> vertexBuffer = Codex::Resolve<VertexBuffer>(circle.tag, circle.vertices);
	//	Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(circle.tag, circle.indices);

	//	return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	//}

	// Mesh::MeshData Mesh::Circle(int segments, float radius, const Shaders& shaders)
	//{
	//	Material::Config matConfig{};
	//	matConfig.shaders = shaders;
	//	matConfig.cullMode = VK_CULL_MODE_NONE;

	//	Material material(matConfig);

	//	IndexedTriangleList circle =
	//		IndexedTriangleList::Circle(material.GetVertexLayout(), segments, radius);

	//	Ref<VertexBuffer> vertexBuffer = Codex::Resolve<VertexBuffer>(circle.tag, circle.vertices);
	//	Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(circle.tag, circle.indices);

	//	return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	//}

	MeshData Mesh::Cube(Material material, Vertex::Flags vertexFlags)
	{
		IndexedTriangleList cube =
			IndexedTriangleList::Cube(material.GetVertexLayout(), vertexFlags);

		Ref<VertexBuffer> vertexBuffer = Codex::Resolve<VertexBuffer>(cube.tag, cube.vertices);
		Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(cube.tag, cube.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	MeshData Mesh::UVSphere(
		Material material, float radius, int latDiv, int longDiv, Vertex::Flags vertexFlags)
	{
		IndexedTriangleList uvSphere = IndexedTriangleList::UVSphere(
			material.GetVertexLayout(), radius, latDiv, longDiv, vertexFlags);

		Ref<VertexBuffer> vertexBuffer =
			Codex::Resolve<VertexBuffer>(uvSphere.tag, uvSphere.vertices);
		Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(uvSphere.tag, uvSphere.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	// Mesh::MeshData Mesh::Cylinder(int segments, float radius, const Shaders& shaders)
	//{
	//	Material material(shaders);

	//	IndexedTriangleList cylinder =
	//		IndexedTriangleList::Cylinder(material.GetVertexLayout(), segments, radius);

	//	Ref<VertexBuffer> vertexBuffer =
	//		Codex::Resolve<VertexBuffer>(cylinder.tag, cylinder.vertices);
	//	Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(cylinder.tag, cylinder.indices);

	//	return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	//}

	// Mesh::MeshData Mesh::Vector(const Float3& headPos, float lineWidth, const Shaders& shaders)
	//{
	//	Material::Config matConfig{};
	//	matConfig.shaders = shaders;
	//	matConfig.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	//	matConfig.lineWidth = lineWidth;

	//	Material lineMaterial(matConfig);

	//	IndexedTriangleList vec =
	//		IndexedTriangleList::Vector(lineMaterial.GetVertexLayout(), headPos);

	//	Ref<VertexBuffer> vertexBuffer = Codex::Resolve<VertexBuffer>(vec.tag, vec.vertices);
	//	Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(vec.tag, vec.indices);

	//	return { std::move(vertexBuffer), std::move(indexBuffer), std::move(lineMaterial) };
	//}

	MeshData Mesh::Import(
		std::string_view filepath, Model::Flags flags, std::optional<Material> material)
	{
		return { Codex::Resolve<Model>(filepath, flags, material)->GetMesh() };
	}

	void Mesh::Update(Delta ts)
	{
		m_Material.GetUniform<BufferUniform>("PerObjectData")["model"] = m_Transform.AsMatrix();

		for (Mesh& child : m_Children)
			child.Update(ts, m_Transform);
	}

	void Mesh::Update(Delta ts, const Transform& parentTransform)
	{
		// Calculate it raw here to avoid the cache check in Transform::AsMatrix
		m_Material.GetUniform<BufferUniform>("PerObjectData")["model"] =
			MatrixTranslation(m_Transform.Translation() + parentTransform.Translation()) *
			MatrixRotation(m_Transform.Rotation() + parentTransform.Rotation()) *
			MatrixScale(m_Transform.Scale() * parentTransform.Scale());

		for (Mesh& child : m_Children)
			child.Update(ts, m_Transform);
	}

	void Mesh::Render(const CommandBuffer& cmdBuff) const
	{
		m_Material.CmdBind(cmdBuff);

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
