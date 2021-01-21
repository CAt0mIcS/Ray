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
		: DrawableBase(entity)
	{
		// RAY_TODO: Different vertex layout depending on opengl or directx
		// Assume that the other resources are not allocated if the VBuff is not
		if (!s_VertexBuffer)
		{
			RAY_MEXPECTS(!s_VertexShader && !s_PixelShader && !s_InputLayout && !s_IndexBuffer && !s_Topology, "[Quad] Only the VertexBuffer was nullptr.");

			constexpr float side = 1.0f / 2.0f;
			// Vertices
			s_IndexBuffer = IndexBuffer::Create(
				{
					0, 1, 2
				}
			);

			s_VertexBuffer = VertexBuffer::Create(
				{
					{  0.0f,  0.5f, 0.5f },
					{  0.5f, -0.5f, 0.5f },
					{ -0.5f, -0.5f, 0.5f },
				}
			);

			// Default Vertex and Pixel Shader for triangles
			s_VertexShader = VertexShader::CreateFromSource("TriangleVertexShader.hlsl");
			s_PixelShader = PixelShader::CreateFromSource("TrianglePixelShader.hlsl");

			// Matching input layout
			s_InputLayout = InputLayout::Create(
				{ { "Position", ShaderDataType::Float3 } },
				s_VertexShader
			);

			// topology
			s_Topology = Topology::Create(Topology::TriangleList);
		}
	}
}


