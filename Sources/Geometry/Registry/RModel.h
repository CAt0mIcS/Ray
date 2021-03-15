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

	class Model
	{
	public:
		Model(std::string_view filepath);

		Model& operator=(Model&& other) noexcept = default;
		Model(Model&& other) noexcept = default;

	private:
		void ParseMesh(
			std::string_view base, const aiMesh& mesh, const aiMaterial* const* pMaterials);
		static Material CreateMaterial(
			const std::string& basePath, const aiMesh& mesh, const aiMaterial* const* pMaterials);
		static std::vector<std::string> GetShaders(
			bool hasDiffuseMap, bool hasSpecularMap, bool hasNormalMap);

	public:
		Scope<Mesh::MeshData> rootMesh;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Model);
