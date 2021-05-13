#pragma once

#include "../Core/RMath.h"
#include "../Core/RTime.h"
#include "../Shading/RMaterial.h"

#include <type_traits>


struct aiMesh;
struct aiMaterial;
struct aiNode;


namespace At0::Ray
{
	class MeshData;
	class CommandBuffer;
	class VertexBuffer;
	class IndexBuffer;

	// RAY_TODO: Move somewhere else
	struct MeshData
	{
		Ref<VertexBuffer> vertexBuffer;
		Ref<IndexBuffer> indexBuffer;
		Ref<Material> material;
		std::vector<MeshData> children;
	};


	class RAY_EXPORT Model
	{
	public:
		Model(std::string_view filepath, Material::Layout layout);
		~Model();

		Model& operator=(Model&& other) noexcept = default;
		Model(Model&& other) noexcept = default;

		MeshData& GetMesh() { return *m_RootMesh; }

		static std::string GetUID(std::string_view filepath, Material::Layout layout);

	private:
		void ParseMesh(std::string_view base, const aiMesh& mesh,
			const aiMaterial* const* pMaterials, Material::Layout layout);
		static Ref<Material> CreateMaterial(const std::string& basePath, const aiMesh& mesh,
			const aiMaterial* const* pMaterials, Material::Layout layout);

	private:
		std::optional<MeshData> m_RootMesh;
	};
}  // namespace At0::Ray
