#include "rlrpch.h"
#include "Cube.h"

#include "RlRender/Bindable/VertexBuffer.h"
#include "RlRender/Bindable/VertexShader.h"
#include "RlRender/Bindable/PixelShader.h"
#include "RlRender/Bindable/Topology.h"
#include "RlRender/Bindable/InputLayout.h"
#include "RlRender/Bindable/IndexBuffer.h"
#include "RlRender/Bindable/ConstantBuffers.h"

#include <RlDebug/RlAssert.h>
#include <RlDebug/Instrumentor.h>


namespace At0::Reyal
{
	std::vector<Scope<Bindable>> Cube::s_StaticBinds;


	Cube::Cube(const Renderer3D& renderer, float size, const float colors[6][3])
	{
		RL_PROFILE_FUNCTION();

		struct Vertex
		{
			struct
			{
				float x, y, z;
			} pos;
		};

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

		if (s_StaticBinds.empty())
		{
			float side = size / 2.0f;
			const std::vector<Vertex> vertices
			{
				{ -side, -side, -side },
				{  side, -side, -side },
				{ -side,  side, -side },
				{  side,  side, -side },
				{ -side, -side,  side },
				{  side, -side,  side },
				{ -side,  side,  side },
				{  side,  side,  side },
			};

			AddStaticBind(MakeScope<VertexBuffer>(vertices));
			auto vertexShader = MakeScope<VertexShader>("VertexShader(Cube)-v.cso");
			ID3DBlob* vbytecode = vertexShader->GetBytecode();
			AddStaticBind(std::move(vertexShader));

			const std::vector<uint16_t> indices
			{
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			};
			AddStaticIndexBuffer(MakeScope<IndexBuffer>(indices));

			AddStaticBind(MakeScope<PixelShader>("PixelShader(Cube)-p.cso"));

			AddStaticBind(MakeScope<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutVec
			{
				{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			AddStaticBind(MakeScope<InputLayout>(inputLayoutVec, vbytecode));

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
		}
		else
		{
			SetIndexFromStatic();
		}

		AddBind(MakeScope<TransformConstantBuffer>(renderer, *this));
	}

	void Cube::Update()
	{
	}

	void Cube::AddStaticBind(Scope<Bindable> bind)
	{
		RL_PROFILE_FUNCTION();
		RL_ASSERT(typeid(*bind) != typeid(IndexBuffer), "Trying to add an IndexBuffer with Drawable::AddBind, use Drawable::AddIndexBuffer.");
		s_StaticBinds.push_back(std::move(bind));
	}

	void Cube::AddStaticIndexBuffer(Scope<IndexBuffer> bind)
	{
		RL_PROFILE_FUNCTION();
		RL_MEXPECTS(m_pIndexBuffer == nullptr, "IndexBuffer already bound.");
		m_pIndexBuffer = bind.get();
		s_StaticBinds.push_back(std::move(bind));
	}

	void Cube::SetIndexFromStatic()
	{
		RL_PROFILE_FUNCTION();
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


