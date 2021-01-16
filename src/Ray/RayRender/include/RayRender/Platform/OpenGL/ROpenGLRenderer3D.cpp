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

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace At0::Ray
{
	bool OpenGLRenderer3D::s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		RAY_PROFILE_FUNCTION();

		RAY_ASSERT(false, "[OpenGLRenderer3D] GLFW Error ({0}): {1}", error, description);
		Log::Flush();
	}


	OpenGLRenderer3D::OpenGLRenderer3D(GLFWwindow* hWnd)
		: m_hWnd(hWnd)
	{
		RAY_PROFILE_FUNCTION();

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			RAY_ASSERT(success, "Failed to initialize GLFW");
			Log::Info("[OpenGLWindow] Successfully initialized GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		// Context initialization
		glfwMakeContextCurrent(m_hWnd);

		// Initialize Glad
		if (!s_GLFWInitialized)
		{
			s_GLFWInitialized = true;

			int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			RAY_ASSERT(status, "Failed to initialize Glad!");

			Log::Info("[OpenGLWindow] Successfully initialized Glad");
			Log::Info("[OpenGLWindow] OpenGL Info:"
				"\n\tVendor:\t{0}\n\tRenderer: {1}\n\tVersion:{2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
		}
	}

	OpenGLRenderer3D::~OpenGLRenderer3D()
	{
		RAY_PROFILE_FUNCTION();
	}

	void OpenGLRenderer3D::ClearBuffer(float red, float green, float blue)
	{
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRenderer3D::EndDraw()
	{
		glfwSwapBuffers(m_hWnd);
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



