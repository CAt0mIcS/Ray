#include "RayRender/rlrpch.h"
#include "Rectangle.h"

#include "../Bindable/IndexBuffer.h"
#include "../Bindable/VertexBuffer.h"
#include "../Bindable/VertexShader.h"
#include "../Bindable/PixelShader.h"
#include "../Bindable/Topology.h"
#include "../Bindable/InputLayout.h"
#include "../Bindable/ConstantBuffers.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>

namespace At0::Ray
{
	std::vector<Scope<Bindable>> Rectangle::s_StaticBinds;

	Rectangle::Rectangle(const Renderer3D& renderer, float size, const float color[3])
	{
		struct Vertex
		{
			struct
			{
				float x, y;
			} pos;
		};

		if (s_StaticBinds.empty())
		{
			float sz = 1.0f / size;
			const std::vector<Vertex> vertices
			{
				{ -sz, -sz },
				{ -sz,  sz },
				{  sz,  sz },
				{  sz, -sz },
			};

			AddStaticBind(MakeScope<VertexBuffer>(vertices));
			auto vertexShader = MakeScope<VertexShader>("VertexShader-v.cso");
			ID3DBlob* vbytecode = vertexShader->GetBytecode();
			AddStaticBind(std::move(vertexShader));

			const std::vector<uint16_t> indices
			{
				0, 1, 2, 3, 0, 2
			};
			AddStaticIndexBuffer(MakeScope<IndexBuffer>(indices));

			AddStaticBind(MakeScope<PixelShader>("PixelShader-p.cso"));

			AddStaticBind(MakeScope<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutVec
			{
				{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			AddStaticBind(MakeScope<InputLayout>(inputLayoutVec, vbytecode));
		}
		else
		{
			SetIndexFromStatic();
		}

		AddBind(MakeScope<TransformConstantBuffer>(renderer, *this));
	}

	void Rectangle::Update()
	{
	}

	void Rectangle::AddStaticBind(Scope<Bindable> bind)
	{
		RAY_PROFILE_FUNCTION();
		RAY_ASSERT(typeid(*bind) != typeid(IndexBuffer), "Trying to add an IndexBuffer with Drawable::AddBind, use Drawable::AddIndexBuffer.");
		s_StaticBinds.push_back(std::move(bind));
	}

	void Rectangle::AddStaticIndexBuffer(Scope<IndexBuffer> bind)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(m_pIndexBuffer == nullptr, "IndexBuffer already bound.");
		m_pIndexBuffer = bind.get();
		s_StaticBinds.push_back(std::move(bind));
	}

	void Rectangle::SetIndexFromStatic()
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