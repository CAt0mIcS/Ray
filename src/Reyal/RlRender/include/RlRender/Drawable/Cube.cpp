#include "rlrpch.h"
#include "Cube.h"

#include "RlRender/Bindable/VertexBuffer.h"
#include "RlRender/Bindable/VertexShader.h"
#include "RlRender/Bindable/PixelShader.h"
#include "RlRender/Bindable/Topology.h"
#include "RlRender/Bindable/InputLayout.h"
#include "RlRender/Bindable/IndexBuffer.h"
#include "RlRender/Bindable/ConstantBuffers.h"


namespace At0::Reyal
{
	Cube::Cube(const Renderer3D& renderer, float size, const float colors[6][3])
	{
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

		AddBind(MakeScope<VertexBuffer>(vertices));
#ifdef NDEBUG
		auto vertexShader = MakeScope<VertexShader>("../../bin/Release/VertexShader(Cube)-v.cso");
#else
		auto vertexShader = MakeScope<VertexShader>("../../bin/Debug/VertexShader(Cube)-v.cso");
#endif
		ID3DBlob* vbytecode = vertexShader->GetBytecode();
		AddBind(std::move(vertexShader));

		const std::vector<uint16_t> indices
		{
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4
		};
		AddIndexBuffer(MakeScope<IndexBuffer>(indices));

#ifdef NDEBUG
		AddBind(MakeScope<PixelShader>("../../bin/Release/PixelShader(Cube)-p.cso"));
#else
		AddBind(MakeScope<PixelShader>("../../bin/Debug/PixelShader(Cube)-p.cso"));
#endif
		
		AddBind(MakeScope<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutVec
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		AddBind(MakeScope<InputLayout>(inputLayoutVec, vbytecode));

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

		AddBind(MakeScope<PixelConstantBuffer<PixelConstantColorBuffer>>(pccb));

		AddBind(MakeScope<TransformConstantBuffer>(renderer, *this));
	}

	void Cube::Update()
	{

	}
}


