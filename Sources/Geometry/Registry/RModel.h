#pragma once

#include "../../Core/RMath.h"
#include "../../Core/RTime.h"
#include "../RMesh.h"

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
	class ModelMaterial;

	class Model
	{
	public:
		Model(std::string_view filepath);

		Model& operator=(Model&& other) noexcept = default;
		Model(Model&& other) noexcept = default;

		Mesh::MeshData& GetMesh() { return *m_RootMesh; }

		static std::string GetUID(std::string_view filepath) { return filepath.data(); }

	private:
		void ParseMesh(
			std::string_view base, const aiMesh& mesh, const aiMaterial* const* pMaterials);
		static Ref<ModelMaterial> CreateMaterial(
			const std::string& basePath, const aiMesh& mesh, const aiMaterial* const* pMaterials);

	private:
		Scope<Mesh::MeshData> m_RootMesh;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Model);
