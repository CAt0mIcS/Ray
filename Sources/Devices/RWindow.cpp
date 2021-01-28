#include "Rpch.h"
#include "RWindow.h"

#include "Input/RMouse.h"
#include "Input/RKeyboard.h"
#include "Utils/RAssert.h"
#include "Utils/RLogger.h"

#include <GLFW/glfw3.h>


namespace At0::Ray
{
	static void GLFWErrorCallback(int error, const char* description)
	{
		RAY_ASSERT(false, "[GLFWWindow] GLFW Error ({0}): {1}", error, description);
		Log::Error("[OpenGLRenderer3D] GLFW Error ({0}): {1}", error, description);
		Log::Flush();
	}

	Window* Window::s_Instance = nullptr;
	Window& Window::Create(uint32_t width, uint32_t height, std::string_view title)
	{
		// No ScopePtr because of private constructor
		s_Instance = new Window(width, height, title);
		return *s_Instance;
	}

	Window& Window::Get()
	{
		RAY_MEXPECTS(s_Instance, "[Window::Get] Cannot get window that has not been created yet.");
		return *s_Instance;
	}

	void Window::Close()
	{
		glfwDestroyWindow(m_hWnd);
		m_hWnd = nullptr;
	}

	bool Window::CursorEnabled() const
	{
		return glfwGetInputMode(m_hWnd, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
	}

	void Window::EnableCursor() const
	{
		glfwSetInputMode(m_hWnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Window::DisableCursor() const
	{
		glfwSetInputMode(m_hWnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	Window::Window(uint32_t width, uint32_t height, std::string_view title)
	{
		RAY_MEXPECTS(glfwInit(), "[Window] Failed to initialize GLFW.");
		Log::Info("[Window] GLFW successfully initialized.");
		glfwSetErrorCallback(GLFWErrorCallback);

		// Remove default opengl api that comes with glfw
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		// Create the window
		m_hWnd = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
		RAY_MEXPECTS(m_hWnd, "[Window] Failed to create window.");

		glfwSetWindowUserPointer(m_hWnd, this);

		SetEventCallbacks();
	}

	Window::~Window()
	{
		if (m_hWnd)
			Close();
		glfwTerminate();

		delete s_Instance;
	}

	void Window::SetEventCallbacks()
	{
		glfwSetCursorPosCallback(m_hWnd, [](GLFWwindow* window, double xPos, double yPos)
			{
				Mouse::SetPos({ (float)xPos, (float)yPos });

				Float2 rawDelta{ 0.0f, 0.0f };
				if (!Window::Get().CursorEnabled())
					rawDelta = Float2{ (float)xPos, (float)yPos } - Window::Get().m_CachedRawDeltaMousePos;

				Float2 mousePos{ (float)xPos, (float)yPos };
				Window::Get().m_CachedRawDeltaMousePos = mousePos;
			}
		);

		glfwSetMouseButtonCallback(m_hWnd, [](GLFWwindow* window, int button, int action, int mods)
			{
				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButton btn = (MouseButton)(button + 1);

					switch (btn)
					{
					case MouseButton::Left:		Mouse::SetLeftPressed(true); break;
					case MouseButton::Right:	Mouse::SetRightPressed(true); break;
					case MouseButton::Middle:	Mouse::SetMiddlePressed(true); break;
					}
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButton btn = (MouseButton)(button + 1);

					switch (btn)
					{
					case MouseButton::Left:		Mouse::SetLeftPressed(false); break;
					case MouseButton::Right:	Mouse::SetRightPressed(false); break;
					case MouseButton::Middle:	Mouse::SetMiddlePressed(false); break;
					}
					break;
				}
				}
			}
		);

		glfwSetKeyCallback(m_hWnd, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				switch (action)
				{
				case GLFW_PRESS:
				{
					Keyboard::SetKeyState((Key)key, true);
					break;
				}

				case GLFW_RELEASE:
				{
					Keyboard::SetKeyState((Key)key, false);
					break;
				}
				case GLFW_REPEAT:
				{

					break;
				}
				}
			}
		);

		glfwSetCharCallback(m_hWnd, [](GLFWwindow* window, unsigned int keycode)
			{

			}
		);

		glfwSetScrollCallback(m_hWnd, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				if (yOffset > 0)
				{
					// Mouse wheel up event
				}
				else if (yOffset < 0)
				{
					// Mouse wheel down event
				}

				if (xOffset > 0)
				{
					// Mouse wheel right event
				}
				else if (xOffset < 0)
				{
					// Mouse wheel left event
				}
			}
		);

		glfwSetWindowSizeCallback(m_hWnd, [](GLFWwindow* window, int width, int height)
			{

			}
		);

		glfwSetWindowPosCallback(m_hWnd, [](GLFWwindow* window, int x, int y)
			{

			}
		);

		glfwSetWindowCloseCallback(m_hWnd, [](GLFWwindow* window)
			{
				Window::Get().Close();
			}
		);

	}
}