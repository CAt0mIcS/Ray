#include "Rpch.h"
#include "RWindow.h"

#include "Graphics/RGraphics.h"
#include "Devices/RMouse.h"
#include "Devices/RKeyboard.h"
#include "Utils/RAssert.h"
#include "Utils/RLogger.h"
#include "Utils/RException.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


namespace At0::Ray
{
	static void GLFWErrorCallback(int error, const char* description)
	{
		RAY_ASSERT(false, "[GLFWWindow] GLFW Error ({0}): {1}", error, description);
		Log::Error("[GLFWWindow] GLFW Error ({0}): {1}", error, description);
		Log::Flush();
	}

	Scope<Window> Window::s_Instance = nullptr;

	Window& Window::Create(uint32_t width, uint32_t height, std::string_view title)
	{
		s_Instance = Scope<Window>(new Window(width, height, title));
		return *s_Instance;
	}

	Window& Window::Get()
	{
		RAY_MEXPECTS(s_Instance, "[Window::Get] Cannot get window that has not been created yet.");
		return *s_Instance;
	}

	void Window::Show() const { glfwShowWindow(m_hWnd); }

	void Window::Close() { glfwDestroyWindow(m_hWnd); }

	bool Window::CursorEnabled() const
	{
		return glfwGetInputMode(m_hWnd, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
	}

	void Window::EnableCursor() const { glfwSetInputMode(m_hWnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

	void Window::DisableCursor() const
	{
		glfwSetInputMode(m_hWnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	bool Window::Update()
	{
		glfwPollEvents();

		return !glfwWindowShouldClose(m_hWnd);
	}

	std::pair<const char**, uint32_t> Window::GetInstanceExtensions()
	{
		uint32_t count;
		const char** extensions = glfwGetRequiredInstanceExtensions(&count);
		return std::make_pair(extensions, count);
	}

	void Window::CreateSurface(
		VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) const
	{
		RAY_VK_THROW_FAILED(glfwCreateWindowSurface(instance, m_hWnd, allocator, surface),
			"[Window] GLFW failed to create the window surface.");
	}

	UInt2 Window::GetSize() const
	{
		UInt2 size;
		glfwGetWindowSize(m_hWnd, (int*)&size.x, (int*)&size.y);
		return size;
	}

	UInt2 Window::GetFramebufferSize() const
	{
		UInt2 size;
		glfwGetFramebufferSize(m_hWnd, (int*)&size.x, (int*)&size.y);
		return size;
	}

	void Window::WaitForEvents() const { glfwWaitEvents(); }

	Window::Window(uint32_t width, uint32_t height, std::string_view title)
	{
		int success = glfwInit();
		RAY_MEXPECTS(success, "[Window] Failed to initialize GLFW.");
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

	Window::~Window() { glfwTerminate(); }

	void Window::SetEventCallbacks()
	{
		glfwSetCursorPosCallback(m_hWnd, [](GLFWwindow* window, double xPos, double yPos) {
			Mouse::SetPos({ (float)xPos, (float)yPos });

			Float2 rawDelta{ 0.0f, 0.0f };
			if (!Window::Get().CursorEnabled())
				rawDelta =
					Float2{ (float)xPos, (float)yPos } - Window::Get().m_CachedRawDeltaMousePos;

			Float2 mousePos{ (float)xPos, (float)yPos };
			Window::Get().m_CachedRawDeltaMousePos = mousePos;
		});

		glfwSetMouseButtonCallback(
			m_hWnd, [](GLFWwindow* window, int button, int action, int mods) {
				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButton btn = (MouseButton)(button + 1);

					switch (btn)
					{
					case MouseButton::Left: Mouse::SetLeftPressed(true); break;
					case MouseButton::Right: Mouse::SetRightPressed(true); break;
					case MouseButton::Middle: Mouse::SetMiddlePressed(true); break;
					}
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButton btn = (MouseButton)(button + 1);

					switch (btn)
					{
					case MouseButton::Left: Mouse::SetLeftPressed(false); break;
					case MouseButton::Right: Mouse::SetRightPressed(false); break;
					case MouseButton::Middle: Mouse::SetMiddlePressed(false); break;
					}
					break;
				}
				}
			});

		glfwSetKeyCallback(
			m_hWnd, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
				switch (action)
				{
				case GLFW_PRESS:
				{
					Key k = (Key)key;
					Keyboard::SetKeyState(k, true);
					break;
				}

				case GLFW_RELEASE:
				{
					Key k = (Key)key;
					Keyboard::SetKeyState(k, false);
					break;
				}
				case GLFW_REPEAT:
				{

					break;
				}
				}
			});

		glfwSetCharCallback(m_hWnd, [](GLFWwindow* window, unsigned int keycode) {
			Log::Debug("Character {0} written.", (unsigned char)keycode);
		});

		glfwSetScrollCallback(m_hWnd, [](GLFWwindow* window, double xOffset, double yOffset) {
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
		});

		glfwSetFramebufferSizeCallback(m_hWnd, [](GLFWwindow* window, int width, int height) {
			Graphics::Get().m_FramebufferResized = true;
		});

		glfwSetWindowSizeCallback(m_hWnd, [](GLFWwindow* window, int width, int height) {

		});

		glfwSetWindowPosCallback(m_hWnd, [](GLFWwindow* window, int x, int y) {

		});

		glfwSetWindowCloseCallback(m_hWnd, [](GLFWwindow* window) { Window::Get().Close(); });
	}
}  // namespace At0::Ray
