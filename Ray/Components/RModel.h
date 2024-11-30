// #pragma once
//
// #include "RComponent.h"
//
// #include "../Core/RDynamicVertex.h"
// #include "../Graphics/Buffers/RIndexBuffer.h"
//
//
// struct aiMesh;
// struct aiMaterial;
// struct aiNode;
// struct aiScene;
//
//
// namespace At0::Ray
//{
//	class Material;
//	class Shader;
//	class Scene;
//
//	class RAY_EXPORT Model : public Component
//	{
//	public:
//		// RAY_TODO: Remove dependency on scene and overhaul model loading (Should happen in e.g.
//		// ResourceManager)
//		Model(Entity entity, Scene& scene, std::string_view filepath,
//			Ref<Material> material = nullptr);
//
//	private:
//		bool ProcessNode(Entity parent, Scene& scene, std::string_view filepath, aiNode* pNode,
//			const aiScene* pScene, Ref<Material> material);
//
//		void ParseMesh(Entity entity, std::string_view filepath, const aiMesh& mesh,
//			const aiScene* pScene, Ref<Material> material);
//		static Ref<Material> CreateMaterial(
//			const std::string& basePath, const aiMaterial* pMaterial);
//		static DynamicVertex AssembleVertices(const aiMesh& mesh, const Shader& shader);
//		static std::vector<IndexBuffer::Type> GenerateIndices(const aiMesh& mesh);
//	};
//
// }  // namespace At0::Ray
//
// RAY_EXPORT_COMPONENT(Model);
