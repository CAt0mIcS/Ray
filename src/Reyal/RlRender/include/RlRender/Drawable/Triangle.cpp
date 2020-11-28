#include "rlrpch.h"
#include "Triangle.h"

#include "Bindable/VertexBuffer.h"
#include "Bindable/IndexBuffer.h"
#include "Bindable/InputLayout.h"
#include "Bindable/VertexShader.h"
#include "Bindable/PixelShader.h"
#include "Bindable/Topology.h"


namespace At0::Reyal
{
	Triangle::Triangle()
	{
        struct Vertex
        {
            struct
            {
                float x, y;
            } pos;
        };

        const std::vector<Vertex> vertices
        {
            {  0.0f,  0.5f },
            {  0.5f, -0.5f },
            { -0.5f, -0.5f }
        };

        AddBind(MakeScope<VertexBuffer>(vertices));
#ifdef NDEBUG
        auto vertexShader = MakeScope<VertexShader>("../../bin/Release/VertexShader-v.cso");
#else
        auto vertexShader = MakeScope<VertexShader>("../../bin/Debug/VertexShader-v.cso");
#endif
        ID3DBlob* vbytecode = vertexShader->GetBytecode();
        AddBind(std::move(vertexShader));

        const std::vector<uint16_t> indices
        {
            1, 2, 3
        };
        AddIndexBuffer(MakeScope<IndexBuffer>(indices));

#ifdef NDEBUG
        AddBind(MakeScope<PixelShader>("../../bin/Release/PixelShader-p.cso"));
#else
        AddBind(MakeScope<PixelShader>("../../bin/Debug/PixelShader-p.cso"));
#endif

        AddBind(MakeScope<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutVec
        {
            { "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        AddBind(MakeScope<InputLayout>(inputLayoutVec, vbytecode));
	}

	void Triangle::Update()
	{
	}
}


