#include "rlpch.h"
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

#include "Reyal/Events/ApplicationEvent.h"
#include "Reyal/Events/MouseEvent.h"
#include "Reyal/Events/KeyboardEvent.h"

#include "Reyal/Input/Keyboard.h"
#include "Reyal/Input/Mouse.h"

#include <RlRender/Renderer3D.h>

#include <RlDebug/RlAssert.h>
#include <RlDebug/ReyalLogger.h>
#include <RlDebug/Instrumentor.h>



namespace At0::Reyal
{
	struct GLFWCallbackData
	{
		OpenGLWindow& win;
		Size2& oldSize;
		Point2& oldPos;
		EventCallbackFn& immediateEventFn;
		std::function<Widget*(const Event& e, const MouseInput& mouse)> eventReceiverFn;
	};

	bool OpenGLWindow::s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		RL_PROFILE_FUNCTION();

		RL_LOG_CRITICAL("[OpenGLWindow] GLFW Error ({0}): {1}", error, description);
	}


	OpenGLWindow::OpenGLWindow(const std::string_view name, Widget* parent, bool isMainWindow)
		: Window(name, parent, isMainWindow)
	{
		RL_PROFILE_FUNCTION();

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			RL_ASSERT(success, "Failed to initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_hWnd = glfwCreateWindow(1280, 720, "", nullptr, nullptr);

		GLFWCallbackData* d = new GLFWCallbackData(GLFWCallbackData{ *this, m_OldSize, m_OldPos, m_ImmediateEventFn, [this](const Event& e, const MouseInput& mouse) { return GetEventReceiver(e, mouse); } });
		glfwSetWindowUserPointer((GLFWwindow*)m_hWnd, d);

		// Context initialization
		glfwMakeContextCurrent((GLFWwindow*)m_hWnd);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RL_ASSERT(status, "Failed to initialize Glad!");
		
		RL_LOG_INFO("[OpenGLWindow] OpenGL Info:");
		RL_LOG_INFO("	Vendor:   {0}", glGetString(GL_VENDOR));
		RL_LOG_INFO("	Renderer: {0}", glGetString(GL_RENDERER));
		RL_LOG_INFO("	Version:	 {0}", glGetString(GL_VERSION));
		RL_LOG_FLUSH();

		SetUpEventCallbacks();

		// Push dummy event so we don't call front on empty queue in application
		Scope<WindowResizeEvent> e = MakeScope<WindowResizeEvent>(m_OldSize, Size2{ 1280.0f, 720.0f });
		m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
	}
	
	OpenGLWindow::~OpenGLWindow()
	{
		RL_PROFILE_FUNCTION();
		if (m_hWnd)
		{
			Close();
		}
	}

	std::string OpenGLWindow::GetTitle() const
	{
		// GLFW doesn't have functionality to get the current window title
		RL_PROFILE_FUNCTION();
		
#ifdef _WIN32

		HWND hWnd = glfwGetWin32Window((GLFWwindow*)m_hWnd);
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
			RL_LOG_WARN("[OpenGLWindow] Failed to retrieve the Window Title");
		}

		return buff;

#elif defined(__linux__)

		::Window hWnd = glfwGetX11Window((GLFWwindow*)m_hWnd);
		XTextProperty prop;
		if (XGetWMName(glfwGetX11Display(), hWnd, &prop) == 0)
		{
			// error
			RL_LOG_WARN("[OpenGLWindow] Failed to retrieve the Window Title");
		}
		return (char*)prop.value;
#endif

	}
	
	void OpenGLWindow::SetTitle(const std::string_view title)
	{
		RL_PROFILE_FUNCTION();
		glfwSetWindowTitle((GLFWwindow*)m_hWnd, title.data());
	}

	void OpenGLWindow::Show() const
	{
		RL_PROFILE_FUNCTION();
		glfwShowWindow((GLFWwindow*)m_hWnd);
	}

	void OpenGLWindow::Hide() const
	{
		RL_PROFILE_FUNCTION();
		glfwHideWindow((GLFWwindow*)m_hWnd);
	}

	void OpenGLWindow::Maximize() const
	{
		RL_PROFILE_FUNCTION();
		glfwMaximizeWindow((GLFWwindow*)m_hWnd);
	}

	void OpenGLWindow::Minimize() const
	{
		RL_PROFILE_FUNCTION();
		glfwIconifyWindow((GLFWwindow*)m_hWnd);
	}

	void OpenGLWindow::Close() const
	{
		RL_PROFILE_FUNCTION();
		delete (GLFWCallbackData*)glfwGetWindowUserPointer((GLFWwindow*)m_hWnd);
		glfwDestroyWindow((GLFWwindow*)m_hWnd);
	}

	bool OpenGLWindow::IsOpen() const
	{
		RL_PROFILE_FUNCTION();
		return m_hWnd ? true : false;
	}
	
	bool OpenGLWindow::ShouldClose()
	{
		RL_PROFILE_FUNCTION();

		glfwPollEvents();
		//glfwSwapBuffers((GLFWwindow*)m_hWnd);
		return glfwWindowShouldClose((GLFWwindow*)m_hWnd);
	}
	
	void OpenGLWindow::SetUpEventCallbacks()
	{
		RL_PROFILE_FUNCTION();

		glfwSetWindowCloseCallback((GLFWwindow*)m_hWnd, [](GLFWwindow* window)
			{
				GLFWCallbackData* pData = (GLFWCallbackData*)glfwGetWindowUserPointer(window);

				WindowCloseEvent e = WindowCloseEvent();
				pData->immediateEventFn(pData->eventReceiverFn(e, pData->win.Mouse), e);
			}
		);

		glfwSetCursorPosCallback((GLFWwindow*)m_hWnd, [](GLFWwindow* window, double xPos, double yPos)
			{
				GLFWCallbackData* pData = (GLFWCallbackData*)glfwGetWindowUserPointer(window);
				pData->win.Mouse.SetPos({ (float)xPos, (float)yPos });

				Scope<MouseMoveEvent> e = MakeScope<MouseMoveEvent>(pData->oldPos, Point2{ (float)xPos, (float)yPos });
				pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
			}
		);

		glfwSetMouseButtonCallback((GLFWwindow*)m_hWnd, [](GLFWwindow* window, int button, int action, int mods)
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

					pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
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

					pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
					break;
				}
				}
			}
		);

		glfwSetKeyCallback((GLFWwindow*)m_hWnd, [](GLFWwindow* window, int key, int scancode, int action, int mods)
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

				pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
			}
		);

		glfwSetCharCallback((GLFWwindow*)m_hWnd, [](GLFWwindow* window, unsigned int keycode)
			{
				GLFWCallbackData* pData = (GLFWCallbackData*)glfwGetWindowUserPointer(window);

				Scope<CharEvent> e = MakeScope<CharEvent>(keycode);
				pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
			}
		);

		glfwSetScrollCallback((GLFWwindow*)m_hWnd, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				GLFWCallbackData* pData = (GLFWCallbackData*)glfwGetWindowUserPointer(window);
				// TODO: MouseWheelLeft and Right events

				if (yOffset > 0)
				{
					// TODO: Set default scroll value (120 on Windows)
					Scope<MouseWheelUpEvent> e = MakeScope<MouseWheelUpEvent>(int(yOffset * 120));
					pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
				}
				else if (yOffset < 0)
				{
					// TODO: Set default scroll value (120 on Windows)
					Scope<MouseWheelDownEvent> e = MakeScope<MouseWheelDownEvent>(int(yOffset * 120));
					pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
				}

				if (xOffset > 0)
				{
					// TODO: Set default scroll value (120 on Windows)
					Scope<MouseWheelLeftEvent> e = MakeScope<MouseWheelLeftEvent>(int(xOffset * 120));
					pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
				}
				else if (xOffset < 0)
				{
					// TODO: Set default scroll value (120 on Windows)
					Scope<MouseWheelRightEvent> e = MakeScope<MouseWheelRightEvent>(int(xOffset * 120));
					pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
				}
			}
		);

		glfwSetWindowRefreshCallback((GLFWwindow*)m_hWnd, [](GLFWwindow* window)
			{
				GLFWCallbackData* pData = (GLFWCallbackData*)glfwGetWindowUserPointer(window);
			
				Scope<PaintEvent> e = MakeScope<PaintEvent>();
				pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
			}
		);

		glfwSetWindowSizeCallback((GLFWwindow*)m_hWnd, [](GLFWwindow* window, int width, int height)
			{
				GLFWCallbackData* pData = (GLFWCallbackData*)glfwGetWindowUserPointer(window);
				pData->win.MoveTo(Point2{ width, height });

				Scope<WindowResizeEvent> e = MakeScope<WindowResizeEvent>(pData->oldSize, Point2{ (float)width, (float)height });
				pData->win.GetEventQueue().PushBack({ pData->eventReceiverFn(*e, pData->win.Mouse), std::move(e) });
			}
		);
	}
}


