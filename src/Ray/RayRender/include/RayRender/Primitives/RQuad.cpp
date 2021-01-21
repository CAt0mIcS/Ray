#include "../RRpch.h"
#include "RQuad.h"

#include <RayUtil/RComponents.h>

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
			// Vertices
			if (RendererAPI::GetAPI() == RendererAPI::D3D11)
			{
				s_VertexBuffer = VertexBuffer::Create(
					{
						{ -side, -side, -side },
						{  side, -side, -side },
						{ -side,  side, -side },
						{  side,  side, -side },
						{ -side, -side,  side },
						{  side, -side,  side },
						{ -side,  side,  side },
						{  side,  side,  side },
					}
				);

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
			}
			else if (RendererAPI::GetAPI() == RendererAPI::OpenGL)
			{
				s_VertexBuffer = VertexBuffer::Create(
					{
						{ -side, -side,  side },
						{  side, -side,  side },
						{  side,  side,  side },
						{ -side,  side,  side },
						{ -side, -side, -side },
						{  side, -side, -side },
						{  side,  side, -side },
						{ -side,  side, -side }
					}
				);

				s_IndexBuffer = IndexBuffer::Create(
					{
						0, 1, 2,  2, 3, 0,
						1, 5, 6,  6, 2, 1,
						7, 6, 5,  5, 4, 7,
						4, 0, 3,  3, 7, 4,
						4, 5, 1,  1, 0, 4,
						3, 2, 6,  6, 7, 3
					}
				);
			}

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


