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

#include "Registry/RModel.h"
#include "Registry/RGeometricPrimitives.h"
#include "Scene/REntity.h"
#include "Utils/RException.h"
#include "Graphics/RVertex.h"


namespace At0::Ray
{
	const Mesh::Shaders Mesh::s_DefaultShaders = { "Resources/Shaders/DefaultShader.vert",
		"Resources/Shaders/DefaultShader.frag" };

	Mesh::Mesh(Entity& entity, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer,
		Material material, std::vector<MeshData> children)
		: Component(entity), m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer),
		  m_Material(material),
		  m_PerObjectUniform("PerObjectData", Shader::Stage::Vertex, material.GetGraphicsPipeline())
	{
		Setup(std::move(children));
	}

	Mesh::Mesh(Entity& entity, MeshData data)
		: Component(entity), m_VertexBuffer(data.vertexBuffer), m_IndexBuffer(data.indexBuffer),
		  m_Material(data.material), m_PerObjectUniform("PerObjectData", Shader::Stage::Vertex,
										 data.material.GetGraphicsPipeline())
	{
		Setup(std::move(data.children));
	}

	Mesh::MeshData Mesh::Triangle(const Shaders& shaders)
	{
		Material::Config matConfig{};
		matConfig.shaders = shaders;
		matConfig.cullMode = VK_CULL_MODE_NONE;

		Material material(matConfig);

		IndexedTriangleList triangle = IndexedTriangleList::Triangle(material.GetVertexLayout());

		Ref<VertexBuffer> vertexBuffer =
			Codex::Resolve<VertexBuffer>(triangle.vertexTag, triangle.vertices);
		Ref<IndexBuffer> indexBuffer =
			Codex::Resolve<IndexBuffer>(triangle.indexTag, triangle.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	Mesh::MeshData Mesh::Plane(const Shaders& shaders)
	{
		Material::Config matConfig{};
		matConfig.shaders = shaders;
		matConfig.cullMode = VK_CULL_MODE_NONE;

		Material material(matConfig);

		IndexedTriangleList plane = IndexedTriangleList::Plane(material.GetVertexLayout());

		Ref<VertexBuffer> vertexBuffer =
			Codex::Resolve<VertexBuffer>(plane.vertexTag, plane.vertices);
		Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(plane.indexTag, plane.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	Mesh::MeshData Mesh::HalfCircle(int segments, float radius, const Shaders& shaders)
	{
		Material::Config matConfig{};
		matConfig.shaders = shaders;
		matConfig.cullMode = VK_CULL_MODE_NONE;

		Material material(matConfig);

		IndexedTriangleList circle =
			IndexedTriangleList::HalfCircle(material.GetVertexLayout(), segments, radius);

		Ref<VertexBuffer> vertexBuffer =
			Codex::Resolve<VertexBuffer>(circle.vertexTag, circle.vertices);
		Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(circle.indexTag, circle.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	Mesh::MeshData Mesh::Circle(int segments, float radius, const Shaders& shaders)
	{
		Material::Config matConfig{};
		matConfig.shaders = shaders;
		matConfig.cullMode = VK_CULL_MODE_NONE;

		Material material(matConfig);

		IndexedTriangleList circle =
			IndexedTriangleList::Circle(material.GetVertexLayout(), segments, radius);

		Ref<VertexBuffer> vertexBuffer =
			Codex::Resolve<VertexBuffer>(circle.vertexTag, circle.vertices);
		Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(circle.indexTag, circle.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	Mesh::MeshData Mesh::Cube(const Shaders& shaders)
	{
		Material material(shaders);

		IndexedTriangleList cube = IndexedTriangleList::Cube(material.GetVertexLayout());

		Ref<VertexBuffer> vertexBuffer =
			Codex::Resolve<VertexBuffer>(cube.vertexTag, cube.vertices);
		Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(cube.indexTag, cube.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	Mesh::MeshData Mesh::UVSphere(float radius, int latDiv, int longDiv, const Shaders& shaders)
	{
		Material material(shaders);

		IndexedTriangleList uvSphere =
			IndexedTriangleList::UVSphere(material.GetVertexLayout(), radius, latDiv, longDiv);

		Ref<VertexBuffer> vertexBuffer =
			Codex::Resolve<VertexBuffer>(uvSphere.vertexTag, uvSphere.vertices);
		Ref<IndexBuffer> indexBuffer =
			Codex::Resolve<IndexBuffer>(uvSphere.indexTag, uvSphere.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	Mesh::MeshData Mesh::Vector(const Float3& headPos, const Shaders& shaders)
	{
		Material::Config matConfig{};
		matConfig.shaders = shaders;
		matConfig.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

		Material lineMaterial(matConfig);

		IndexedTriangleList vec =
			IndexedTriangleList::Vector(lineMaterial.GetVertexLayout(), headPos);

		Ref<VertexBuffer> vertexBuffer = Codex::Resolve<VertexBuffer>(vec.vertexTag, vec.vertices);
		Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(vec.indexTag, vec.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(lineMaterial) };
	}

	Mesh::MeshData Mesh::Import(std::string_view filepath)
	{
		return { Codex::Resolve<Model>(filepath)->GetMesh() };
	}

	void Mesh::Update(Delta ts)
	{
		m_PerObjectUniform["model"] = m_Transform.AsMatrix();
		for (Mesh& child : m_Children)
			child.Update(ts, m_Transform);
	}

	void Mesh::Update(Delta ts, const Transform& parentTransform)
	{
		// Calculate it raw here to avoid the cache check in Transform::AsMatrix
		m_PerObjectUniform["model"] =
			MatrixTranslation(m_Transform.Translation() + parentTransform.Translation()) *
			MatrixRotation(m_Transform.Rotation() + parentTransform.Rotation()) *
			MatrixScale(m_Transform.Scale() * parentTransform.Scale());

		for (Mesh& child : m_Children)
			child.Update(ts, m_Transform);
	}

	void Mesh::Render(const CommandBuffer& cmdBuff) const
	{
		m_Material.GetGraphicsPipeline().CmdBind(cmdBuff);

		for (const auto& [tag, uniform] : m_Uniforms)
			uniform->CmdBind(cmdBuff);

		m_PerObjectUniform.CmdBind(cmdBuff);
		m_VertexBuffer->CmdBind(cmdBuff);
		m_IndexBuffer->CmdBind(cmdBuff);

		vkCmdDrawIndexed(cmdBuff, m_IndexBuffer->GetNumberOfIndices(), 1, 0, 0, 0);

		for (const Mesh& child : m_Children)
			child.Render(cmdBuff);
	}

	void Mesh::AddUniform(std::string_view tag, Scope<Uniform> uniform)
	{
		m_Uniforms.emplace_back(tag, std::move(uniform));
	}

	bool Mesh::HasUniform(std::string_view tag) const
	{
		for (auto& [uTag, uniform] : m_Uniforms)
			if (tag == uTag)
				return true;
		return false;
	}

	Uniform& Mesh::GetUniform(std::string_view tag)
	{
		for (auto& [uTag, uniform] : m_Uniforms)
			if (tag == uTag)
				return *uniform;

		RAY_THROW_RUNTIME("[Mesh] Failed to get uniform with tag {0}", tag);
	}

	Mesh::~Mesh() {}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		m_VertexBuffer = std::move(other.m_VertexBuffer);
		m_IndexBuffer = std::move(other.m_IndexBuffer);

		m_Material = std::move(other.m_Material);
		m_PerObjectUniform = std::move(other.m_PerObjectUniform);

		m_Transform = std::move(other.m_Transform);
		m_Uniforms = std::move(other.m_Uniforms);
		m_Children = std::move(other.m_Children);
		return *this;
	}

	Mesh::Mesh(Mesh&& other) noexcept
		: Component(*other.m_Entity), m_VertexBuffer(std::move(other.m_VertexBuffer)),
		  m_IndexBuffer(std::move(other.m_IndexBuffer)), m_Material(std::move(other.m_Material)),
		  m_PerObjectUniform(std::move(other.m_PerObjectUniform)),
		  m_Transform(std::move(other.m_Transform)), m_Uniforms(std::move(other.m_Uniforms)),
		  m_Children(std::move(other.m_Children))
	{
	}

	void Mesh::Setup(std::vector<MeshData> children)
	{
		for (MeshData& child : children)
		{
			m_Children.emplace_back(GetEntity(), child);
		}

		if (const Texture2D* diffuseMap = m_Material.GetDiffuseMap())
		{
			AddUniform("materialDiffuse",
				MakeScope<SamplerUniform>("materialDiffuse", Shader::Stage::Fragment, *diffuseMap,
					m_Material.GetGraphicsPipeline()));
		}
		if (const Texture2D* specularMap = m_Material.GetSpecularMap())
		{
			AddUniform("materialSpecular",
				MakeScope<SamplerUniform>("materialSpecular", Shader::Stage::Fragment, *specularMap,
					m_Material.GetGraphicsPipeline()));
		}
	}

}  // namespace At0::Ray
