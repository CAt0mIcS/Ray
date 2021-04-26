#pragma once

#include "../../Core/RMath.h"
#include "../../Core/RTime.h"
#include "../Materials/RMaterial.h"

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


	class Model
	{
	public:
		enum Flags
		{
			Unspecified = 0b00000000,
			NoDiffuseMap = 0b0000001,
			NoSpecularMap = 0b0000010,
			NoNormalMap = 0b00000100
		};

	public:
		Model(std::string_view filepath, int flags = 0);
		~Model();

		Model& operator=(Model&& other) noexcept = default;
		Model(Model&& other) noexcept = default;

		MeshData& GetMesh() { return *m_RootMesh; }

		static std::string GetUID(std::string_view filepath, int flags = 0);

	private:
		void ParseMesh(std::string_view base, const aiMesh& mesh,
			const aiMaterial* const* pMaterials, int flags);
		static Ref<Material> CreateMaterial(const std::string& basePath, const aiMesh& mesh,
			const aiMaterial* const* pMaterials, int flags);

	private:
		Scope<MeshData> m_RootMesh;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Model);
