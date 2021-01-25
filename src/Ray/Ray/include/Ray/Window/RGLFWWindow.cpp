#include "Ray/Rpch.h"
#include "RGLFWWindow.h"

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// Annoying X11 macros
#ifdef None
#undef None
#endif

#include "Ray/Input/RKeyboard.h"
#include "Ray/Input/RMouse.h"

#include <RayRender/RRenderer3D.h>
#include <RayRender/RRendererAPI.h>

#include <RayDebug/RAssert.h>
#include <RayDebug/RLogger.h>
#include <RayDebug/RInstrumentor.h>
#include <RayUtil/RKeyCodes.h>
#include <RayUtil/RHelper.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>


namespace At0::Ray
{
	bool GLFWWindow::s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		RAY_PROFILE_FUNCTION();

		RAY_ASSERT(false, "[GLFWWindow] GLFW Error ({0}): {1}", error, description);
		Log::Critical("[OpenGLRenderer3D] GLFW Error ({0}): {1}", error, description);
		Log::Flush();
	}


	GLFWWindow::GLFWWindow(std::string_view name, const Point2& pos, const Size2& size, Widget* parent)
		: Window(name, parent), m_hWnd(0), m_CachedRawDeltaMousePos{ 0.0f, 0.0f }
	{
		RAY_PROFILE_FUNCTION();

		// GLFW should only be initialized once
		if (!s_GLFWInitialized)
		{
			s_GLFWInitialized = true;
			int success = glfwInit();
			RAY_ASSERT(success, "Failed to initialize GLFW");
			Log::Info("[GLFWWindow] Successfully initialized GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);

			// Remove default OpenGL context from glfw
			if (RendererAPI::GetAPI() == RendererAPI::Vulkan)
			{
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			}
		}

		// Create the window and move it to the specified position
		m_hWnd = glfwCreateWindow((int)size.x, (int)size.y, "", nullptr, nullptr);
		glfwSetWindowUserPointer(m_hWnd, this);
		glfwSetWindowPos(m_hWnd, (int)pos.x, (int)pos.y);
		m_IsOpen = true;

		// Create the renderer, also sets the context
		m_Renderer3D = Renderer3D::Create((void*)m_hWnd, *this);

		// Disable VSync by default
		SetVSync(false);

		SetUpEventCallbacks();
	}

	GLFWWindow::~GLFWWindow()
	{
		RAY_PROFILE_FUNCTION();
		if (m_hWnd)
		{
			Close();
		}
		glfwTerminate();
		Log::Debug("[GLFWWindow] '{0}' destroyed", this->GetName());
	}

	std::string GLFWWindow::GetTitle() const
	{
		// GLFW doesn't have functionality to get the current window title
		RAY_PROFILE_FUNCTION();

#ifdef _WIN32

		HWND hWnd = glfwGetWin32Window(m_hWnd);
		std::string buff;
		int len = GetWindowTextLengthA(hWnd);
		if (len > 0)
		{
			buff.resize(len);
			GetWindowTextA(hWnd, buff.data(), len + 1);
		}
		else
		{
			RAY_THROW_RUNTIME("[GLFWWindow] Failed to retrieve the Window Title");
		}

		return buff;

#elif defined(__linux__)

		::Window hWnd = glfwGetX11Window(m_hWnd);
		XTextProperty prop;
		if (XGetWMName(glfwGetX11Display(), hWnd, &prop) == 0)
		{
			RAY_THROW_RUNTIME("[GLFWWindow] Failed to retrieve the Window Title");
		}
		return (char*)prop.value;
#endif

	}

	void GLFWWindow::SetTitle(std::string_view title)
	{
		RAY_PROFILE_FUNCTION();
		glfwSetWindowTitle(m_hWnd, title.data());
	}

	void GLFWWindow::Show() const
	{
		RAY_PROFILE_FUNCTION();
		glfwShowWindow(m_hWnd);
	}

	void GLFWWindow::Hide() const
	{
		RAY_PROFILE_FUNCTION();
		glfwHideWindow(m_hWnd);
	}

	void GLFWWindow::Maximize() const
	{
		RAY_PROFILE_FUNCTION();
		glfwMaximizeWindow(m_hWnd);
	}

	void GLFWWindow::Minimize() const
	{
		RAY_PROFILE_FUNCTION();
		glfwIconifyWindow(m_hWnd);
	}

	void GLFWWindow::Move(const Point2& pos)
	{
		glfwSetWindowPos(m_hWnd, pos.x, pos.y);
	}

	void GLFWWindow::Resize(const Size2& size)
	{
		glfwSetWindowSize(m_hWnd, size.x, size.y);
	}

	Point2 GLFWWindow::GetPos() const
	{
		int x, y;
		glfwGetWindowPos(m_hWnd, &x, &y);
		return { (float)x, (float)y };
	}

	Size2 GLFWWindow::GetSize() const
	{
		int width, height;
		glfwGetWindowSize(m_hWnd, &width, &height);
		return { (float)width, (float)height };
	}

	void GLFWWindow::Close()
	{
		RAY_PROFILE_FUNCTION();
		if (IsOpen())
		{
			glfwDestroyWindow(m_hWnd);

			m_IsOpen = false;

			WindowCloseEvent e;
			for (auto* pListener : EventDispatcher<WindowCloseEvent>::Get())
			{
				pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
			}
		}
	}

	bool GLFWWindow::IsOpen() const
	{
		return m_IsOpen;
	}

	void GLFWWindow::Update()
	{
		RAY_PROFILE_FUNCTION();

		// RAY_TODO: Multiple windows
		//glfwMakeContextCurrent(m_hWnd);

		glfwPollEvents();
	}

	void GLFWWindow::SetIcon(std::string_view path)
	{
		RAY_MEXPECTS(!Util::EndsWith(path, ".ico"),
			"[GLFWWindow::SetIcon] File '{0}' has an invalid extension. "
			".ico files are currently not supported by the OpenGL implementation.",
			path
		);

		GLFWimage images[1];
		images[0].pixels = stbi_load(path.data(), &images[0].width, &images[0].height, 0, 4);
		glfwSetWindowIcon(m_hWnd, std::size(images), images);
		stbi_image_free(images[0].pixels);
	}

	void GLFWWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval((int)enabled);
	}

	void GLFWWindow::SetMousePos(const Point2& pos)
	{
		glfwSetCursorPos(m_hWnd, (double)pos.x, (double)pos.y);
	}

	Point2 GLFWWindow::GetMousePos() const
	{
		double xPos, yPos;
		glfwGetCursorPos(m_hWnd, &xPos, &yPos);

		return { (float)xPos, (float)yPos };
	}

	void GLFWWindow::EnableCursor()
	{
		glfwSetInputMode(m_hWnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void GLFWWindow::DisableCursor()
	{
		glfwSetInputMode(m_hWnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	bool GLFWWindow::CursorEnabled() const
	{
		return glfwGetInputMode(m_hWnd, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
	}

	void GLFWWindow::SetUpEventCallbacks()
	{
		RAY_PROFILE_FUNCTION();

		glfwSetCursorPosCallback(m_hWnd, [](GLFWwindow* window, double xPos, double yPos)
			{
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);
				win.Mouse.SetPos({ (float)xPos, (float)yPos });

				Point2 rawDelta{ 0.0f, 0.0f };
				if (!win.CursorEnabled())
					rawDelta = Point2{ (float)xPos, (float)yPos } - win.m_CachedRawDeltaMousePos;

				Point2 mousePos{ (float)xPos, (float)yPos };
				win.m_CachedRawDeltaMousePos = mousePos;
				MouseMoveEvent e(mousePos, rawDelta);
				for (auto* pListener : win.EventDispatcher<MouseMoveEvent>::Get())
				{
					pListener->OnEvent(*win.GetEventReceiver(e, win.Mouse), e);
				}
			}
		);

		glfwSetMouseButtonCallback(m_hWnd, [](GLFWwindow* window, int button, int action, int mods)
			{
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e((MouseButton)(button + 1));

					switch (e.GetButton())
					{
					case MouseButton::Left:		win.Mouse.SetLeftPressed(true); break;
					case MouseButton::Right:	win.Mouse.SetRightPressed(true); break;
					case MouseButton::Middle:	win.Mouse.SetMiddlePressed(true); break;
					}


					for (auto* pListener : win.EventDispatcher<MouseButtonPressedEvent>::Get())
					{
						pListener->OnEvent(*win.GetEventReceiver(e, win.Mouse), e);
					}
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e((MouseButton)(button + 1));

					switch (e.GetButton())
					{
					case MouseButton::Left:		win.Mouse.SetLeftPressed(false); break;
					case MouseButton::Right:	win.Mouse.SetRightPressed(false); break;
					case MouseButton::Middle:	win.Mouse.SetMiddlePressed(false); break;
					}

					for (auto* pListener : win.EventDispatcher<MouseButtonReleasedEvent>::Get())
					{
						pListener->OnEvent(*win.GetEventReceiver(e, win.Mouse), e);
					}
					break;
				}
				}
			}
		);

		glfwSetKeyCallback(m_hWnd, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent e((Key)key, 0);
					win.Keyboard.SetKeyState((Key)key, true);
					for (auto* pListener : win.EventDispatcher<KeyPressedEvent>::Get())
					{
						pListener->OnEvent(*win.GetEventReceiver(e, win.Mouse), e);
					}

					break;
				}

				case GLFW_RELEASE:
				{
					KeyReleasedEvent e((Key)key);
					win.Keyboard.SetKeyState((Key)key, false);
					for (auto* pListener : win.EventDispatcher<KeyReleasedEvent>::Get())
					{
						pListener->OnEvent(*win.GetEventReceiver(e, win.Mouse), e);
					}
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e((Key)key, 1);
					for (auto* pListener : win.EventDispatcher<KeyPressedEvent>::Get())
					{
						pListener->OnEvent(*win.GetEventReceiver(e, win.Mouse), e);
					}

					break;
				}
				}
			}
		);

		glfwSetCharCallback(m_hWnd, [](GLFWwindow* window, unsigned int keycode)
			{
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);

				CharEvent e((uint16_t)keycode);
				for (auto* pListener : win.EventDispatcher<CharEvent>::Get())
				{
					pListener->OnEvent(*win.GetEventReceiver(e, win.Mouse), e);
				}			}
		);

		glfwSetScrollCallback(m_hWnd, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);

				if (yOffset > 0)
				{
					MouseWheelUpEvent e((float)yOffset);
					for (auto* pListener : win.EventDispatcher<MouseWheelUpEvent>::Get())
					{
						pListener->OnEvent(*win.GetEventReceiver(e, win.Mouse), e);
					}
				}
				else if (yOffset < 0)
				{
					MouseWheelDownEvent e((float)yOffset);
					for (auto* pListener : win.EventDispatcher<MouseWheelDownEvent>::Get())
					{
						pListener->OnEvent(*win.GetEventReceiver(e, win.Mouse), e);
					}
				}

				if (xOffset > 0)
				{
					MouseWheelRightEvent e((float)xOffset);
					for (auto* pListener : win.EventDispatcher<MouseWheelRightEvent>::Get())
					{
						pListener->OnEvent(*win.GetEventReceiver(e, win.Mouse), e);
					}
				}
				else if (xOffset < 0)
				{
					MouseWheelLeftEvent e((float)xOffset);
					for (auto* pListener : win.EventDispatcher<MouseWheelLeftEvent>::Get())
					{
						pListener->OnEvent(*win.GetEventReceiver(e, win.Mouse), e);
					}
				}
			}
		);

		glfwSetWindowRefreshCallback(m_hWnd, [](GLFWwindow* window)
			{
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);

				PaintEvent e;
				for (auto* pListener : win.EventDispatcher<PaintEvent>::Get())
				{
					pListener->OnEvent(*win.GetEventReceiver(e, win.Mouse), e);
				}
			}
		);

		glfwSetWindowSizeCallback(m_hWnd, [](GLFWwindow* window, int width, int height)
			{
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);

				//ResizeTo(Point2{ width, height });

				WindowResizeEvent e(win.m_OldSize, Point2{ (float)width, (float)height });
				for (auto* pListener : win.EventDispatcher<WindowResizeEvent>::Get())
				{
					pListener->OnEvent(win, e);
				}
			}
		);

		glfwSetWindowPosCallback(m_hWnd, [](GLFWwindow* window, int x, int y)
			{
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);

				//MoveTo(Point2{ x, y });

				WindowMoveEvent e(win.m_OldPos, Point2{ (float)x, (float)y });
				for (auto* pListener : win.EventDispatcher<WindowMoveEvent>::Get())
				{
					pListener->OnEvent(win, e);
				}
			}
		);

		glfwSetWindowCloseCallback(m_hWnd, [](GLFWwindow* window)
			{
				GLFWWindow& win = *(GLFWWindow*)glfwGetWindowUserPointer(window);

				win.Close();
			}
		);
	}
}
