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
		template<typename... MaterialArgs>
		Model(std::string_view filepath, Model::Flags flags = Model::Flags::Unspecified,
			std::optional<Material> material = std::nullopt, MaterialArgs&&... args)
		{
			Material::Config config{};
			(Material::FillConfig(config, args), ...);

			Setup(filepath, config, flags, material);
		}
		~Model();

		Model& operator=(Model&& other) noexcept = default;
		Model(Model&& other) noexcept = default;

		MeshData& GetMesh() { return *m_RootMesh; }

		template<typename... MaterialArgs>
		static std::string GetUID(std::string_view filepath,
			Model::Flags flags = Model::Flags::Unspecified,
			std::optional<Material> material = std::nullopt, MaterialArgs&&... args)
		{
			// RAY_TODO: Take MaterialArgs and custom material into account
			std::ostringstream oss;
			oss << filepath << "#" << (uint32_t)flags;
			return oss.str();
		}

	private:
		void Setup(std::string_view filepath, Material::Config& config,
			Model::Flags flags = Model::Flags::Unspecified,
			std::optional<Material> material = std::nullopt);
		void ParseMesh(std::string_view base, const aiMesh& mesh,
			const aiMaterial* const* pMaterials, Model::Flags flags,
			std::optional<Material> material, Material::Config& config);
		static Material CreateMaterial(const std::string& basePath, const aiMesh& mesh,
			const aiMaterial* const* pMaterials, Model::Flags flags, Material::Config& config);

		static bool HasNormalMap(const aiMesh& mesh, const aiMaterial* const* pMaterials,
			std::optional<Material> material);

	private:
		MeshData* m_RootMesh = nullptr;
	};


	inline Model::Flags operator|(Model::Flags r, Model::Flags l)
	{
		return (Model::Flags)((int)r | (int)l);
	}
}  // namespace At0::Ray
