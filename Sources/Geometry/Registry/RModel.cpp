#include "Rpch.h"
#include "RModel.h"

#include "../RMesh.h"
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
	Model::Model(std::string_view filepath)
	{
		Log::Info("[Model] Importing model \"{0}\"", filepath);

		Assimp::Importer imp;
		const aiScene* pScene = imp.ReadFile(
			filepath.data(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
								 aiProcess_ConvertToLeftHanded |
								 aiProcess_GenNormals /* | aiProcess_CalcTangentSpace*/);

		if (!pScene)
			RAY_THROW_RUNTIME("[Model] Failed to load: \"{0}\"", imp.GetErrorString());

		for (uint32_t i = 0; i < pScene->mNumMeshes; ++i)
		{
			ParseMesh(filepath, *pScene->mMeshes[i], pScene->mMaterials);
		}
	}

	void Model::ParseMesh(
		std::string_view base, const aiMesh& mesh, const aiMaterial* const* pMaterials)
	{
		Log::Info("[Model] Parsing mesh \"{0}\"", mesh.mName.C_Str());

		using namespace std::string_literals;
		const std::string meshTag = std::string(base) + "%"s + mesh.mName.C_Str();

		VertexLayout layout{};
		layout.Append(VK_FORMAT_R32G32B32_SFLOAT);	// Position
		layout.Append(VK_FORMAT_R32G32_SFLOAT);		// Texture coordinate
		layout.Append(VK_FORMAT_R32G32B32_SFLOAT);	// Normal

		VertexInput vertexInput(layout);

		for (uint32_t i = 0; i < mesh.mNumVertices; ++i)
		{
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

		Mesh::MeshData data{ Codex::Resolve<VertexBuffer>(meshTag, std::move(vertexInput)),
			Codex::Resolve<IndexBuffer>(meshTag, std::move(indices)),
			CreateMaterial(basePath, mesh, pMaterials) };

		// RAY_TODO: Scene hierachy
		if (!m_RootMesh)
		{
			m_RootMesh = MakeScope<Mesh::MeshData>(std::move(data));
		}
		else
		{
			m_RootMesh->children.emplace_back(std::move(data));
		}
	}

	Material Model::CreateMaterial(
		const std::string& basePath, const aiMesh& mesh, const aiMaterial* const* pMaterials)
	{
		aiString diffuseTexFileName;
		aiString specularTexFileName;
		aiString normalTexFileName;

		Ref<Texture2D> diffuseMap = nullptr;
		Ref<Texture2D> specularMap = nullptr;
		Ref<Texture2D> normalMap = nullptr;

		if (pMaterials[mesh.mMaterialIndex]->GetTexture(
				aiTextureType_DIFFUSE, 0, &diffuseTexFileName) == aiReturn_SUCCESS)
		{
			diffuseMap = MakeRef<Texture2D>(basePath + diffuseTexFileName.C_Str());
		}

		if (pMaterials[mesh.mMaterialIndex]->GetTexture(
				aiTextureType_SPECULAR, 0, &specularTexFileName) == aiReturn_SUCCESS)
		{
			specularMap = MakeRef<Texture2D>(basePath + specularTexFileName.C_Str());
		}

		if (pMaterials[mesh.mMaterialIndex]->GetTexture(
				aiTextureType_NORMALS, 0, &normalTexFileName) == aiReturn_SUCCESS)
		{
			normalMap = MakeRef<Texture2D>(basePath + normalTexFileName.C_Str());
		}

		std::vector<std::string> shaders =
			GetShaders(diffuseMap != nullptr, specularMap != nullptr, normalMap != nullptr);

		return { shaders, { 1.0f, 1.0f, 1.0f, 1.0f }, diffuseMap, specularMap, normalMap };
	}

	std::vector<std::string> Model::GetShaders(
		bool hasDiffuseMap, bool hasSpecularMap, bool hasNormalMap)
	{
		std::vector<std::string> shaders;
		std::vector<std::string> shaderCodes;

		if (hasDiffuseMap)
			shaderCodes.emplace_back("Diff");
		if (hasSpecularMap)
			shaderCodes.emplace_back("Spec");
		if (hasNormalMap)
			shaderCodes.emplace_back("Norm");

		// Sort shader codes alphabetically
		std::sort(shaderCodes.begin(), shaderCodes.end());

		shaderCodes.emplace(shaderCodes.begin(), "_");
		std::string shaderCode =
			std::accumulate(shaderCodes.begin(), shaderCodes.end(), std::string{});

		shaders.emplace_back("Resources/Shaders/ModelShader" + shaderCode + ".vert");
		shaders.emplace_back("Resources/Shaders/ModelShader" + shaderCode + ".frag");

		return shaders;
	}
}  // namespace At0::Ray
