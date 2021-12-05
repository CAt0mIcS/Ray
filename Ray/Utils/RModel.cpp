#include "Rpch.h"
#include "RModel.h"

#include "Core/RDynamicVertex.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Images/RTexture2D.h"
#include "Graphics/RCodex.h"
#include "Graphics/Buffers/RVertexBuffer.h"

#include "RException.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Components/RMeshRenderer.h"
#include "Components/RParentEntity.h"
#include "Scene/RScene.h"
#include "Core/RTime.h"

#define RAY_MULTITHREADED_IMPORT 1


namespace At0::Ray
{
	struct ShaderFileCode
	{
		static constexpr const char* DiffuseMap = "_Diff";
		static constexpr const char* Metallic = "_Metal";
		static constexpr const char* SpecularMap = "_Spec";
		static constexpr const char* NormalMap = "_Norm";
		static constexpr const char* HeightMap = "_Height";
		static constexpr const char* Occlusion = "_Occl";
		static constexpr const char* DetailMask = "_DtlMsk";
		static constexpr const char* Emission = "_Emi";
		static constexpr const char* Color = "_Col";
	};

	Model::Model(std::string_view filepath, Ref<Material> material)
	{
		Assimp::Importer importer;
		const aiScene* pScene = importer.ReadFile(filepath.data(),
			aiProcess_Triangulate | /*aiProcess_JoinIdenticalVertices |*/
				aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

		if (!pScene)
			ThrowRuntime("[Model] Failed to load: \"{0}\"", importer.GetErrorString());

		Time start = Time::Now();

#if RAY_MULTITHREADED_IMPORT
		std::vector<std::future<void>> futures;
		std::mutex mutexMaterial;
		std::mutex mutexScene;
		auto parseMeshAsync = [this, &mutexMaterial, &mutexScene](std::string_view filepath,
								  const aiMesh* pMesh, const aiMaterial* const* pMaterials,
								  Ref<Material> material)
		{
			const aiMesh& mesh = *pMesh;
			const std::string basePath = std::filesystem::path(filepath).remove_filename().string();
			const std::string meshTag =
				std::string(filepath) + std::string("#") + mesh.mName.C_Str();

			// Material creation stage
			if (!material)
			{
				std::scoped_lock lock(mutexMaterial);
				material = CreateMaterial(basePath, mesh, pMaterials);
			}

			// Vertex assembly stage
			DynamicVertex vertices =
				AssembleVertices(mesh, material->GetGraphicsPipeline().GetShader());

			// Index generation stage
			std::vector<IndexBuffer::Type> indices = GenerateIndices(mesh);

			// Parents
			std::scoped_lock lock(mutexScene);
			if (!m_ParentSet)
			{
				m_VertexData.vertexBuffer = Codex::Resolve<VertexBuffer>(meshTag, vertices);
				m_VertexData.indexBuffer = Codex::Resolve<IndexBuffer>(meshTag, indices);
				m_VertexData.material = std::move(material);
				m_ParentSet = true;
			}
			// Children
			else
			{
				Ref<VertexBuffer> vertexBuffer = Codex::Resolve<VertexBuffer>(meshTag, vertices);
				Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(meshTag, indices);

				// ParentEntity component added by mesh
				Entity entity = Scene::Get().CreateEntity();
				Ray::MeshRenderer& meshRenderer =
					entity.Emplace<Ray::MeshRenderer>(std::move(material));
				entity.Emplace<Ray::Mesh>(Mesh::VertexData{ vertexBuffer, indexBuffer });
				m_VertexData.children.emplace_back(entity);
			}

			Log::Info(
				"[Model] Loaded mesh \"{0}\" of resource \"{1}\"", mesh.mName.C_Str(), filepath);
		};

		for (uint32_t i = 0; i < pScene->mNumMeshes; ++i)
		{
			futures.push_back(std::async(std::launch::async, parseMeshAsync, filepath,
				pScene->mMeshes[i], pScene->mMaterials, material));
		}

		// RAY_TODO: Finish while scene is already rendering
		for (auto& future : futures)
			future.get();
#else
		for (uint32_t i = 0; i < pScene->mNumMeshes; ++i)
		{
			ParseMesh(filepath, *pScene->mMeshes[i], pScene->mMaterials, material);
		}
#endif

		Log::Info("[Model] Loading of resource \"{0}\" took {1}s", filepath,
			(Time::Now() - start).AsSeconds());
	}

	void Model::ParseMesh(std::string_view filepath, const aiMesh& mesh,
		const aiMaterial* const* pMaterials, Ref<Material> material)
	{
		const std::string basePath = std::filesystem::path(filepath).remove_filename().string();
		const std::string meshTag = std::string(filepath) + std::string("#") + mesh.mName.C_Str();

		Log::Info("[Model] Parsing mesh \"{0}\" of resource \"{1}\"", mesh.mName.C_Str(), filepath);

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
			m_ParentSet = true;
		}
		// Children
		else
		{
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
			diffuseMap = Texture2D::Acquire(basePath + diffuseTexFileName.C_Str());
		}

		if (pMaterials[mesh.mMaterialIndex]->GetTexture(
				aiTextureType_SPECULAR, 0, &specularTexFileName) == aiReturn_SUCCESS)
		{
			specularMap = Texture2D::Acquire(basePath + specularTexFileName.C_Str());
		}

		if (pMaterials[mesh.mMaterialIndex]->GetTexture(
				aiTextureType_NORMALS, 0, &normalTexFileName) == aiReturn_SUCCESS)
		{
			normalMap = Texture2D::Acquire(basePath + normalTexFileName.C_Str());
		}

		std::string shaderFileName = "Resources/Shaders/Phong";
		std::vector<std::string> fileCodes;
		if (diffuseMap)
			fileCodes.emplace_back(ShaderFileCode::DiffuseMap);
		if (specularMap)
			fileCodes.emplace_back(ShaderFileCode::SpecularMap);
		if (normalMap)
			fileCodes.emplace_back(ShaderFileCode::NormalMap);
		std::sort(fileCodes.begin(), fileCodes.end());
		shaderFileName += std::accumulate(fileCodes.begin(), fileCodes.end(), std::string{});

		auto pipeline = Ray::GraphicsPipeline::Builder()
							.SetShader(Ray::Shader::Acquire(
								{ shaderFileName + ".vert", shaderFileName + ".frag" }))
							.Acquire();

		auto builder = Ray::Material::Builder(std::move(pipeline));
		if (diffuseMap)
			builder.Set(UniformTag::DiffuseMapSampler, diffuseMap);
		if (specularMap)
			builder.Set(UniformTag::SpecularMapSampler, specularMap);
		if (normalMap)
			builder.Set(UniformTag::NormalMapSampler, normalMap);

		return builder.Acquire();
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
