#include "RayRender/rlrpch.h"
#include "ShadedCube.h"

#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RAssert.h>

#include "../Bindable/VertexBuffer.h"
#include "../Bindable/InputLayout.h"
#include "../Bindable/IndexBuffer.h"
#include "../Bindable/ConstantBuffers.h"
#include "../Bindable/Topology.h"
#include "../Bindable/VertexShader.h"
#include "../Bindable/PixelShader.h"

namespace At0::Ray
{

	template<typename V>
	struct IndexedTriangleList
	{
	public:
		IndexedTriangleList(std::vector<V> vertices, std::vector<uint16_t> indices)
			: vertices(std::move(vertices)), indices(std::move(indices))
		{
		}

		void SetNormalsIndependentFlat()
		{
			for (size_t i = 0; i < indices.size(); i += 3)
			{
				auto& v0 = vertices[indices[i]];
				auto& v1 = vertices[indices[i + 1]];
				auto& v2 = vertices[indices[i + 2]];
				auto p0 = LoadFloat3(&v0.pos);
				auto p1 = LoadFloat3(&v1.pos);
				auto p2 = LoadFloat3(&v2.pos);

				auto n = Vector3Normalize(Vector3Cross(p1 - p0, p2 - p0));

				StoreFloat3(&v0.n, n);
				StoreFloat3(&v1.n, n);
				StoreFloat3(&v2.n, n);
			}
		}

		std::vector<V> vertices;
		std::vector<uint16_t> indices;
	};

	template<class V>
	static IndexedTriangleList<V> MakeIndependentCube()
	{
		constexpr float side = 1.0f / 2.0f;

		std::vector<V> vertices(24);
		vertices[0].pos = { -side,-side,-side };// 0 near side
		vertices[1].pos = { side,-side,-side };// 1
		vertices[2].pos = { -side,side,-side };// 2
		vertices[3].pos = { side,side,-side };// 3
		vertices[4].pos = { -side,-side,side };// 4 far side
		vertices[5].pos = { side,-side,side };// 5
		vertices[6].pos = { -side,side,side };// 6
		vertices[7].pos = { side,side,side };// 7
		vertices[8].pos = { -side,-side,-side };// 8 left side
		vertices[9].pos = { -side,side,-side };// 9
		vertices[10].pos = { -side,-side,side };// 10
		vertices[11].pos = { -side,side,side };// 11
		vertices[12].pos = { side,-side,-side };// 12 right side
		vertices[13].pos = { side,side,-side };// 13
		vertices[14].pos = { side,-side,side };// 14
		vertices[15].pos = { side,side,side };// 15
		vertices[16].pos = { -side,-side,-side };// 16 bottom side
		vertices[17].pos = { side,-side,-side };// 17
		vertices[18].pos = { -side,-side,side };// 18
		vertices[19].pos = { side,-side,side };// 19
		vertices[20].pos = { -side,side,-side };// 20 top side
		vertices[21].pos = { side,side,-side };// 21
		vertices[22].pos = { -side,side,side };// 22
		vertices[23].pos = { side,side,side };// 23

		return{
			std::move(vertices),{
				0,2, 1,    2,3,1,
				4,5, 7,    4,7,6,
				8,10, 9,  10,11,9,
				12,13,15, 12,15,14,
				16,17,18, 18,17,19,
				20,23,21, 20,22,23
			}
		};
	}




	std::vector<Scope<Bindable>> ShadedCube::s_StaticBinds;

	ShadedCube::ShadedCube(Renderer3D& renderer)
	{
		struct Vertex
		{
			Float3 pos;
			Float3 n;
		};

		if (s_StaticBinds.empty())
		{
			auto model = MakeIndependentCube<Vertex>();
			model.SetNormalsIndependentFlat();

			AddStaticBind(MakeScope<VertexBuffer>(model.vertices));

			Scope<VertexShader> pvs = MakeScope<VertexShader>("VertexShader(ShadedCube)-v.cso");
			ID3DBlob* pvsb = pvs->GetBytecode();
			AddStaticBind(std::move(pvs));

			AddStaticBind(MakeScope<PixelShader>("PixelShader(ShadedCube)-p.cso"));

			AddStaticIndexBuffer(MakeScope<IndexBuffer>(model.indices));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied
			{
				{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
				{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			};
			AddStaticBind(MakeScope<InputLayout>(ied, pvsb));
			AddStaticBind(MakeScope<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		}
		else
		{
			SetIndexFromStatic();
		}

		AddBind(MakeScope<TransformConstantBuffer>(renderer, *this));

		// Randomize zDir scaling
		//static std::mt19937 mtEngine;
		//static std::uniform_real_distribution<float> zScaleDist(-5.0f, 5.0f);
		//static float scale = zScaleDist(mtEngine);
		//SetScale(1.0f, 1.0f, scale);
	}

	void ShadedCube::AddStaticBind(Scope<Bindable> bind)
	{
		RAY_PROFILE_FUNCTION();
		RAY_ASSERT(typeid(*bind) != typeid(IndexBuffer), "Trying to add an IndexBuffer with Drawable::AddBind, use Drawable::AddIndexBuffer.");
		s_StaticBinds.push_back(std::move(bind));
	}

	void ShadedCube::AddStaticIndexBuffer(Scope<IndexBuffer> bind)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(m_pIndexBuffer == nullptr, "IndexBuffer already bound.");
		m_pIndexBuffer = bind.get();
		s_StaticBinds.push_back(std::move(bind));
	}

	void ShadedCube::SetIndexFromStatic()
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
}