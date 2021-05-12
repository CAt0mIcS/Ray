#include "Rpch.h"
#include "RModel.h"

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
	// Model::Model(std::string_view filepath)
	//{
	//	Log::Info("[Model] Importing model \"{0}\"", filepath);

	//	const aiScene* pScene = nullptr;
	//	Assimp::Importer imp;
	//	pScene = imp.ReadFile(filepath.data(),
	//		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
	//			aiProcess_ConvertToLeftHanded |
	//			(aiProcess_CalcTangentSpace ? (flags & Model::NoNormalMap) == 0 : 0));

	//	if (!pScene)
	//		RAY_THROW_RUNTIME("[Model] Failed to load: \"{0}\"", imp.GetErrorString());

	//	for (uint32_t i = 0; i < pScene->mNumMeshes; ++i)
	//	{
	//		ParseMesh(filepath, *pScene->mMeshes[i], pScene->mMaterials);
	//	}
	//}

	// Model::~Model() {}

	// std::string Model::GetUID(std::string_view filepath)
	//{
	//	// RAY_TODO: Take custom material into account
	//	std::ostringstream oss;
	//	oss << filepath;
	//	return oss.str();
	//}

	// void Model::ParseMesh(
	//	std::string_view base, const aiMesh& mesh, const aiMaterial* const* pMaterials)
	//{
	//	Log::Info("[Model] Parsing mesh \"{0}\"", mesh.mName.C_Str());

	//	using namespace std::string_literals;
	//	const std::string meshTag = std::string(base) + "%"s + mesh.mName.C_Str();


	//	std::vector<IndexBuffer::Type> indices;
	//	indices.reserve(mesh.mNumFaces * 3);

	//	for (uint32_t i = 0; i < mesh.mNumFaces; ++i)
	//	{
	//		const auto& face = mesh.mFaces[i];
	//		RAY_MEXPECTS(face.mNumIndices == 3, "[Model] Was not triangulated");
	//		indices.emplace_back(face.mIndices[0]);
	//		indices.emplace_back(face.mIndices[1]);
	//		indices.emplace_back(face.mIndices[2]);
	//	}

	//	std::string basePath = std::filesystem::path(base).remove_filename().string();

	//	// RAY_TODO:
	//	Scene hierachy if (m_RootMesh)
	//	{
	//		m_RootMesh->children.emplace_back(MeshData{
	//			Codex::Resolve<VertexBuffer>(meshTag, std::move(vertexInput)),
	//			Codex::Resolve<IndexBuffer>(meshTag, std::move(indices)),
	//			material ? *material : CreateMaterial(basePath, mesh, pMaterials, flags, config) });
	//	}
	//	else
	//	{
	//		m_RootMesh = MeshData{ Codex::Resolve<VertexBuffer>(meshTag, std::move(vertexInput)),
	//			Codex::Resolve<IndexBuffer>(meshTag, std::move(indices)),
	//			material ? *material : CreateMaterial(basePath, mesh, pMaterials, flags, config) };
	//	}
	//}

	// Material Model::CreateMaterial(const std::string& basePath, const aiMesh& mesh)
	//{
	//	aiString diffuseTexFileName;
	//	aiString specularTexFileName;
	//	aiString normalTexFileName;

	//	Ref<Texture2D> diffuseMap = nullptr;
	//	Ref<Texture2D> specularMap = nullptr;
	//	Ref<Texture2D> normalMap = nullptr;

	//	if (pMaterials[mesh.mMaterialIndex]->GetTexture(
	//			aiTextureType_DIFFUSE, 0, &diffuseTexFileName) == aiReturn_SUCCESS &&
	//		(flags & Model::NoDiffuseMap) == 0 && !config.diffuseMap.value)
	//	{
	//		diffuseMap = MakeRef<Texture2D>(basePath + diffuseTexFileName.C_Str());
	//		config.diffuseMap = diffuseMap;
	//	}

	//	if (pMaterials[mesh.mMaterialIndex]->GetTexture(
	//			aiTextureType_SPECULAR, 0, &specularTexFileName) == aiReturn_SUCCESS &&
	//		(flags & Model::NoSpecularMap) == 0 && !config.specularMap.value)
	//	{
	//		specularMap = MakeRef<Texture2D>(basePath + specularTexFileName.C_Str());
	//		config.specularMap = specularMap;
	//	}

	//	if (pMaterials[mesh.mMaterialIndex]->GetTexture(
	//			aiTextureType_NORMALS, 0, &normalTexFileName) == aiReturn_SUCCESS &&
	//		(flags & Model::NoNormalMap) == 0 && !config.normalMap.value)
	//	{
	//		normalMap = MakeRef<Texture2D>(basePath + normalTexFileName.C_Str());
	//		config.normalMap = normalMap;
	//	}

	//	return Material{ config };
	//}
}  // namespace At0::Ray
