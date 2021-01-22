#include "../RRpch.h"
#include "RTriangle.h"

#include <RayUtil/RComponents.h>

#include "../Core/RScene.h"
#include "../Core/RVertex.h"

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
		// Assume that the other resources are not allocated if the VBuff is not
		if (!s_VertexBuffer)
		{
			RAY_MEXPECTS(!s_VertexShader && !s_PixelShader && !s_InputLayout && !s_IndexBuffer && !s_Topology, "[Quad] Only the VertexBuffer was nullptr.");

			// Vertices
			VertexLayout layout;
			layout.Append<VertexLayout::Position3D>();

			VertexData data(layout, 3);
			data[0].Set<VertexLayout::Position3D>({ 0.0f,  0.5f, 0.5f });
			data[1].Set<VertexLayout::Position3D>({ 0.5f, -0.5f, 0.5f });
			data[2].Set<VertexLayout::Position3D>({ -0.5f, -0.5f, 0.5f });
			s_VertexBuffer = VertexBuffer::Create(data);


			s_IndexBuffer = IndexBuffer::Create(
				{
					0, 1, 2
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

		m_TransformConstantBuffer = VertexConstantBuffer::Create(GetComponent<TransformComponent>().ToMatrix());
	}
}


