#include "rpch.h"
#include "OpenGLWindow.h"

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

#include "Ray/Input/Keyboard.h"
#include "Ray/Input/Mouse.h"

#include <RayRender/Renderer3D.h>

#include <RayDebug/RAssert.h>
#include <RayDebug/RLogger.h>
#include <RayDebug/RInstrumentor.h>



namespace At0::Ray
{
	struct GLFWCallbackData
	{
		OpenGLWindow& win;
		Size2& oldSize;
		Point2& oldPos;
		EventCallbackFn& immediateEventFn;
		bool& windowIsOpen;
		std::function<Widget*(const Event& e, const MouseInput& mouse)> eventReceiverFn;
	};

	bool OpenGLWindow::s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		RAY_PROFILE_FUNCTION();

		RAY_LOG_CRITICAL("[OpenGLWindow] GLFW Error ({0}): {1}", error, description);
		RAY_LOG_FLUSH();
	}


	OpenGLWindow::OpenGLWindow(const std::string_view name, Widget* parent, bool isMainWindow)
		: Window(name, parent, isMainWindow), m_OldPos{}, m_OldSize{}, m_hWnd(0)
	{
		RAY_PROFILE_FUNCTION();

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			RAY_ASSERT(success, "Failed to initialize GLFW");
			RAY_LOG_INFO("[OpenGLWindow] Successfully initialized GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		m_hWnd = glfwCreateWindow(1280, 720, "", nullptr, nullptr);
		m_IsOpen = true;

		GLFWCallbackData* data = new GLFWCallbackData(GLFWCallbackData
			{
				*this, 
				m_OldSize, 
				m_OldPos, 
				m_ImmediateEventFn, 
				m_IsOpen, 
				[this](const Event& e, const MouseInput& mouse) { return GetEventReceiver(e, mouse); }
			}
		);
		glfwSetWindowUserPointer(m_hWnd, data);

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
	
	void OpenGLWindow::SetTitle(const std::string_view title)
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

	void OpenGLWindow::Close()
	{
		RAY_PROFILE_FUNCTION();
		if (IsOpen())
		{
			delete (GLFWCallbackData*)glfwGetWindowUserPointer(m_hWnd);
			glfwDestroyWindow(m_hWnd);

			m_IsOpen = false;

			// Push a dummy event into the queue so that the condition_variable in the queue will notify all threads to check
			//Scope<WindowCloseEvent> eDummy = MakeScope<WindowCloseEvent>();
			//m_EventQueue.PushBack({ this, std::move(eDummy) });
		}
	}

	bool OpenGLWindow::IsOpen() const
	{
		return m_IsOpen;
	}

	void OpenGLWindow::OnUpdate()
	{
		RAY_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_hWnd);
		
		// --------------------------------------------------------
		// Rendering (TODO)
		glfwSwapBuffers(m_hWnd);
		// --------------------------------------------------------

		glfwPollEvents();
	}

	void OpenGLWindow::SetIcon(const std::string_view path)
	{
		RAY_ASSERT(false, "Incomplete Implementation");
	}

	bool OpenGLWindow::InitRenderer3D()
	{
		RAY_PROFILE_FUNCTION();

		if (!m_Renderer3D)
		{
			m_Renderer3D = MakeScope<Renderer3D>();
			//m_Renderer3D->Init(m_hWnd);
			return true;
		}
		return false;
	}

	bool OpenGLWindow::InitRenderer2D()
	{
		RAY_PROFILE_FUNCTION();

		//if (!m_Renderer2D)
		//{
		//	m_Renderer2D = MakeScope<Renderer2D>();
		//	m_Renderer2D->Init(m_hWnd);
		//  return true;
		//}
		return false;
	}

	Renderer3D* OpenGLWindow::GetRenderer3D() const
	{
		RAY_PROFILE_FUNCTION();

		if (GetParent())
		{
			return GetParent()->GetRenderer3D();
		}
		return m_Renderer3D.get();
	}
	
	void OpenGLWindow::SetUpEventCallbacks()
	{
		RAY_PROFILE_FUNCTION();

		glfwSetWindowCloseCallback((GLFWwindow*)m_hWnd, [](GLFWwindow* window)
			{
				GLFWCallbackData* pData = (GLFWCallbackData*)glfwGetWindowUserPointer(window);
				pData->win.Close();
			}
		);

		glfwSetCursorPosCallback(m_hWnd, [](GLFWwindow* window, double xPos, double yPos)
			{
				GLFWCallbackData* pData = (GLFWCallbackData*)glfwGetWindowUserPointer(window);
				pData->win.Mouse.SetPos({ (float)xPos, (float)yPos });

				Scope<MouseMoveEvent> e = MakeScope<MouseMoveEvent>(pData->oldPos, Point2{ (float)xPos, (float)yPos });
				//pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
			}
		);

		glfwSetMouseButtonCallback(m_hWnd, [](GLFWwindow* window, int button, int action, int mods)
			{
				GLFWCallbackData* pData = (GLFWCallbackData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					Scope<MouseButtonPressedEvent> e = MakeScope<MouseButtonPressedEvent>((MouseButton)(button + 1));

					switch (e->GetButton())
					{
					case MouseButton::Left:		pData->win.Mouse.SetLeftPressed(true); break;
					case MouseButton::Right:	pData->win.Mouse.SetRightPressed(true); break;
					case MouseButton::Middle:	pData->win.Mouse.SetMiddlePressed(true); break;
					}

					//pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
					break;
				}
				case GLFW_RELEASE:
				{
					Scope<MouseButtonReleasedEvent> e = MakeScope<MouseButtonReleasedEvent>((MouseButton)(button + 1));

					switch (e->GetButton())
					{
					case MouseButton::Left:		pData->win.Mouse.SetLeftPressed(false); break;
					case MouseButton::Right:	pData->win.Mouse.SetRightPressed(false); break;
					case MouseButton::Middle:	pData->win.Mouse.SetMiddlePressed(false); break;
					}

					//pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
					break;
				}
				}
			}
		);

		glfwSetKeyCallback(m_hWnd, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				GLFWCallbackData* pData = (GLFWCallbackData*)glfwGetWindowUserPointer(window);

				Scope<KeyEvent> e;
				switch (action)
				{
				case GLFW_PRESS:
					e = MakeScope<KeyPressedEvent>((unsigned char)key, 0);
					pData->win.Keyboard.SetKeyState((unsigned char)key, true);
					break;
				case GLFW_RELEASE:
					e = MakeScope<KeyReleasedEvent>((unsigned char)key);
					pData->win.Keyboard.SetKeyState((unsigned char)key, false);
					break;
				case GLFW_REPEAT:
					e = MakeScope<KeyPressedEvent>((unsigned char)key, 1);
					break;
				}

				//pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
			}
		);

		glfwSetCharCallback(m_hWnd, [](GLFWwindow* window, unsigned int keycode)
			{
				GLFWCallbackData* pData = (GLFWCallbackData*)glfwGetWindowUserPointer(window);

				Scope<CharEvent> e = MakeScope<CharEvent>(keycode);
				//pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
			}
		);

		glfwSetScrollCallback(m_hWnd, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				GLFWCallbackData* pData = (GLFWCallbackData*)glfwGetWindowUserPointer(window);
				// TODO: MouseWheelLeft and Right events

				if (yOffset > 0)
				{
					// TODO: Set default scroll value (120 on Windows)
					Scope<MouseWheelUpEvent> e = MakeScope<MouseWheelUpEvent>(int(yOffset * 120));
					//pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
				}
				else if (yOffset < 0)
				{
					// TODO: Set default scroll value (120 on Windows)
					Scope<MouseWheelDownEvent> e = MakeScope<MouseWheelDownEvent>(int(yOffset * 120));
					//pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
				}

				if (xOffset > 0)
				{
					// TODO: Set default scroll value (120 on Windows)
					Scope<MouseWheelLeftEvent> e = MakeScope<MouseWheelLeftEvent>(int(xOffset * 120));
					//pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
				}
				else if (xOffset < 0)
				{
					// TODO: Set default scroll value (120 on Windows)
					Scope<MouseWheelRightEvent> e = MakeScope<MouseWheelRightEvent>(int(xOffset * 120));
					//pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
				}
			}
		);

		glfwSetWindowRefreshCallback(m_hWnd, [](GLFWwindow* window)
			{
				GLFWCallbackData* pData = (GLFWCallbackData*)glfwGetWindowUserPointer(window);

				Scope<PaintEvent> e = MakeScope<PaintEvent>();
				//pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
			}
		);

		glfwSetWindowSizeCallback(m_hWnd, [](GLFWwindow* window, int width, int height)
			{
				GLFWCallbackData* pData = (GLFWCallbackData*)glfwGetWindowUserPointer(window);
				pData->win.MoveTo(Point2{ width, height });

				Scope<WindowResizeEvent> e = MakeScope<WindowResizeEvent>(pData->oldSize, Point2{ (float)width, (float)height });
				//pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
			}
		);
	}
}


