#include "Rpch.h"
#include "RWindow.h"

#include "Graphics/RGraphics.h"
#include "Scene/RScene.h"
#include "Devices/RMouse.h"
#include "Devices/RKeyboard.h"
#include "RayBase/RAssert.h"
#include "RayBase/RLogger.h"
#include "RayBase/RException.h"

#include "Events/REventListener.h"

#include "UI/RButton.h"


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
		RAY_MEXPECTS(s_Instance, "[Window::Get] Cannot get window that has not been created yet");
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

	bool Window::Minimized() const { return glfwGetWindowAttrib(m_hWnd, GLFW_ICONIFIED) != 0; }

	bool Window::IsOpen() const { return glfwGetWindowAttrib(m_hWnd, GLFW_VISIBLE) != 0; }

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
		ThrowRenderError(glfwCreateWindowSurface(instance, m_hWnd, allocator, surface),
			"[Window] GLFW failed to create the window surface");
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

	void Window::SetTitle(std::string_view newTitle) const
	{
		glfwSetWindowTitle(m_hWnd, newTitle.data());
	}

	Window::Window(uint32_t width, uint32_t height, std::string_view title)
	{
		int success = glfwInit();
		RAY_MEXPECTS(success, "[Window] Failed to initialize GLFW");
		Log::Info("[Window] GLFW successfully initialized");
		glfwSetErrorCallback(GLFWErrorCallback);

		// Remove default opengl api that comes with glfw
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		// Create the window
		m_hWnd = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
		RAY_MEXPECTS(m_hWnd, "[Window] Failed to create window");
		Log::Info("[Window] Created successfully ({0})", m_hWnd);

		glfwSetWindowUserPointer(m_hWnd, this);

		SetEventCallbacks();
	}

	Window::~Window() { glfwTerminate(); }

	void Window::SetEventCallbacks()
	{
		glfwSetCursorPosCallback(m_hWnd, [](GLFWwindow* window, double xPos, double yPos) {
			int32_t dx = (int)Window::Get().m_PrevousMousePos.x - xPos;
			int32_t dy = (int)Window::Get().m_PrevousMousePos.y - yPos;

			Float2 newPos = { (float)xPos, (float)yPos };
			Window::Get().m_PrevousMousePos = newPos;

			Mouse::SetPos(newPos);

			MouseMovedEvent e(newPos, { dx, dy });
			for (auto* listener : Window::Get().EventDispatcher<MouseMovedEvent>::Get())
			{
				listener->OnEvent(e);
				if (e.Handled)
					break;
			}

			Window::Get().GenerateHoverEvents();
		});

		glfwSetMouseButtonCallback(m_hWnd, [](GLFWwindow* window, int button, int action,
											   int mods) {
			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButton btn = (MouseButton)button;

				switch (btn)
				{
				case MouseButton::Left: Mouse::SetLeftPressed(true); break;
				case MouseButton::Right: Mouse::SetRightPressed(true); break;
				case MouseButton::Middle: Mouse::SetMiddlePressed(true); break;
				}

				MouseButtonPressedEvent e(btn, Window::Get().GetClickedWidget());
				for (auto* listener : Window::Get().EventDispatcher<MouseButtonPressedEvent>::Get())
				{
					listener->OnEvent(e);
					if (e.Handled)
						break;
				}

				break;
			}
			case GLFW_RELEASE:
			{
				MouseButton btn = (MouseButton)button;

				switch (btn)
				{
				case MouseButton::Left: Mouse::SetLeftPressed(false); break;
				case MouseButton::Right: Mouse::SetRightPressed(false); break;
				case MouseButton::Middle: Mouse::SetMiddlePressed(false); break;
				}

				MouseButtonReleasedEvent e(btn, Window::Get().GetReleasedWidget());
				for (auto* listener :
					Window::Get().EventDispatcher<MouseButtonReleasedEvent>::Get())
				{
					listener->OnEvent(e);
					if (e.Handled)
						break;
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

					KeyPressedEvent e(k);
					for (auto* listener : Window::Get().EventDispatcher<KeyPressedEvent>::Get())
					{
						listener->OnEvent(e);
						if (e.Handled)
							break;
					}

					break;
				}

				case GLFW_RELEASE:
				{
					Key k = (Key)key;

					Keyboard::SetKeyState(k, false);

					KeyReleasedEvent e(k);
					for (auto* listener : Window::Get().EventDispatcher<KeyReleasedEvent>::Get())
					{
						listener->OnEvent(e);
						if (e.Handled)
							break;
					}

					break;
				}
				case GLFW_REPEAT:
				{
					Key k = (Key)key;
					uint32_t repeatCount = 1;

					KeyRepeatedEvent e(k, repeatCount);
					for (auto* listener : Window::Get().EventDispatcher<KeyRepeatedEvent>::Get())
					{
						listener->OnEvent(e);
						if (e.Handled)
							break;
					}

					break;
				}
				}
			});

		glfwSetCharCallback(m_hWnd, [](GLFWwindow* window, unsigned int keycode) {
			CharEvent e(keycode);
			for (auto* listener : Window::Get().EventDispatcher<CharEvent>::Get())
			{
				listener->OnEvent(e);
				if (e.Handled)
					break;
			}
		});

		glfwSetScrollCallback(m_hWnd, [](GLFWwindow* window, double xOffset, double yOffset) {
			if (yOffset > 0)
			{
				ScrollUpEvent e({ xOffset, yOffset });
				for (auto* listener : Window::Get().EventDispatcher<ScrollUpEvent>::Get())
				{
					listener->OnEvent(e);
					if (e.Handled)
						break;
				}
			}
			else if (yOffset < 0)
			{
				ScrollDownEvent e({ xOffset, yOffset });
				for (auto* listener : Window::Get().EventDispatcher<ScrollDownEvent>::Get())
				{
					listener->OnEvent(e);
					if (e.Handled)
						break;
				}
			}

			if (xOffset > 0)
			{
				ScrollRightEvent e({ xOffset, yOffset });
				for (auto* listener : Window::Get().EventDispatcher<ScrollRightEvent>::Get())
				{
					listener->OnEvent(e);
					if (e.Handled)
						break;
				}
			}
			else if (xOffset < 0)
			{
				ScrollLeftEvent e({ xOffset, yOffset });
				for (auto* listener : Window::Get().EventDispatcher<ScrollLeftEvent>::Get())
				{
					listener->OnEvent(e);
					if (e.Handled)
						break;
				}
			}
		});

		glfwSetFramebufferSizeCallback(m_hWnd, [](GLFWwindow* window, int width, int height) {
			FramebufferResizedEvent e({ width, height });
			for (auto* listener : Window::Get().EventDispatcher<FramebufferResizedEvent>::Get())
			{
				listener->OnEvent(e);
				if (e.Handled)
					break;
			}
		});

		glfwSetWindowSizeCallback(m_hWnd, [](GLFWwindow* window, int width, int height) {
			WindowResizedEvent e({ width, height });
			for (auto* listener : Window::Get().EventDispatcher<WindowResizedEvent>::Get())
			{
				listener->OnEvent(e);
				if (e.Handled)
					break;
			}
		});

		glfwSetWindowPosCallback(m_hWnd, [](GLFWwindow* window, int x, int y) {
			WindowMovedEvent e({ x, y });
			for (auto* listener : Window::Get().EventDispatcher<WindowMovedEvent>::Get())
			{
				listener->OnEvent(e);
				if (e.Handled)
					break;
			}
		});

		glfwSetWindowCloseCallback(m_hWnd, [](GLFWwindow* window) {
			WindowClosedEvent e;
			for (auto* listener : Window::Get().EventDispatcher<WindowClosedEvent>::Get())
			{
				listener->OnEvent(e);
				if (e.Handled)
					break;
			}

			Window::Get().Close();
		});
	}

	void Window::GenerateHoverEvents()
	{
		auto btnView = Scene::Get().EntityView<Button>();
		for (Entity btnEntity : btnView)
		{
			Button& btn = btnEntity.Get<Button>();
			if (!Mouse::IsOnWidget(btn) && &btn == m_HoverWidget)
			{
				HoverLeaveEvent e(m_HoverWidget);
				for (auto* listener : EventDispatcher<HoverLeaveEvent>::Get())
					listener->OnEvent(e);

				m_HoverWidget = nullptr;
			}
			else if (Mouse::IsOnWidget(btn) && &btn != m_HoverWidget)
			{
				m_HoverWidget = &btn;

				HoverEnterEvent e(m_HoverWidget);
				for (auto* listener : EventDispatcher<HoverEnterEvent>::Get())
					listener->OnEvent(e);
			}
		}
	}

	Widget* Window::GetClickedWidget()
	{
		auto btnView = Scene::Get().EntityView<Button>();
		for (Entity btnEntity : btnView)
		{
			Button& btn = btnEntity.Get<Button>();
			if (Mouse::IsOnWidget(btn))
			{
				m_ClickedWidget = &btn;
				return m_ClickedWidget;
			}
		}
		return nullptr;
	}

	Widget* Window::GetReleasedWidget()
	{
		auto btnView = Scene::Get().EntityView<Button>();
		for (Entity btnEntity : btnView)
		{
			Button& btn = btnEntity.Get<Button>();
			if (m_ClickedWidget == &btn)
			{
				m_ClickedWidget = nullptr;
				return &btn;
			}
		}
		return nullptr;
	}
}  // namespace At0::Ray
