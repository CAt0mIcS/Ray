#include "RayRender/RRpch.h"

#include "ROpenGLRenderer3D.h"

#include "../../RVertexBuffer.h"
#include "../../RIndexBuffer.h"
#include "../../RVertexShader.h"
#include "../../RPixelShader.h"
#include "../../RInputLayout.h"
#include "../../RTopology.h"

#include <RayDebug/RAssert.h>
#include <RayUtil/RException.h>
#include <RayDebug/RInstrumentor.h>
#include <RayUtil/RComponents.h>


namespace At0::Ray
{
	OpenGLRenderer3D::OpenGLRenderer3D(GLFWwindow* hWnd)
		: m_hWnd(hWnd)
	{
		RAY_PROFILE_FUNCTION();
	}

	OpenGLRenderer3D::~OpenGLRenderer3D()
	{
		RAY_PROFILE_FUNCTION();
	}

	void OpenGLRenderer3D::ClearBuffer(float red, float green, float blue)
	{

	}

	void OpenGLRenderer3D::EndDraw()
	{

	}

	void OpenGLRenderer3D::Draw(Scene& scene)
	{
		RAY_PROFILE_FUNCTION();

		for (Drawable& d : scene)
		{
			d.GetComponent<VertexBufferComponent>().Buffer->Bind();
			d.GetComponent<IndexBufferComponent>().Buffer->Bind();
			d.GetComponent<VertexShaderComponent>().Shader->Bind();
			d.GetComponent<PixelShaderComponent>().Shader->Bind();
			d.GetComponent<InputLayoutComponent>().Layout->Bind();
			d.GetComponent<TopologyComponent>().PrimitiveTopology->Bind();


		}
	}
}



