#pragma once

#include "RComponent.h"

#include "../Core/RDynamicVertex.h"
#include "../Graphics/Buffers/RIndexBuffer.h"


struct aiMesh;
struct aiMaterial;
struct aiNode;
struct aiScene;


namespace At0::Ray
{
	class Material;
	class Shader;

	class RAY_EXPORT Model : public Component
	{
	public:
		Model(Entity entity, std::string_view filepath, Ref<Material> material = nullptr);

		void CmdBind(const CommandBuffer& cmdBuff);

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
}  // namespace At0::Ray

RAY_EXPORT_COMPONENT(Model);
