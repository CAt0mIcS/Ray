#include "RayRender/RRpch.h"

#include "ROpenGLRenderer3D.h"

#include "../../RVertexBuffer.h"
#include "../../RIndexBuffer.h"
#include "../../RVertexShader.h"
#include "../../RPixelShader.h"
#include "../../RTopology.h"
#include "../../RConstantBuffers.h"

#include "ROpenGLTopology.h"
#include "ROpenGLPixelShader.h"
#include "ROpenGLVertexShader.h"

#include "../../Core/RCamera.h"

#include <RayDebug/RAssert.h>
#include <RayUtil/RException.h>
#include <RayDebug/RInstrumentor.h>
#include <RayUtil/RComponents.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace At0::Ray
{
	bool OpenGLRenderer3D::s_GladInitialized = false;

	OpenGLRenderer3D::OpenGLRenderer3D(GLFWwindow* hWnd, EventDispatcher<WindowResizeEvent>& resizeDispatcher)
		: m_hWnd(hWnd),
		EventListener<WindowResizeEvent>(resizeDispatcher)
	{
		RAY_PROFILE_FUNCTION();

		// Context initialization
		glfwMakeContextCurrent(m_hWnd);

		// Initialize Glad
		if (!s_GladInitialized)
		{
			s_GladInitialized = true;

			int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			RAY_ASSERT(status, "Failed to initialize Glad!");

			Log::Info("[OpenGLWindow] Successfully initialized Glad");
			Log::Info("[OpenGLWindow] OpenGL Info:"
				"\n\tVendor:\t{0}\n\tRenderer: {1}\n\tVersion:{2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
		}

		int width, height;
		glfwGetWindowSize(m_hWnd, &width, &height);
		glViewport(0, 0, width, height);

		glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
	}

	OpenGLRenderer3D::~OpenGLRenderer3D()
	{
		RAY_PROFILE_FUNCTION();
	}

	void OpenGLRenderer3D::ClearBuffer(float red, float green, float blue)
	{
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderer3D::EndDraw()
	{
		glfwSwapBuffers(m_hWnd);
	}

	void OpenGLRenderer3D::Draw(const Camera& camera, Scene& scene)
	{
		RAY_PROFILE_FUNCTION();

		uint32_t buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);

		for (Ref<Drawable>& d : scene)
		{
			d->GetVertexBuffer()->Bind();

			IndexBuffer* idxBuff = d->GetIndexBuffer();
			idxBuff->Bind();
			OpenGLShader* pShader = (OpenGLShader*)d->GetShader();
			pShader->Bind();

			Matrix mvp = d->GetComponent<TransformComponent>().ToMatrix() *
				camera.GetMatrix() * camera.GetProjection();

			int blockIndex = glGetUniformBlockIndex(pShader->GetProgram(), "type_Projection");
			glUniformBlockBinding(pShader->GetProgram(), blockIndex, 1);
			glBindBufferBase(GL_UNIFORM_BUFFER, 1, buffer);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(mvp), &mvp, GL_DYNAMIC_DRAW);

			// RAY_TEMPORARY
			//d->GetVertexConstantBuffer()->Update(
			//	d->GetComponent<TransformComponent>().ToMatrix() *
			//	camera.GetMatrix() * camera.GetProjection()
			//);
			//d->GetVertexConstantBuffer()->Bind();

			OpenGLTopology* pTopology = (OpenGLTopology*)d->GetTopology();
			glDrawElements(pTopology->Get(), idxBuff->GetIndicesCount(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void OpenGLRenderer3D::OnEvent(Widget& receiver, WindowResizeEvent& e)
	{
		glViewport(0, 0, (int)e.GetSize().x, (int)e.GetSize().y);
	}
}



