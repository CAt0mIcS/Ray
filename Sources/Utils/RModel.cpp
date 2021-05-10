#include "Rpch.h"
#include "RModel.h"

#include "Components/RMesh.h"
#include "Graphics/RVertex.h"
#include "Graphics/RCodex.h"
#include "Utils/RLogger.h"
#include "Utils/RException.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Images/RTexture2D.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace At0::Ray
{
	Model::Model(std::string_view filepath, Model::Flags flags, std::optional<Material> material)
	{
		Log::Info("[Model] Importing model \"{0}\"", filepath);

		const aiScene* pScene = nullptr;
		Assimp::Importer imp;
		if (flags & Flags::NoNormals)
		{
			pScene = imp.ReadFile(filepath.data(),
				aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
					aiProcess_ConvertToLeftHanded /* | aiProcess_CalcTangentSpace*/);
		}
		else
		{
			pScene = imp.ReadFile(
				filepath.data(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded |
									 aiProcess_GenNormals /* | aiProcess_CalcTangentSpace*/);
		}

		if (!pScene)
			RAY_THROW_RUNTIME("[Model] Failed to load: \"{0}\"", imp.GetErrorString());

		for (uint32_t i = 0; i < pScene->mNumMeshes; ++i)
		{
			ParseMesh(filepath, *pScene->mMeshes[i], pScene->mMaterials, flags, material);
		}
	}

	Model::~Model() {}

	std::string Model::GetUID(
		std::string_view filepath, Model::Flags flags, std::optional<Material> material)
	{
		std::ostringstream oss;
		oss << filepath << "#" << (uint32_t)flags;
		return oss.str();
	}

	void Model::ParseMesh(std::string_view base, const aiMesh& mesh,
		const aiMaterial* const* pMaterials, Model::Flags flags, std::optional<Material> material)
	{
		Log::Info("[Model] Parsing mesh \"{0}\"", mesh.mName.C_Str());

		using namespace std::string_literals;
		const std::string meshTag = std::string(base) + "%"s + mesh.mName.C_Str();

		VertexLayout layout{};
		layout.Append(VK_FORMAT_R32G32B32_SFLOAT);	// Position

		aiString normalTexFileName;
		if ((flags & Model::NoTextureCoordinates) == 0)
			layout.Append(VK_FORMAT_R32G32_SFLOAT);	 // Texture coordinate
		if (HasNormalMap(mesh, pMaterials, material) && (flags & Model::NoNormalMap) == 0)
			flags = flags | Model::NoNormals;
		else if ((flags & Model::NoNormals) == 0)
			layout.Append(VK_FORMAT_R32G32B32_SFLOAT);	// Normal

		VertexInput vertexInput(layout);

		for (uint32_t i = 0; i < mesh.mNumVertices; ++i)
		{
			if (flags & Model::NoTextureCoordinates && flags & Model::NoNormals)
				vertexInput.Emplace(
					Float3(mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z));

			else if (flags & Model::NoTextureCoordinates)
				vertexInput.Emplace(
					Float3(mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z),
					Float3(mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z));

			else if (flags & Model::NoNormals)
				vertexInput.Emplace(
					Float3(mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z),
					Float2(mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y));

			else
				vertexInput.Emplace(
					Float3(mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z),
					Float2(mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y),
					Float3(mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z));
		}

		std::vector<IndexBuffer::Type> indices;
		indices.reserve(mesh.mNumFaces * 3);

		for (uint32_t i = 0; i < mesh.mNumFaces; ++i)
		{
			const auto& face = mesh.mFaces[i];
			RAY_MEXPECTS(face.mNumIndices == 3, "[Model] Was not triangulated");
			indices.emplace_back(face.mIndices[0]);
			indices.emplace_back(face.mIndices[1]);
			indices.emplace_back(face.mIndices[2]);
		}

		std::string basePath = std::filesystem::path(base).remove_filename().string();

		MeshData data{ Codex::Resolve<VertexBuffer>(meshTag, std::move(vertexInput)),
			Codex::Resolve<IndexBuffer>(meshTag, std::move(indices)),
			material ? *material : CreateMaterial(basePath, mesh, pMaterials, flags) };

		// RAY_TODO: Scene hierachy
		if (!m_RootMesh)
		{
			m_RootMesh = MakeScope<MeshData>(std::move(data));
		}
		else
		{
			m_RootMesh->children.emplace_back(std::move(data));
		}
	}

	Material Model::CreateMaterial(const std::string& basePath, const aiMesh& mesh,
		const aiMaterial* const* pMaterials, Model::Flags flags)
	{
		aiString diffuseTexFileName;
		aiString specularTexFileName;
		aiString normalTexFileName;

		Ref<Texture2D> diffuseMap = nullptr;
		Ref<Texture2D> specularMap = nullptr;
		Ref<Texture2D> normalMap = nullptr;

#ifndef NDEBUG
		if ((flags & Model::NoDiffuseMap) != 0)
			Log::Debug("[Model] Diffuse map for model \"{0}\" disabled.", basePath);

		if ((flags & Model::NoSpecularMap) != 0)
			Log::Debug("[Model] Diffuse map for model \"{0}\" disabled.", basePath);

		if ((flags & Model::NoNormalMap) != 0)
			Log::Debug("[Model] Diffuse map for model \"{0}\" disabled.", basePath);
#endif

		if (pMaterials[mesh.mMaterialIndex]->GetTexture(
				aiTextureType_DIFFUSE, 0, &diffuseTexFileName) == aiReturn_SUCCESS &&
			(flags & Model::NoDiffuseMap) == 0)
		{
			diffuseMap = MakeRef<Texture2D>(basePath + diffuseTexFileName.C_Str());
		}

		if (pMaterials[mesh.mMaterialIndex]->GetTexture(
				aiTextureType_SPECULAR, 0, &specularTexFileName) == aiReturn_SUCCESS &&
			(flags & Model::NoSpecularMap) == 0)
		{
			specularMap = MakeRef<Texture2D>(basePath + specularTexFileName.C_Str());
		}

		if (pMaterials[mesh.mMaterialIndex]->GetTexture(
				aiTextureType_NORMALS, 0, &normalTexFileName) == aiReturn_SUCCESS &&
			(flags & Model::NoNormalMap) == 0)
		{
			normalMap = MakeRef<Texture2D>(basePath + normalTexFileName.C_Str());
		}

		return Material{ Material::DiffuseMap(diffuseMap), Material::SpecularMap(specularMap),
			Material::NormalMap(normalMap) };
	}

	bool Model::HasNormalMap(
		const aiMesh& mesh, const aiMaterial* const* pMaterials, std::optional<Material> material)
	{
		aiString normalTexFileName("");
		aiReturn ret = pMaterials[mesh.mMaterialIndex]->GetTexture(
			aiTextureType_NORMALS, 0, &normalTexFileName);

// RAY_TODO: Check if std::optional<Material> material has normal map
#ifndef NDEBUG
		if (material)
			Log::Error("[Model] Missing implementation to check for the existence of a normal map "
					   "in a custom material");
#endif

		return ret == aiReturn_SUCCESS || normalTexFileName.length != 0;
	}

}  // namespace At0::Ray
