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

	class RAY_EXPORT Model
	{
	public:
		struct Data
		{
			Ref<VertexBuffer> vertexBuffer;
			Ref<IndexBuffer> indexBuffer;
			Material::Layout layout;
			std::vector<Model::Data> children;
		};

	public:
		Model(std::string_view filepath, Material::Layout layout);
		~Model();

		Model& operator=(Model&& other) noexcept = default;
		Model(Model&& other) noexcept = default;

		const Model::Data& GetData() const { return m_Data; }

		static std::string GetUID(std::string_view filepath, Material::Layout layout);

	private:
		void ParseMesh(
			std::string_view base, const aiMesh& mesh, const aiMaterial* const* pMaterials);
		static Material::Layout CreateMaterial(const std::string& basePath, const aiMesh& mesh,
			const aiMaterial* const* pMaterials, Material::Layout layout);

	private:
		Model::Data m_Data;
	};
}  // namespace At0::Ray
