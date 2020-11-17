#include "rlpch.h"
#include "OpenGLWindow.h"

#include "Reyal/Events/ApplicationEvent.h"
#include "Reyal/Events/MouseEvent.h"
#include "Reyal/Events/KeyboardEvent.h"

#include <RlRender/Renderer3D.h>

#include <GLFW/glfw3.h>


namespace At0::Reyal
{
	OpenGLWindow::OpenGLWindow(const std::string_view name, Widget* parent, bool isMainWindow)
		: Window(name, parent, isMainWindow)
	{
	}
	
	OpenGLWindow::~OpenGLWindow()
	{
		if (m_hWnd)
		{
			Close();
		}
	}

	std::string OpenGLWindow::GetTitle() const
	{
		return std::string();
	}
	
	void OpenGLWindow::SetTitle(const std::string_view title)
	{
		glfwSetWindowTitle((GLFWwindow*)m_hWnd, title.data());
	}

	void OpenGLWindow::Show() const
	{
		glfwShowWindow((GLFWwindow*)m_hWnd);
	}

	void OpenGLWindow::Hide() const
	{
		glfwHideWindow((GLFWwindow*)m_hWnd);
	}

	void OpenGLWindow::Maximize() const
	{
		glfwMaximizeWindow((GLFWwindow*)m_hWnd);
	}

	void OpenGLWindow::Minimize() const
	{
		glfwIconifyWindow((GLFWwindow*)m_hWnd);
	}

	void OpenGLWindow::Close() const
	{
		glfwDestroyWindow((GLFWwindow*)m_hWnd);
	}

	bool OpenGLWindow::IsOpen() const
	{
		return m_hWnd ? true : false;
	}
	
	bool OpenGLWindow::ShouldClose()
	{
		return glfwWindowShouldClose((GLFWwindow*)m_hWnd);
	}
	
	void OpenGLWindow::SetUpEventCallbacks()
	{

	}
}


