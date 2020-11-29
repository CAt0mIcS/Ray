#include "rlrpch.h"
#include "Cube.h"

#include "RlRender/Bindable/VertexBuffer.h"
#include "RlRender/Bindable/VertexShader.h"
#include "RlRender/Bindable/PixelShader.h"
#include "RlRender/Bindable/Topology.h"
#include "RlRender/Bindable/InputLayout.h"
#include "RlRender/Bindable/IndexBuffer.h"


namespace At0::Reyal
{
	Cube::Cube(float size)
	{
		struct Vertex
		{
			struct
			{
				float x, y, z;
			} pos;
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
			0,1,  1,3,  3,2,  2,0,
			0,4,  1,5,  3,7,  2,6,
			4,5,  5,7,  7,6,  6,4
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
	}

	void Cube::Update()
	{

	}
}


