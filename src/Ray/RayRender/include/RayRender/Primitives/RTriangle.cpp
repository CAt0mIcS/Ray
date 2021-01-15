#include "../RRpch.h"
#include "RTriangle.h"

#include <RayUtil/RComponents.h>

#include "../Core/RScene.h"
#include "../RVertexShader.h"
#include "../RPixelShader.h"
#include "../RInputLayout.h"
#include "../RIndexBuffer.h"
#include "../RVertexBuffer.h"
#include "../RTopology.h"


namespace At0::Ray
{
	Triangle::Triangle(Entity entity)
		: Drawable(entity)
	{
		constexpr float side = 1.0f / 2.0f;
		// Vertices
		Ref<VertexBuffer> pVertexBuffer = VertexBuffer::Create(
			{
				{  0.0f,  0.5f },
				{  0.5f, -0.5f },
				{ -0.0f, -0.5f },
			}
		);

		Ref<IndexBuffer> pIdxBuffer = IndexBuffer::Create(
			{
				1, 2, 3
			}
		);
		m_IndexBufferCount = 3;

		// Default Vertex and Pixel Shader for quads
		Ref<VertexShader> pVShader = VertexShader::CreateFromCompiled("TriangleVertexShader.cso");
		Ref<PixelShader> pPShader = PixelShader::CreateFromCompiled("TrianglePixelShader.cso");

		// Matching input layout
		Ref<InputLayout> pInputLayout = InputLayout::Create(
			{ { "Position", ShaderDataType::Float3 } },
			pVShader
		);

		// topology
		Ref<Topology> pTopology = Topology::Create(Topology::TriangleList);

		// bind everything
		pVertexBuffer->Bind();
		pIdxBuffer->Bind();
		pVShader->Bind();
		pPShader->Bind();
		pInputLayout->Bind();
		pTopology->Bind();

		AddComponent<VertexBufferComponent>(pVertexBuffer);
		AddComponent<IndexBufferComponent>(pIdxBuffer);
		AddComponent<VertexShaderComponent>(pVShader);
		AddComponent<PixelShaderComponent>(pPShader);
		AddComponent<InputLayoutComponent>(pInputLayout);
		AddComponent<TopologyComponent>(pTopology);
	}
}


