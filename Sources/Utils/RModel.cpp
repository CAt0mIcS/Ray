#include "Rpch.h"
#include "RModel.h"

#include "Core/RDynamicVertex.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Images/RTexture2D.h"
#include "Graphics/RCodex.h"
#include "Graphics/Buffers/RVertexBuffer.h"

#include "Shading/Phong/RPhongMaterial.h"
#include "Shading/Flat/RFlatColorMaterial.h"

#include "RException.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Components/RMeshRenderer.h"
#include "Components/RParentEntity.h"
#include "Scene/RScene.h"


namespace At0::Ray
{
	Model::Model(std::string_view filepath, Ref<Material> material)
	{
		Assimp::Importer importer;
		const aiScene* pScene = importer.ReadFile(filepath.data(),
			aiProcess_Triangulate | /*aiProcess_JoinIdenticalVertices |*/
				aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

		if (!pScene)
			RAY_THROW_RUNTIME("[Model] Failed to load: \"{0}\"", importer.GetErrorString());

		for (uint32_t i = 0; i < pScene->mNumMeshes; ++i)
		{
			ParseMesh(filepath, *pScene->mMeshes[i], pScene->mMaterials, material);
		}
	}

	void Model::ParseMesh(std::string_view filepath, const aiMesh& mesh,
		const aiMaterial* const* pMaterials, Ref<Material> material)
	{
		const std::string basePath = std::filesystem::path(filepath).remove_filename().string();
		const std::string meshTag = std::string(filepath) + std::string("#") + mesh.mName.C_Str();

		// Material creation stage
		if (!material)
			material = CreateMaterial(basePath, mesh, pMaterials);

		// Vertex assembly stage
		DynamicVertex vertices =
			AssembleVertices(mesh, material->GetGraphicsPipeline().GetShader());

		// Index generation stage
		std::vector<IndexBuffer::Type> indices = GenerateIndices(mesh);

		// Parents
		if (!m_ParentSet)
		{
			m_VertexData.vertexBuffer = Codex::Resolve<VertexBuffer>(meshTag, vertices);
			m_VertexData.indexBuffer = Codex::Resolve<IndexBuffer>(meshTag, indices);
			m_VertexData.material = std::move(material);

			// vertexBuffer = Codex::Resolve<VertexBuffer>(meshTag, vertices);
			// indexBuffer = Codex::Resolve<IndexBuffer>(meshTag, indices);
			m_ParentSet = true;
		}
		// Children
		else
		{
			// m_VertexData.children.emplace_back(Codex::Resolve<VertexBuffer>(meshTag, vertices),
			//	Codex::Resolve<IndexBuffer>(meshTag, indices));

			Ref<VertexBuffer> vertexBuffer = Codex::Resolve<VertexBuffer>(meshTag, vertices);
			Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(meshTag, indices);

			// ParentEntity component added by mesh
			Entity entity = Scene::Get().CreateEntity();
			Ray::MeshRenderer& meshRenderer =
				entity.Emplace<Ray::MeshRenderer>(std::move(material));
			entity.Emplace<Ray::Mesh>(Mesh::VertexData{ vertexBuffer, indexBuffer });
			m_VertexData.children.emplace_back(entity);
		}
	}

	Ref<Material> Model::CreateMaterial(
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

		PhongMaterial::Layout layout{};
		layout.diffuseMap = diffuseMap;
		layout.specularMap = specularMap;
		layout.normalMap = normalMap;

		return MakeRef<PhongMaterial>(layout);
	}

	DynamicVertex Model::AssembleVertices(const aiMesh& mesh, const Shader& shader)
	{
		DynamicVertex vertices(shader);

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

		return vertices;
	}

	std::vector<IndexBuffer::Type> Model::GenerateIndices(const aiMesh& mesh)
	{
		std::vector<IndexBuffer::Type> indices;
		indices.reserve(mesh.mNumFaces * 3);

		for (uint32_t i = 0; i < mesh.mNumFaces; ++i)
		{
			const aiFace& face = mesh.mFaces[i];
			RAY_MEXPECTS(face.mNumIndices == 3, "[Model] Was not triangulated");
			indices.emplace_back(face.mIndices[0]);
			indices.emplace_back(face.mIndices[1]);
			indices.emplace_back(face.mIndices[2]);
		}

		return indices;
	}
}  // namespace At0::Ray
