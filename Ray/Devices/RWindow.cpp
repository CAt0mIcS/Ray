#include "RWindow.h"

#include "Graphics/Core/RRenderContext.h"
#include "Scene/RScene.h"
#include "Devices/RMouse.h"
#include "Devices/RKeyboard.h"

#include "Events/REventListener.h"

#include "Components/UI/RButton.h"


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

	bool Window::s_GlfwInitialized = false;

	Window::Window(const EngineRenderContext& engineContext, uint32_t width, uint32_t height,
		std::string_view title)
	{
		TryInitializeGlfw();

		// Create the window
		m_hWnd = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
		RAY_MEXPECTS(m_hWnd, "[Window] Failed to create window");
		Log::Info("[Window] Created successfully ({0})", m_hWnd);

		glfwSetWindowUserPointer(m_hWnd, this);
		SetEventCallbacks();

		m_RenderContext = MakeScope<RenderContext>(*this, engineContext);
	}

	void Window::Show() const
	{
		glfwShowWindow(m_hWnd);
	}

	void Window::Hide() const
	{
		glfwHideWindow(m_hWnd);
	}

	void Window::Close()
	{
		m_RenderContext.reset();
		glfwDestroyWindow(m_hWnd);
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

	bool Window::Minimized() const
	{
		return glfwGetWindowAttrib(m_hWnd, GLFW_ICONIFIED) != 0;
	}

	bool Window::IsOpen() const
	{
		return glfwGetWindowAttrib(m_hWnd, GLFW_VISIBLE) != 0;
	}

	bool Window::Update(Delta dt)
	{
		glfwPollEvents();
		bool shouldClose = glfwWindowShouldClose(m_hWnd);

		if (!shouldClose)
			m_RenderContext->graphics.Update(dt);

		return !shouldClose;
	}

	std::pair<const char**, uint32_t> Window::GetInstanceExtensions()
	{
		TryInitializeGlfw();

		uint32_t count;
		const char** extensions = glfwGetRequiredInstanceExtensions(&count);
		return std::make_pair(extensions, count);
	}

	void Window::CreateSurface(
		VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) const
	{
		ThrowVulkanError(glfwCreateWindowSurface(instance, m_hWnd, allocator, surface),
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

	void Window::WaitForEvents() const
	{
		glfwWaitEvents();
	}

	void Window::SetTitle(std::string_view newTitle) const
	{
		glfwSetWindowTitle(m_hWnd, newTitle.data());
	}

	Window::~Window()
	{
		// RAY_TODO: Never freeing glfw resources
		// glfwTerminate();
	}

	void Window::SetEventCallbacks()
	{
		glfwSetCursorPosCallback(m_hWnd,
			[](GLFWwindow* hWnd, double xPos, double yPos)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(hWnd);

				int32_t dx = (int)window->m_PrevousMousePos.x - xPos;
				int32_t dy = (int)window->m_PrevousMousePos.y - yPos;

				Float2 newPos = { (float)xPos, (float)yPos };
				window->m_PrevousMousePos = newPos;

				Mouse::SetPos(newPos);

				Log::Trace("[Window] MouseMoveEvent {x={0}, y={1}} (Listeners: {2})", newPos.x,
					newPos.y, window->EventDispatcher<MouseMovedEvent>::Get().size());

				MouseMovedEvent e(newPos, { dx, dy });
				for (auto* listener : window->EventDispatcher<MouseMovedEvent>::Get())
				{
					listener->OnEvent(e);
					if (e.Handled)
						break;
				}

				window->GenerateHoverEvents();
			});

		glfwSetMouseButtonCallback(m_hWnd,
			[](GLFWwindow* hWnd, int button, int action, int mods)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(hWnd);
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

					Log::Trace("[Window] MouseButtonPressedEvent ({0}) (Listeners: {1})",
						String::Construct(btn),
						window->EventDispatcher<MouseButtonPressedEvent>::Get().size());

					MouseButtonPressedEvent e(btn, window->GetClickedWidget());
					for (auto* listener : window->EventDispatcher<MouseButtonPressedEvent>::Get())
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

					Log::Trace("[Window] MouseButtonReleasedEvent ({0}) (Listeners: {1})",
						String::Construct(btn),
						window->EventDispatcher<MouseButtonReleasedEvent>::Get().size());

					MouseButtonReleasedEvent e(btn, window->GetReleasedWidget());
					for (auto* listener : window->EventDispatcher<MouseButtonReleasedEvent>::Get())
					{
						listener->OnEvent(e);
						if (e.Handled)
							break;
					}

					break;
				}
				}
			});

		glfwSetKeyCallback(m_hWnd,
			[](GLFWwindow* hWnd, int key, int scancode, int action, int mods)
			{
				if (key == -1)
					return;

				Window* window = (Window*)glfwGetWindowUserPointer(hWnd);
				switch (action)
				{
				case GLFW_PRESS:
				{
					Key k = (Key)key;

					Keyboard::SetKeyState(k, true);

					Log::Trace("[Window] KeyPressedEvent ({0}) (Listeners: {1})",
						String::Construct(k),
						window->EventDispatcher<KeyPressedEvent>::Get().size());

					KeyPressedEvent e(k);
					for (auto* listener : window->EventDispatcher<KeyPressedEvent>::Get())
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

					Log::Trace("[Window] KeyReleasedEvent ({0}) (Listeners: {1})",
						String::Construct(k),
						window->EventDispatcher<KeyReleasedEvent>::Get().size());

					KeyReleasedEvent e(k);
					for (auto* listener : window->EventDispatcher<KeyReleasedEvent>::Get())
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

					Log::Trace("[Window] KeyRepeatedEvent ({0}, repeatCount: {1}) (Listeners: {2})",
						String::Construct(k), repeatCount,
						window->EventDispatcher<KeyRepeatedEvent>::Get().size());

					KeyRepeatedEvent e(k, repeatCount);
					for (auto* listener : window->EventDispatcher<KeyRepeatedEvent>::Get())
					{
						listener->OnEvent(e);
						if (e.Handled)
							break;
					}

					break;
				}
				}
			});

		glfwSetCharCallback(m_hWnd,
			[](GLFWwindow* hWnd, unsigned int keycode)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(hWnd);
				Log::Trace("[Window] CharEvent ({0}) (Listeners: {1})", keycode,
					window->EventDispatcher<CharEvent>::Get().size());

				CharEvent e(keycode);
				for (auto* listener : window->EventDispatcher<CharEvent>::Get())
				{
					listener->OnEvent(e);
					if (e.Handled)
						break;
				}
			});

		glfwSetScrollCallback(m_hWnd,
			[](GLFWwindow* hWnd, double xOffset, double yOffset)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(hWnd);
				if (yOffset > 0)
				{
					Log::Trace("[Window] ScrollUpEvent {x={0}, y={1}} (Listeners: {2})", xOffset,
						yOffset, window->EventDispatcher<ScrollUpEvent>::Get().size());

					ScrollUpEvent e({ xOffset, yOffset });
					for (auto* listener : window->EventDispatcher<ScrollUpEvent>::Get())
					{
						listener->OnEvent(e);
						if (e.Handled)
							break;
					}
				}
				else if (yOffset < 0)
				{
					Log::Trace("[Window] ScrollDownEvent {x={0}, y={1}} (Listeners: {2})", xOffset,
						yOffset, window->EventDispatcher<ScrollDownEvent>::Get().size());

					ScrollDownEvent e({ xOffset, yOffset });
					for (auto* listener : window->EventDispatcher<ScrollDownEvent>::Get())
					{
						listener->OnEvent(e);
						if (e.Handled)
							break;
					}
				}

				if (xOffset > 0)
				{
					Log::Trace("[Window] ScrollRightEvent {x={0}, y={1}} (Listeners: {2})", xOffset,
						yOffset, window->EventDispatcher<ScrollRightEvent>::Get().size());

					ScrollRightEvent e({ xOffset, yOffset });
					for (auto* listener : window->EventDispatcher<ScrollRightEvent>::Get())
					{
						listener->OnEvent(e);
						if (e.Handled)
							break;
					}
				}
				else if (xOffset < 0)
				{
					Log::Trace("[Window] ScrollLeftEvent {x={0}, y={1}} (Listeners: {2})", xOffset,
						yOffset, window->EventDispatcher<ScrollLeftEvent>::Get().size());

					ScrollLeftEvent e({ xOffset, yOffset });
					for (auto* listener : window->EventDispatcher<ScrollLeftEvent>::Get())
					{
						listener->OnEvent(e);
						if (e.Handled)
							break;
					}
				}
			});

		glfwSetFramebufferSizeCallback(m_hWnd,
			[](GLFWwindow* hWnd, int width, int height)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(hWnd);
				Log::Trace("[Window] FramebufferResizedEvent {w={0}, h={1}} (Listeners: {2})",
					width, height, window->EventDispatcher<FramebufferResizedEvent>::Get().size());

				FramebufferResizedEvent e({ width, height });
				for (auto* listener : window->EventDispatcher<FramebufferResizedEvent>::Get())
				{
					listener->OnEvent(e);
					if (e.Handled)
						break;
				}
			});

		glfwSetWindowSizeCallback(m_hWnd,
			[](GLFWwindow* hWnd, int width, int height)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(hWnd);
				Log::Trace("[Window] WindowResizedEvent {w={0}, h={1}} (Listeners: {2})", width,
					height, window->EventDispatcher<WindowResizedEvent>::Get().size());

				WindowResizedEvent e({ width, height });
				for (auto* listener : window->EventDispatcher<WindowResizedEvent>::Get())
				{
					listener->OnEvent(e);
					if (e.Handled)
						break;
				}
			});

		glfwSetWindowPosCallback(m_hWnd,
			[](GLFWwindow* hWnd, int x, int y)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(hWnd);
				Log::Trace("[Window] WindowMovedEvent {x={0}, y={1}} (Listeners: {2})", x, y,
					window->EventDispatcher<WindowMovedEvent>::Get().size());

				WindowMovedEvent e({ x, y });
				for (auto* listener : window->EventDispatcher<WindowMovedEvent>::Get())
				{
					listener->OnEvent(e);
					if (e.Handled)
						break;
				}
			});

		glfwSetWindowCloseCallback(m_hWnd,
			[](GLFWwindow* hWnd)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(hWnd);
				Log::Trace("[Window] WindowClosedEvent (Listeners: {0})",
					window->EventDispatcher<WindowClosedEvent>::Get().size());

				WindowClosedEvent e;
				for (auto* listener : window->EventDispatcher<WindowClosedEvent>::Get())
				{
					listener->OnEvent(e);
					if (e.Handled)
						break;
				}

				window->Close();
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
				Log::Trace("[Window] HoverLeaveEvent \"{0}\" (Listeners: {1})",
					m_HoverWidget->GetName(), EventDispatcher<HoverLeaveEvent>::Get().size());

				HoverLeaveEvent e(m_HoverWidget);
				for (auto* listener : EventDispatcher<HoverLeaveEvent>::Get())
					listener->OnEvent(e);

				m_HoverWidget = nullptr;
			}
			else if (Mouse::IsOnWidget(btn) && &btn != m_HoverWidget)
			{
				m_HoverWidget = &btn;

				Log::Trace("[Window] HoverEnterEvent \"{0}\" (Listeners: {1})",
					m_HoverWidget->GetName(), EventDispatcher<HoverEnterEvent>::Get().size());

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

	// RAY_TODO: Don't initialize it lazily?
	// RAY_TODO: Try initializing every time before glfw function is called or only in constructor
	// and Window::GetInstanceExtensions
	void Window::TryInitializeGlfw()
	{
		if (s_GlfwInitialized)
			return;

		int success = glfwInit();
		RAY_MEXPECTS(success, "[Window] Failed to initialize GLFW");
		s_GlfwInitialized = true;
		Log::Info("[Window] GLFW successfully initialized");
		glfwSetErrorCallback(GLFWErrorCallback);

		// Remove default opengl api that comes with glfw
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}
}  // namespace At0::Ray
