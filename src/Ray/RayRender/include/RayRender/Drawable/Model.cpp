#include "RayRender/rlrpch.h"
#include "Model.h"

#include "../Bindable/IndexBuffer.h"
#include "../Bindable/Topology.h"
#include "../Bindable/InputLayout.h"
#include "../Bindable/VertexShader.h"
#include "../Bindable/PixelShader.h"
#include "../Bindable/VertexBuffer.h"
#include "../Bindable/ConstantBuffers.h"

#include "../Renderer3D.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>
#include <RayUtil/RException.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


namespace At0::Ray
{
	std::vector<Scope<Bindable>> Model::s_StaticBinds;

	Model::Model(std::string_view filepath, const float colors[6][3], const Renderer3D& renderer)
	{
		Assimp::Importer importer;
		const aiScene* pScene = importer.ReadFile(filepath.data(),
			aiProcess_Triangulate | aiProcess_ConvertToLeftHanded
		);

		if (!pScene)
			RAY_THROW_RUNTIME("Failed to read model file: '{0}'", filepath);

		ProcessNode(pScene->mRootNode, pScene);

		// General Initialization

		struct PixelConstantColorBuffer
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[6];
		};

		Scope<VertexShader> vshader = MakeScope<VertexShader>("VertexShader(Cube)-v.cso");
		ID3DBlob* vshaderbytecode = vshader->GetBytecode();
		AddBind(std::move(vshader));
		AddBind(MakeScope<PixelShader>("PixelShader(Cube)-p.cso"));
		AddBind(MakeScope<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		std::vector<D3D11_INPUT_ELEMENT_DESC> ied
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		AddBind(MakeScope<InputLayout>(ied, vshaderbytecode));

		const PixelConstantColorBuffer pccb
		{
			{
				{ colors[0][0], colors[0][1], colors[0][2] },
				{ colors[1][0], colors[1][1], colors[1][2] },
				{ colors[2][0], colors[2][1], colors[2][2] },
				{ colors[3][0], colors[3][1], colors[3][2] },
				{ colors[4][0], colors[4][1], colors[4][2] },
				{ colors[5][0], colors[5][1], colors[5][2] },
			}
		};

		AddStaticBind(MakeScope<PixelConstantBuffer<PixelConstantColorBuffer>>(pccb));

		AddBind(MakeScope<TransformConstantBuffer>(renderer, *this));
	}

	void Model::Update()
	{

	}

	void Model::Draw(Renderer3D* renderer)
	{
		Bind();

		for (auto& mesh : m_Meshes)
		{
			mesh.Draw(renderer);
		}
	}

	void Model::AddStaticBind(Scope<Bindable> bind)
	{
		RAY_PROFILE_FUNCTION();
		RAY_ASSERT(typeid(*bind) != typeid(IndexBuffer), "Trying to add an IndexBuffer with Drawable::AddBind, use Drawable::AddIndexBuffer.");
		s_StaticBinds.push_back(std::move(bind));
	}

	void Model::AddStaticIndexBuffer(Scope<IndexBuffer> bind)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(m_pIndexBuffer == nullptr, "IndexBuffer already bound.");
		m_pIndexBuffer = bind.get();
		s_StaticBinds.push_back(std::move(bind));
	}

	void Model::SetIndexFromStatic()
	{
		RAY_PROFILE_FUNCTION();
		for (Scope<Bindable>& bindable : s_StaticBinds)
		{
			if (const IndexBuffer* p = dynamic_cast<IndexBuffer*>(bindable.get()))
			{
				m_pIndexBuffer = p;
				break;
			}
		}
	}
	
	void Model::ProcessNode(aiNode* pNode, const aiScene* pScene)
	{
		for (uint32_t i = 0; i < pNode->mNumMeshes; ++i)
		{
			aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(pMesh, pScene));
		}

		for (uint32_t i = 0; i < pNode->mNumChildren; ++i)
		{
			ProcessNode(pNode->mChildren[i], pScene);
		}
	}
	
	Mesh Model::ProcessMesh(aiMesh* pMesh, const aiScene* pScene)
	{
		struct Vertex
		{
			struct
			{
				float x, y, z;
			} pos;
		};

		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;

		for (uint32_t i = 0; i < pMesh->mNumVertices; ++i)
		{
			Vertex v;
			v.pos.x = pMesh->mVertices[i].x;
			v.pos.y = pMesh->mVertices[i].y;
			v.pos.z = pMesh->mVertices[i].z;

			vertices.push_back(std::move(v));
		}

		for (uint32_t i = 0; i < pMesh->mNumFaces; ++i)
		{
			for (uint32_t j = 0; j < pMesh->mFaces[i].mNumIndices; ++j)
				indices.emplace_back(pMesh->mFaces[i].mIndices[j]);
		}

		return Mesh(vertices, indices);
	}
}