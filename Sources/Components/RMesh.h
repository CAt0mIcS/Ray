#pragma once

#include "../Components/RComponent.h"
#include "../Shading/RMaterial.h"

#include "../Core/RMath.h"
#include "../Core/RTime.h"
#include "../Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "../Graphics/Pipelines/Uniforms/RSamplerUniform.h"
#include "../Utils/RModel.h"
#include "../Graphics/RVertex.h"

#include <type_traits>


namespace At0::Ray
{
	class VertexBuffer;
	class IndexBuffer;
	class VertexInput;
	class CommandBuffer;
	class Texture2D;


	struct MeshData
	{
		Ref<VertexBuffer> vertexBuffer;
		Ref<IndexBuffer> indexBuffer;
		Material material;
		std::vector<MeshData> children;
	};

	class RAY_EXPORT Mesh : public Component
	{
		using Shaders = std::vector<std::string>;
		static const Shaders s_DefaultShaders;

	public:
		Mesh(Entity entity, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer,
			Material material, std::vector<MeshData> children = {});
		Mesh(Entity entity, MeshData data);

		static MeshData Triangle(Material material);
		static MeshData Plane(Material material, Vertex::Flags vertexFlags = Vertex::Position3D);
		// static MeshData HalfCircle(
		//	int segments = 360, float radius = 1.0f, const Shaders& shaders = s_DefaultShaders);
		// static MeshData Circle(
		//	int segments = 360, float radius = 1.0f, const Shaders& shaders = s_DefaultShaders);

		static MeshData Cube(Material material, Vertex::Flags vertexFlags = Vertex::Position3D);
		// static MeshData IcoSphere(
		//	Material material, Vertex::Flags vertexFlags = Vertex::Position3D);
		static MeshData UVSphere(Material material, float radius = 1.0f, int latDiv = 12,
			int longDiv = 24, Vertex::Flags vertexFlags = Vertex::Position3D);
		// static MeshData Cylinder(
		//	int segments = 360, float radius = 1.0f, const Shaders& shaders = s_DefaultShaders);
		// static MeshData Vector(const Float3& headPos, float lineWidth = 1.0f,
		//	const Shaders& shaders = s_DefaultShaders);
		static MeshData Import(
			std::string_view filepath, Model::Flags flags = Model::Flags::Unspecified);

		/**
		 * @returns The root transform of this mesh
		 */
		Transform& GetTransform() { return (Transform&)std::as_const(*this).GetTransform(); }

		/**
		 * @returns The root transform of this mesh
		 */
		const Transform& GetTransform() const { return m_Transform; }

		/**
		 * Updates all resources.
		 */
		void Update(Delta ts);

		/**
		 * Updates all resources.
		 * This function is called for children of a parent mesh
		 */
		void Update(Delta ts, const Transform& parentTransform);

		/**
		 * Binds all bindables and draws the object
		 */
		void Render(const CommandBuffer& cmdBuff) const;

		/**
		 * @returns The material this mesh is using
		 */
		const Material& GetMaterial() const { return m_Material; }

		/**
		 * @returns The material this mesh is using
		 */
		Material& GetMaterial() { return m_Material; }

		/**
		 * Sets a new material for the mesh
		 */
		void SetMaterial(Material material) { m_Material = std::move(material); }

		~Mesh();
		Mesh& operator=(Mesh&& other) noexcept;
		Mesh(Mesh&& other) noexcept;

	private:
		void Setup(std::vector<MeshData> children = {});

	private:
		Ref<VertexBuffer> m_VertexBuffer = nullptr;
		Ref<IndexBuffer> m_IndexBuffer = nullptr;
		Material m_Material;

		std::vector<Mesh> m_Children;

		Transform m_Transform;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Mesh);
