#include "Ray/rpch.h"
#include "ROpenGLWindow.h"

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

#include <RayRender/Renderer3D.h>

#include <RayDebug/RAssert.h>
#include <RayDebug/RLogger.h>
#include <RayDebug/RInstrumentor.h>
#include <RayUtil/RKeyCodes.h>


namespace At0::Ray
{
	bool OpenGLWindow::s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		RAY_PROFILE_FUNCTION();

		RAY_LOG_CRITICAL("[OpenGLWindow] GLFW Error ({0}): {1}", error, description);
		RAY_LOG_FLUSH();
	}


	OpenGLWindow::OpenGLWindow(std::string_view name, const Point2& pos, const Size2& size, Widget* parent)
		: Window(name, parent), m_hWnd(0)
	{
		RAY_PROFILE_FUNCTION();

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			RAY_ASSERT(success, "Failed to initialize GLFW");
			RAY_LOG_INFO("[OpenGLWindow] Successfully initialized GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		m_hWnd = glfwCreateWindow((int)size.x, (int)size.y, "", nullptr, nullptr);
		glfwSetWindowPos(m_hWnd, (int)pos.x, (int)pos.y);
		m_IsOpen = true;

		// Context initialization
		glfwMakeContextCurrent(m_hWnd);

		// Initialize Glad
		if (!s_GLFWInitialized)
		{
			s_GLFWInitialized = true;

			int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			RAY_ASSERT(status, "Failed to initialize Glad!");

			RAY_LOG_INFO("[OpenGLWindow] Successfully initialized Glad");
			RAY_LOG_INFO("[OpenGLWindow] OpenGL Info:"
			"\n\tVendor:\t{0}\n\tRenderer: {1}\n\tVersion:{2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
		}

		SetUpEventCallbacks();

		// Push dummy event so we don't call front on empty queue in application
		//Scope<WindowResizeEvent> e = MakeScope<WindowResizeEvent>(m_OldSize, Size2{ 1280.0f, 720.0f });
		//m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
	}
	
	OpenGLWindow::~OpenGLWindow()
	{
		RAY_PROFILE_FUNCTION();
		if (m_hWnd)
		{
			Close();
		}
		glfwTerminate();
		RAY_LOG_DEBUG("[OpenGLWindow] '{0}' destroyed", this->GetName());
	}

	std::string OpenGLWindow::GetTitle() const
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
			//error
			RAY_LOG_WARN("[OpenGLWindow] Failed to retrieve the Window Title");
		}

		return buff;

#elif defined(__linux__)

		::Window hWnd = glfwGetX11Window(m_hWnd);
		XTextProperty prop;
		if (XGetWMName(glfwGetX11Display(), hWnd, &prop) == 0)
		{
			// error
			RAY_LOG_WARN("[OpenGLWindow] Failed to retrieve the Window Title");
		}
		return (char*)prop.value;
#endif

	}
	
	void OpenGLWindow::SetTitle(std::string_view title)
	{
		RAY_PROFILE_FUNCTION();
		glfwSetWindowTitle(m_hWnd, title.data());
	}

	void OpenGLWindow::Show() const
	{
		RAY_PROFILE_FUNCTION();
		glfwShowWindow(m_hWnd);
	}

	void OpenGLWindow::Hide() const
	{
		RAY_PROFILE_FUNCTION();
		glfwHideWindow(m_hWnd);
	}

	void OpenGLWindow::Maximize() const
	{
		RAY_PROFILE_FUNCTION();
		glfwMaximizeWindow(m_hWnd);
	}

	void OpenGLWindow::Minimize() const
	{
		RAY_PROFILE_FUNCTION();
		glfwIconifyWindow(m_hWnd);
	}

	void OpenGLWindow::SetPos(const Point2& pos)
	{
		glfwSetWindowPos(m_hWnd, pos.x, pos.y);
	}

	void OpenGLWindow::SetSize(const Size2& size)
	{
		glfwSetWindowSize(m_hWnd, size.x, size.y);
	}

	Point2 OpenGLWindow::GetPos() const
	{
		int x, y;
		glfwGetWindowPos(m_hWnd, &x, &y);
		return { x, y };
	}

	Size2 OpenGLWindow::GetSize() const
	{
		int width, height;
		glfwGetWindowSize(m_hWnd, &width, &height);
		return { width, height };
	}

	void OpenGLWindow::Close()
	{
		RAY_PROFILE_FUNCTION();
		if (IsOpen())
		{
			glfwDestroyWindow(m_hWnd);

			m_IsOpen = false;

			WindowCloseEvent e;
			for (auto* pListener : EventDispatcher<WindowCloseEvent>::Get())
			{
				pListener->OnEvent(GetEventReceiver(e, Mouse), e);
			}
		}
	}

	bool OpenGLWindow::IsOpen() const
	{
		return m_IsOpen;
	}

	void OpenGLWindow::Update()
	{
		RAY_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_hWnd);
		
		// --------------------------------------------------------
		// Rendering (RAY_TODO)
		glfwSwapBuffers(m_hWnd);
		// --------------------------------------------------------

		glfwPollEvents();
	}

	void OpenGLWindow::SetIcon(std::string_view path)
	{
		RAY_ASSERT(false, "Incomplete Implementation");
	}
	
	void OpenGLWindow::SetUpEventCallbacks()
	{
		RAY_PROFILE_FUNCTION();

		glfwSetCursorPosCallback(m_hWnd, [this](double xPos, double yPos)
			{
				Mouse.SetPos({ (float)xPos, (float)yPos });

				MouseMoveEvent e(Point2{ (float)xPos, (float)yPos });
				for (auto* pListener : EventDispatcher<MouseMoveEvent>::Get())
				{
					pListener->OnEvent(GetEventReceiver(e, Mouse), e);
				}
			}
		);

		glfwSetMouseButtonCallback(m_hWnd, [this](int button, int action, int mods)
			{
				std::cout << MouseButtonToString((MouseButton)button) << '\n';

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e((MouseButton)(button + 1));

					switch (e.GetButton())
					{
					case MouseButton::Left:		Mouse.SetLeftPressed(true); break;
					case MouseButton::Right:	Mouse.SetRightPressed(true); break;
					case MouseButton::Middle:	Mouse.SetMiddlePressed(true); break;
					}


					for (auto* pListener : EventDispatcher<MouseButtonPressedEvent>::Get())
					{
						pListener->OnEvent(GetEventReceiver(e, Mouse), e);
					}
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e((MouseButton)(button + 1));

					switch (e.GetButton())
					{
					case MouseButton::Left:		Mouse.SetLeftPressed(false); break;
					case MouseButton::Right:	Mouse.SetRightPressed(false); break;
					case MouseButton::Middle:	Mouse.SetMiddlePressed(false); break;
					}

					for (auto* pListener : EventDispatcher<MouseButtonReleasedEvent>::Get())
					{
						pListener->OnEvent(GetEventReceiver(e, Mouse), e);
					}
					break;
				}
				}
			}
		);

		glfwSetKeyCallback(m_hWnd, [this](int key, int scancode, int action, int mods)
			{
				switch (action)
				{
				case GLFW_PRESS:
				{
					Key k = (Key)key;
					std::cout << KeyToString(k) << '\n';

					KeyPressedEvent e((unsigned char)key, 0);
					Keyboard.SetKeyState((unsigned char)key, true);
					for (auto* pListener : EventDispatcher<KeyPressedEvent>::Get())
					{
						pListener->OnEvent(GetEventReceiver(e, Mouse), e);
					}

					break;
				}
					
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e((unsigned char)key);
					Keyboard.SetKeyState((unsigned char)key, false);
					for (auto* pListener : EventDispatcher<KeyReleasedEvent>::Get())
					{
						pListener->OnEvent(GetEventReceiver(e, Mouse), e);
					}
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e((unsigned char)key, 1);
					for (auto* pListener : EventDispatcher<KeyPressedEvent>::Get())
					{
						pListener->OnEvent(GetEventReceiver(e, Mouse), e);
					}

					break;
				}
				}
			}
		);

		glfwSetCharCallback(m_hWnd, [this](unsigned int keycode)
			{
				CharEvent e(keycode);
				for (auto* pListener : EventDispatcher<CharEvent>::Get())
				{
					pListener->OnEvent(GetEventReceiver(e, Mouse), e);
				}			}
		);

		glfwSetScrollCallback(m_hWnd, [this](double xOffset, double yOffset)
			{
				if (yOffset > 0)
				{
					MouseWheelUpEvent e((float)yOffset);
					for (auto* pListener : EventDispatcher<MouseWheelUpEvent>::Get())
					{
						pListener->OnEvent(GetEventReceiver(e, Mouse), e);
					}
				}
				else if (yOffset < 0)
				{
					MouseWheelDownEvent e((float)yOffset);
					for (auto* pListener : EventDispatcher<MouseWheelDownEvent>::Get())
					{
						pListener->OnEvent(GetEventReceiver(e, Mouse), e);
					}
				}

				if (xOffset > 0)
				{
					MouseWheelRightEvent e((float)xOffset);
					for (auto* pListener : EventDispatcher<MouseWheelRightEvent>::Get())
					{
						pListener->OnEvent(GetEventReceiver(e, Mouse), e);
					}
				}
				else if (xOffset < 0)
				{
					MouseWheelLeftEvent e((float)xOffset);
					for (auto* pListener : EventDispatcher<MouseWheelLeftEvent>::Get())
					{
						pListener->OnEvent(GetEventReceiver(e, Mouse), e);
					}
				}
			}
		);

		glfwSetWindowRefreshCallback(m_hWnd, [this]()
			{
				PaintEvent e;
				for (auto* pListener : EventDispatcher<PaintEvent>::Get())
				{
					pListener->OnEvent(GetEventReceiver(e, Mouse), e);
				}
			}
		);

		glfwSetWindowSizeCallback(m_hWnd, [this](int width, int height)
			{
				//ResizeTo(Point2{ width, height });

				WindowResizeEvent e(m_OldSize, Point2{ (float)width, (float)height });
				for (auto* pListener : EventDispatcher<WindowResizeEvent>::Get())
				{
					pListener->OnEvent(this, e);
				}
			}
		);

		glfwSetWindowPosCallback(m_hWnd, [this](int x, int y) 
			{
				//MoveTo(Point2{ x, y });

				WindowMoveEvent e(m_OldPos, Point2{ (float)x, (float)y });
				for (auto* pListener : EventDispatcher<WindowMoveEvent>::Get())
				{
					pListener->OnEvent(this, e);
				}
			}
		);

		glfwSetWindowCloseCallback(m_hWnd, [this]()
			{
				Close();
			}
		);
	}
}


