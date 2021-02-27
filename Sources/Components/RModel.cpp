#include "Rpch.h"
#include "RModel.h"

#include "RMesh.h"
#include "Core/RVertex.h"
#include "Graphics/Core/RCodex.h"
#include "Utils/RException.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace At0::Ray
{
	Model::Model(std::string_view filepath)
	{
		Assimp::Importer imp;
		const aiScene* pScene = imp.ReadFile(
			filepath.data(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
								 aiProcess_ConvertToLeftHanded |
								 aiProcess_GenNormals /* | aiProcess_CalcTangentSpace*/);

		if (!pScene)
			RAY_THROW_RUNTIME("Failed to load model: {0}", imp.GetErrorString());

		for (uint32_t i = 0; i < pScene->mNumMeshes; ++i)
		{
			m_Meshes.emplace_back(ParseMesh(filepath, *pScene->mMeshes[i], pScene->mMaterials));
		}
	}

	Model::~Model() {}

	void Model::Update(Delta ts)
	{
		for (Mesh& mesh : m_Meshes)
			mesh.Update(ts);
	}

	void Model::Render(const CommandBuffer& cmdBuff) const
	{
		for (const Mesh& mesh : m_Meshes)
		{
			mesh.Bind(cmdBuff);
			mesh.Render(cmdBuff);
		}
	}

	Mesh Model::ParseMesh(
		std::string_view base, const aiMesh& mesh, const aiMaterial* const* pMaterials)
	{
		using namespace std::string_literals;
		const std::string meshTag = std::string(base) + "%"s + mesh.mName.C_Str();

		VertexLayout layout{};
		layout.Append(VK_FORMAT_R32G32B32_SFLOAT);	// Position
		// layout.Append(VK_FORMAT_R32G32B32_SFLOAT; // Normal

		VertexInput vertexInput(layout);

		for (uint32_t i = 0; i < mesh.mNumVertices; ++i)
		{
			vertexInput.Emplace(
				glm::vec3(mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z)); /*,
				glm::vec3(mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z));*/
		}

		std::vector<IndexBuffer::Type> indices;
		indices.reserve(mesh.mNumFaces * 3);

		for (uint32_t i = 0; i < mesh.mNumFaces; ++i)
		{
			const auto& face = mesh.mFaces[i];
			RAY_MEXPECTS(face.mNumIndices == 3, "Model was not triangulated.");
			indices.emplace_back(face.mIndices[0]);
			indices.emplace_back(face.mIndices[1]);
			indices.emplace_back(face.mIndices[2]);
		}

		std::vector<std::string_view> shaders{ "Resources/Shaders/DefaultShader.vert",
			"Resources/Shaders/DefaultShader.frag" };
		Material material(
			Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderPass(), shaders));

		return { Codex::Resolve<VertexBuffer>(meshTag, std::move(vertexInput)),
			Codex::Resolve<IndexBuffer>(meshTag, std::move(indices)), std::move(material) };
	}

	Model& Model::operator=(Model&& other)
	{
		m_Meshes = std::move(other.m_Meshes);
		return *this;
	}

	Model::Model(Model&& other) : m_Meshes(std::move(other.m_Meshes)) {}
}  // namespace At0::Ray
