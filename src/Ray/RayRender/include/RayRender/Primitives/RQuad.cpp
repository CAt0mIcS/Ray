#include "../RRpch.h"
#include "RQuad.h"

#include <RayUtil/RComponents.h>

#include "../Core/RVertex.h"
#include "../Core/RScene.h"

#include "../RVertexShader.h"
#include "../RPixelShader.h"
#include "../RInputLayout.h"
#include "../RIndexBuffer.h"
#include "../RVertexBuffer.h"
#include "../RTopology.h"
#include "../RConstantBuffers.h"
#include "../RRendererAPI.h"


namespace At0::Ray
{
	Quad::Quad(Entity entity)
		: DrawableBase(entity)
	{
		// Assume that the other resources are not allocated if the VBuff is not
		if (!s_VertexBuffer)
		{
			RAY_MEXPECTS(!s_VertexShader && !s_PixelShader && !s_InputLayout && !s_IndexBuffer && !s_Topology, "[Quad] Only the VertexBuffer was nullptr.");

			constexpr float side = 1.0f / 2.0f;
			VertexLayout layout;
			layout.Append<VertexLayout::Position3D>();
			VertexData data(layout, 8);
			data[0].Set<VertexLayout::Position3D>({ -side, -side, -side });
			data[1].Set<VertexLayout::Position3D>({ side, -side, -side });
			data[2].Set<VertexLayout::Position3D>({ -side,  side, -side });
			data[3].Set<VertexLayout::Position3D>({ side,  side, -side });
			data[4].Set<VertexLayout::Position3D>({ -side, -side,  side });
			data[5].Set<VertexLayout::Position3D>({ side, -side,  side });
			data[6].Set<VertexLayout::Position3D>({ -side,  side,  side });
			data[7].Set<VertexLayout::Position3D>({ side,  side,  side });

			// Vertices (automatically converted to right handed coordinate system when using opengl
			//s_VertexBuffer = VertexBuffer::Create(
			//	{
			//		{ -side, -side, -side },
			//		{  side, -side, -side },
			//		{ -side,  side, -side },
			//		{  side,  side, -side },
			//		{ -side, -side,  side },
			//		{  side, -side,  side },
			//		{ -side,  side,  side },
			//		{  side,  side,  side },
			//	}
			//);

			s_VertexBuffer = VertexBuffer::Create(data);

			s_IndexBuffer = IndexBuffer::Create(
				{
					0, 2, 1,  2, 3, 1,
					1, 3, 5,  3, 7, 5,
					2, 6, 3,  3, 6, 7,
					4, 5, 7,  4, 7, 6,
					0, 4, 2,  2, 4, 6,
					0, 1, 4,  1, 5, 4
				}
			);

			// Default Vertex and Pixel Shader for quads
			s_VertexShader = VertexShader::CreateFromSource("QuadVertexShader.hlsl");
			s_PixelShader = PixelShader::CreateFromSource("QuadPixelShader.hlsl");


			// Matching input layout
			s_InputLayout = InputLayout::Create(
				{ { "Position", ShaderDataType::Float3 } },
				s_VertexShader
			);

			// topology
			s_Topology = Topology::Create(Topology::TriangleList);
		}

		// RAY_TODO: Update mvp with camera
		Matrix quadTransform = GetComponent<TransformComponent>().ToMatrix();
		Matrix mvp = quadTransform;
		m_TransformConstantBuffer = VertexConstantBuffer::Create<Matrix>(mvp);
	}
}


