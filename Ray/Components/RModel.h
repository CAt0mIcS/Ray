#pragma once

#include "RComponent.h"

#include "../Core/RDynamicVertex.h"
#include "../Graphics/Buffers/RIndexBuffer.h"


struct aiMesh;
struct aiMaterial;
struct aiNode;
struct aiScene;

#include "RMesh.h"
#include "RMeshRenderer.h"


namespace At0::Ray
{
	class Material;
	class Shader;

	class RAY_EXPORT Model : public Component
	{
	public:
		Model(Entity entity, std::string_view filepath, Ref<Material> material = nullptr);

	private:
		bool ProcessNode(Entity parent, std::string_view filepath, aiNode* pNode,
			const aiScene* pScene, Ref<Material> material);

		void ParseMesh(Entity entity, std::string_view filepath, const aiMesh& mesh,
			const aiScene* pScene, Ref<Material> material);
		static Ref<Material> CreateMaterial(
			const std::string& basePath, const aiMaterial* pMaterial);
		static DynamicVertex AssembleVertices(const aiMesh& mesh, const Shader& shader);
		static std::vector<IndexBuffer::Type> GenerateIndices(const aiMesh& mesh);
	};

	class MeshContainer : public Component
	{
	public:
		MeshContainer(Entity entity) : Component(entity) {}

		void AddMesh(Mesh mesh, Ref<Material> material)
		{
			Entity e = mesh.GetEntity();

			m_Meshes.emplace_back(
				MeshContainer::Data{ std::move(mesh), MeshRenderer{ e, std::move(material) } });
			RAY_DEBUG_FLAG(m_Name += "|" + std::string(m_Meshes.back().mesh.GetName()));
		}

		RAY_DEBUG_FLAG(std::string_view GetName() const { return m_Name; })

		void Render(const CommandBuffer& cmdBuff) const;
		void Update();

	private:
		struct Data
		{
			Mesh mesh;
			MeshRenderer meshRenderer;
		};

		std::vector<Data> m_Meshes;
		RAY_DEBUG_FLAG(std::string m_Name);
	};

}  // namespace At0::Ray

RAY_EXPORT_COMPONENT(Model);
RAY_EXPORT_COMPONENT(MeshContainer);
