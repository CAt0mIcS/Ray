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

#include "../RRendererAPI.h"


namespace At0::Ray
{
	Triangle::Triangle(Entity entity)
		: Drawable(entity)
	{
		constexpr float side = 1.0f / 2.0f;
		// Vertices
		// RAY_TODO: Different vertex layout depending on opengl or directx
		Ref<VertexBuffer> pVertexBuffer;
		Ref<VertexShader> pVShader;
		Ref<PixelShader> pPShader;
		if (RendererAPI::GetAPI() == RendererAPI::D3D11)
		{
			pVertexBuffer = VertexBuffer::Create(
				{
					{  0.0f,  0.5f, 1.0f },
					{  0.5f, -0.5f, 1.0f },
					{ -0.5f, -0.5f, 1.0f },
				}
			);

			// Default Vertex and Pixel Shader for quads
			pVShader = VertexShader::CreateFromCompiled("TriangleVertexShader.cso");
			pPShader = PixelShader::CreateFromCompiled("TrianglePixelShader.cso");
		}
		else if (RendererAPI::GetAPI() == RendererAPI::OpenGL)
		{
			pVertexBuffer = VertexBuffer::Create(
				{
					{ -0.5f, -0.5f, 1.0f },
					{  0.0f,  0.5f, 1.0f },
					{  0.5f, -0.5f, 1.0f }
				}
			);

			// Default Vertex and Pixel Shader for quads
			pVShader = VertexShader::CreateFromSource("TriangleVertexShader.glsl");
			pPShader = PixelShader::CreateFromSource("TrianglePixelShader.glsl");
		}

		Ref<IndexBuffer> pIdxBuffer = IndexBuffer::Create(
			{
				0, 1, 2
			}
		);

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


