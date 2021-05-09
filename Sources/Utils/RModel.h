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


	class Model
	{
	public:
		enum Flags
		{
			Unspecified = 0 << 0,
			NoDiffuseMap = 1 << 0,
			NoSpecularMap = 1 << 1,
			NoNormalMap = 1 << 2,

			NoTextureCoordinates = 1 << 3,
			NoNormals = 1 << 4
		};

	public:
		Model(std::string_view filepath, Model::Flags flags = Model::Flags::Unspecified);
		~Model();

		Model& operator=(Model&& other) noexcept = default;
		Model(Model&& other) noexcept = default;

		MeshData& GetMesh() { return *m_RootMesh; }

		static std::string GetUID(
			std::string_view filepath, Model::Flags flags = Model::Flags::Unspecified);

	private:
		void ParseMesh(std::string_view base, const aiMesh& mesh,
			const aiMaterial* const* pMaterials, Model::Flags flags);
		static Material CreateMaterial(const std::string& basePath, const aiMesh& mesh,
			const aiMaterial* const* pMaterials, Model::Flags flags);

	private:
		Scope<MeshData> m_RootMesh;
	};


	inline Model::Flags operator|(Model::Flags r, Model::Flags l)
	{
		return (Model::Flags)((int)r | (int)l);
	}
}  // namespace At0::Ray
