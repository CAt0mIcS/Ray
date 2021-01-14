#include "../RRpch.h"
#include "RQuad.h"

#include <RayUtil/RComponents.h>

#include "../Core/RScene.h"
#include "../RVertexShader.h"
#include "../RPixelShader.h"


namespace At0::Ray
{
	Quad::Quad(Entity entity)
		: Drawable(entity)
	{
		// Default Vertex and Pixel Shader for quads
		Ref<VertexShader> pVShader = VertexShader::CreateFromCompiled("QuadVertexShader.cso");
		Ref<PixelShader> pPShader = PixelShader::CreateFromCompiled("QuadPixelShader.cso");

		AddComponent<VertexShaderComponent>(pVShader);
		AddComponent<PixelShaderComponent>(pPShader);
	}
}


