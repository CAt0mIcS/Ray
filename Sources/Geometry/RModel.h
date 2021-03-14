#pragma once

#include "../Core/RComponent.h"
#include "../Core/RMath.h"
#include "../Core/RTime.h"

#include <type_traits>


struct aiMesh;
struct aiMaterial;
struct aiNode;


namespace At0::Ray
{
	class Mesh;
	class CommandBuffer;
	class VertexBuffer;
	class IndexBuffer;

	class RAY_EXPORT Model : public Component
	{
	public:
		Model(Entity& entity, std::string_view filepath);
		~Model();

		void Update(Delta ts);
		void Render(const CommandBuffer& cmdBuff) const;

		template<typename T>
		T& Get()
		{
			return Get(std::type_identity<T>());
		}

		Model& operator=(Model&& other) noexcept;
		Model(Model&& other) noexcept;

	private:
		static Mesh ParseMesh(Entity& entity, std::string_view base, const aiMesh& mesh,
			const aiMaterial* const* pMaterials);
		static Mesh CreateMesh(Entity& entity, const std::string& basePath, const aiMesh& mesh,
			const aiMaterial* const* pMaterials, Ref<VertexBuffer> vertexBuffer,
			Ref<IndexBuffer> indexBuffer);

		Transform& Get(std::type_identity<Transform>) { return m_Transform; }

	private:
		std::vector<Mesh> m_Meshes;
		Transform m_Transform{};
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Model);
