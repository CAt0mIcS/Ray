#pragma once

#include "../Core/RComponent.h"
#include "Registry/RMaterial.h"

#include "../Core/RMath.h"
#include "../Core/RTime.h"
#include "../Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "../Graphics/Pipelines/Uniforms/RSamplerUniform.h"


#include <type_traits>


namespace At0::Ray
{
	class VertexBuffer;
	class IndexBuffer;
	class VertexInput;
	class CommandBuffer;
	class Texture2D;


	class RAY_EXPORT Mesh : public Component
	{
		friend class Model;

	private:
		struct MeshData
		{
			Ref<VertexBuffer> vertexBuffer;
			Ref<IndexBuffer> indexBuffer;
			Material material;
			std::vector<MeshData> children;
		};

	private:
		using Shaders = std::vector<std::string>;
		static const Shaders s_DefaultShaders;

	public:
		Mesh(Entity& entity, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer,
			Material material, std::vector<MeshData> children = {});
		Mesh(Entity& entity, MeshData data);

		static MeshData Triangle(const Shaders& shaders = s_DefaultShaders);
		static MeshData Plane(const Shaders& shaders = s_DefaultShaders);
		static MeshData HalfCircle(
			int segments = 360, float radius = 1.0f, const Shaders& shaders = s_DefaultShaders);
		static MeshData Circle(
			int segments = 360, float radius = 1.0f, const Shaders& shaders = s_DefaultShaders);

		static MeshData Cube(const Shaders& shaders = s_DefaultShaders);
		static MeshData IcoSphere(const Shaders& shaders = s_DefaultShaders);
		static MeshData UVSphere(float radius = 1.0f, int latDiv = 12, int longDiv = 24,
			const Shaders& shaders = s_DefaultShaders);
		static MeshData Cylinder(
			float radius = 1.0f, int segments = 360, const Shaders& shaders = s_DefaultShaders);
		static MeshData Vector(const Float3& headPos, float lineWidth = 1.0f,
			const Shaders& shaders = s_DefaultShaders);
		static MeshData Import(std::string_view filepath);

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
		 * Adds a uniform to the list of uniforms which are automatically bound in CmdBind
		 * @param tag Unique tag to identify the uniform
		 * @param uniform Uniform to add
		 */
		void AddUniform(std::string_view tag, Scope<Uniform> uniform);

		/**
		 * @returns If the uniform with tag has been added
		 */
		bool HasUniform(std::string_view tag) const;

		/**
		 * @param tag The tag used when adding the uniform with "AddUniform"
		 * @returns The uniform which was added using "AddUniform"
		 */
		Uniform& GetUniform(std::string_view tag);

		/**
		 * @returns The material this mesh is using
		 */
		const Material& GetMaterial() const { return m_Material; }

		~Mesh();
		Mesh& operator=(Mesh&& other) noexcept;
		Mesh(Mesh&& other) noexcept;

	private:
		void Setup(std::vector<MeshData> children = {});

	private:
		Ref<VertexBuffer> m_VertexBuffer = nullptr;
		Ref<IndexBuffer> m_IndexBuffer = nullptr;

		BufferUniform m_PerObjectUniform;
		Material m_Material;

		std::vector<std::pair<std::string, Scope<Uniform>>> m_Uniforms;
		std::vector<Mesh> m_Children;

		Transform m_Transform;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Mesh);
