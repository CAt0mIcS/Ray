#include "../RRpch.h"
#include "RQuad.h"

#include <RayUtil/RComponents.h>

#include "../Core/RScene.h"
#include "../RVertexShader.h"
#include "../RPixelShader.h"
#include "../RInputLayout.h"
#include "../RIndexBuffer.h"


namespace At0::Ray
{
	Quad::Quad(Entity entity)
		: Drawable(entity)
	{

		struct Vertex
		{
			struct
			{
				float x, y, z;
			} pos;
		};

		constexpr float side = 1.0f / 2.0f;
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

		// Vertices
		Ref<IndexBuffer> pIdxBuffer = IndexBuffer::Create(
			{
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			}
		);

		// Default Vertex and Pixel Shader for quads
		Ref<VertexShader> pVShader = VertexShader::CreateFromCompiled("QuadVertexShader.cso");
		Ref<PixelShader> pPShader = PixelShader::CreateFromCompiled("QuadPixelShader.cso");

		// Matching input layout
		Ref<InputLayout> pInputLayout = InputLayout::Create(
			{ { "POSITION", ShaderDataType::Float3 } },
			pVShader
		);

		pIdxBuffer->Bind();
		pVShader->Bind();
		pPShader->Bind();
		pInputLayout->Bind();

		AddComponent<IndexBufferComponent>(pIdxBuffer);
		AddComponent<VertexShaderComponent>(pVShader);
		AddComponent<PixelShaderComponent>(pPShader);
		AddComponent<InputLayoutComponent>(pInputLayout);
	}
}


