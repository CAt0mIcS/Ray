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
#include "Core/RDynamicVertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace At0::Ray
{
	Model::Model(std::string_view filepath, Material::Layout layout)
	{
		m_Data.layout = std::move(layout);

		Log::Info("[Model] Importing model \"{0}\"", filepath);

		const aiScene* pScene = nullptr;
		Assimp::Importer imp;
		pScene = imp.ReadFile(
			filepath.data(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
								 aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace);

		if (!pScene)
			RAY_THROW_RUNTIME("[Model] Failed to load: \"{0}\"", imp.GetErrorString());

		for (uint32_t i = 0; i < pScene->mNumMeshes; ++i)
		{
			ParseMesh(filepath, *pScene->mMeshes[i], pScene->mMaterials);
		}
	}

	Model::~Model() {}

	std::string Model::GetUID(std::string_view filepath, Material::Layout layout)
	{
		// RAY_TODO: Take custom material into account
		std::ostringstream oss;
		oss << filepath;
		return oss.str();
	}

	void Model::ParseMesh(
		std::string_view base, const aiMesh& mesh, const aiMaterial* const* pMaterials)
	{
		Log::Info("[Model] Parsing mesh \"{0}\"", mesh.mName.C_Str());

		using namespace std::string_literals;
		const std::string meshTag = std::string(base) + "%"s + mesh.mName.C_Str();
		std::string basePath = std::filesystem::path(base).remove_filename().string();

		m_Data.layout = CreateMaterial(basePath, mesh, pMaterials, std::move(m_Data.layout));
		DynamicVertex vertices(*Codex::Resolve<Shader>(m_Data.layout.shaders));

		bool hasPos = vertices.Has(AttributeMap<AttributeType::Position>::Semantic);
		bool hasUV = vertices.Has(AttributeMap<AttributeType::UV>::Semantic);
		bool hasNormal = vertices.Has(AttributeMap<AttributeType::Normal>::Semantic);
		bool hasTangent = vertices.Has(AttributeMap<AttributeType::Tangent>::Semantic);

		for (uint32_t i = 0; i < mesh.mNumVertices; ++i)
		{
			vertices.BeginVertex();

			if (hasPos)
				vertices[AttributeMap<AttributeType::Position>::Semantic] =
					Float3{ mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z };
			if (hasUV)
				vertices[AttributeMap<AttributeType::UV>::Semantic] =
					Float2{ mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y };
			if (hasNormal)
				vertices[AttributeMap<AttributeType::Normal>::Semantic] =
					Float3{ mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z };
			if (hasTangent)
				vertices[AttributeMap<AttributeType::Tangent>::Semantic] =
					Float3{ mesh.mTangents[i].x, mesh.mTangents[i].y, mesh.mTangents[i].z };
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


		// RAY_TODO: Scene hierachy
		if (!m_Data.vertexBuffer || !m_Data.indexBuffer)
		{
			m_Data.vertexBuffer = Codex::Resolve<VertexBuffer>(meshTag, std::move(vertices));
			m_Data.indexBuffer = Codex::Resolve<IndexBuffer>(meshTag, std::move(indices));
		}
		else
		{
			Model::Data data{};
			data.vertexBuffer = Codex::Resolve<VertexBuffer>(meshTag, std::move(vertices));
			data.indexBuffer = Codex::Resolve<IndexBuffer>(meshTag, std::move(indices));
			data.layout = m_Data.layout;

			m_Data.children.emplace_back(data);
		}
	}

	Material::Layout Model::CreateMaterial(const std::string& basePath, const aiMesh& mesh,
		const aiMaterial* const* pMaterials, Material::Layout layout)
	{
		aiString diffuseTexFileName;
		aiString specularTexFileName;
		aiString normalTexFileName;

		Ref<Texture2D> diffuseMap = nullptr;
		Ref<Texture2D> specularMap = nullptr;
		Ref<Texture2D> normalMap = nullptr;

		if (pMaterials[mesh.mMaterialIndex]->GetTexture(
				aiTextureType_DIFFUSE, 0, &diffuseTexFileName) == aiReturn_SUCCESS &&
			!layout.diffuseMap)
		{
			diffuseMap = MakeRef<Texture2D>(basePath + diffuseTexFileName.C_Str());
			layout.diffuseMap = diffuseMap;
		}

		if (pMaterials[mesh.mMaterialIndex]->GetTexture(
				aiTextureType_SPECULAR, 0, &specularTexFileName) == aiReturn_SUCCESS &&
			!layout.specularMap)
		{
			specularMap = MakeRef<Texture2D>(basePath + specularTexFileName.C_Str());
			layout.specularMap = specularMap;
		}

		if (pMaterials[mesh.mMaterialIndex]->GetTexture(
				aiTextureType_NORMALS, 0, &normalTexFileName) == aiReturn_SUCCESS &&
			!layout.normalMap)
		{
			normalMap = MakeRef<Texture2D>(basePath + normalTexFileName.C_Str());
			layout.normalMap = normalMap;
		}

		return layout;
	}
}  // namespace At0::Ray
