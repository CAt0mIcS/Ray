#include "Ray/Rpch.h"

#ifdef _WIN32

#include "RWinAPIWindow.h"

#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RLogger.h>
#include <RayDebug/RAssert.h>

#include <RayUtil/RException.h>
#include <RayUtil/RRandom.h>
#include <RayUtil/RKeyCodes.h>

#include <RayRender/Renderer3D.h>

#include <Windows.h>

// RAY_TODO: App closes when non-mainwindow is resized and closed


namespace At0::Ray
{
	WinAPIWindow::WinAPIWindow(std::string_view name, const Point2 pos, const Size2 size, Widget* parent)
		: Window(name, parent), m_hWnd(0)
	{
		RAY_PROFILE_FUNCTION();

		auto rnd = Util::GenerateRandomToken<std::wstring>(5);
		RAY_MEXPECTS(!rnd.empty(), "The random token generated for the window class name is empty");
		RAY_LOG_INFO("[Window] Creating Window Class with Name '{0}'", rnd);
		RAY_WND_THROW_LAST_FAILED(CreateNativeWindow(L"", rnd.c_str(), WS_OVERLAPPEDWINDOW, 0, pos.x, pos.y, size.x, size.y));
		m_IsOpen = true;
	}

	WinAPIWindow::~WinAPIWindow()
	{
		RAY_PROFILE_FUNCTION();
		RAY_LOG_DEBUG("[WinAPIWindow] '{0}' destroyed", this->GetName());
		DestroyWindow(m_hWnd);
	}

	bool WinAPIWindow::CreateNativeWindow(
		const wchar_t* windowName, 
		const wchar_t* windowClassName, 
		DWORD style, DWORD exStyle, 
		uint32_t x, uint32_t y, 
		uint32_t width, uint32_t height, 
		HWND hWndParent, HMENU hMenu
	)
	{
		RAY_PROFILE_FUNCTION();

		WNDCLASS wc{};
		wc.lpfnWndProc = WinAPIWindow::WindowProcSetup;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = windowClassName;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);

		RegisterClass(&wc);

		RECT wr;
		wr.left = x;
		wr.top = y;
		wr.right = wr.left + width;
		wr.bottom = wr.top + height;
		AdjustWindowRect(&wr, style, FALSE);

		m_hWnd = CreateWindowEx(
			exStyle, windowClassName, windowName, style, wr.left, wr.top, wr.right - wr.left,
			wr.bottom - wr.top, hWndParent, hMenu, wc.hInstance, this
		);

		return m_hWnd ? true : false;
	}
	
	LRESULT WinAPIWindow::WindowProcSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		RAY_PROFILE_FUNCTION();

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			WinAPIWindow* pThis = (WinAPIWindow*)pCreate->lpCreateParams;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WinAPIWindow::WindowProcPass);

			pThis->m_hWnd = hWnd;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	LRESULT WinAPIWindow::WindowProcPass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		RAY_PROFILE_FUNCTION();

		WinAPIWindow* pThis = (WinAPIWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		return pThis->HandleMessage(uMsg, wParam, lParam);
	}
	
	LRESULT WinAPIWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		RAY_PROFILE_FUNCTION();
		
		switch (uMsg)
		{
		case WM_DESTROY:
		{
			// RAY_TODO: Check if resources of the closed window are destroyed correctly
			RAY_LOG_DEBUG("[MessageLoop] WM_DESTROY message received");
			PostQuitMessage(0);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			//static std::chrono::time_point<std::chrono::high_resolution_clock> prevTime;
			//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - prevTime).count() << '\n';
			//prevTime = std::chrono::high_resolution_clock::now();

			// ANSWER: Classes need to be built in a hierachy and shouldn't depend on each other much

			POINTS pt = MAKEPOINTS(lParam);
			Mouse.SetPos({ (float)pt.x, (float)pt.y });

			MouseMoveEvent e(Mouse.GetPos());
			for (auto* pListener : EventDispatcher<MouseMoveEvent>::Get())
			{
				pListener->OnEvent(GetEventReceiver(e, Mouse), e);
			}


			// Loop over the widgets and check if the mouse left any
			SetHoveringWidget();

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			Mouse.SetLeftPressed(true);

			MouseButtonPressedEvent e(MouseButton::Left);
			for (auto* pListener : EventDispatcher<MouseButtonPressedEvent>::Get())
			{
				pListener->OnEvent(GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		case WM_LBUTTONUP:
		{
			Mouse.SetLeftPressed(false);

			MouseButtonReleasedEvent e(MouseButton::Left);
			for (auto* pListener : EventDispatcher<MouseButtonReleasedEvent>::Get())
			{
				pListener->OnEvent(GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			Mouse.SetMiddlePressed(true);

			MouseButtonPressedEvent e(MouseButton::Middle);
			for (auto* pListener : EventDispatcher<MouseButtonPressedEvent>::Get())
			{
				pListener->OnEvent(GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		case WM_MBUTTONUP:
		{
			Mouse.SetMiddlePressed(false);

			MouseButtonReleasedEvent e(MouseButton::Middle);
			for (auto* pListener : EventDispatcher<MouseButtonReleasedEvent>::Get())
			{
				pListener->OnEvent(GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		// RAY_TODO: Test, add more mouse buttons and add xbuttons in MouseInput
		case WM_XBUTTONDOWN:
		{
			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
			{
				MouseButtonPressedEvent e(MouseButton::Button3);
				for (auto* pListener : EventDispatcher<MouseButtonPressedEvent>::Get())
				{
					pListener->OnEvent(GetEventReceiver(e, Mouse), e);
				}
			}
			else
			{
				MouseButtonPressedEvent e(MouseButton::Button4);
				for (auto* pListener : EventDispatcher<MouseButtonPressedEvent>::Get())
				{
					pListener->OnEvent(GetEventReceiver(e, Mouse), e);
				}
			}

			return 0;
		}
		case WM_XBUTTONUP:
		{
			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
			{
				MouseButtonReleasedEvent e(MouseButton::Button3);
				for (auto* pListener : EventDispatcher<MouseButtonReleasedEvent>::Get())
				{
					pListener->OnEvent(GetEventReceiver(e, Mouse), e);
				}
			}
			else
			{
				MouseButtonReleasedEvent e(MouseButton::Button4);
				for (auto* pListener : EventDispatcher<MouseButtonReleasedEvent>::Get())
				{
					pListener->OnEvent(GetEventReceiver(e, Mouse), e);
				}
			}

			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			Mouse.SetRightPressed(true);

			MouseButtonPressedEvent e(MouseButton::Right);
			for (auto* pListener : EventDispatcher<MouseButtonPressedEvent>::Get())
			{
				pListener->OnEvent(GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		case WM_RBUTTONUP:
		{
			Mouse.SetRightPressed(false);

			MouseButtonReleasedEvent e(MouseButton::Right);
			for (auto* pListener : EventDispatcher<MouseButtonReleasedEvent>::Get())
			{
				pListener->OnEvent(GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		// RAY_TODO: Keyboard message using new mouse/keyboard keycodes
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			Keyboard.SetKeyState((uint16_t)wParam, true);

			KeyPressedEvent e((uint16_t)wParam, (uint32_t)(lParam & 0xff));
			for (auto* pListener : EventDispatcher<KeyPressedEvent>::Get())
			{
				pListener->OnEvent(GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			Keyboard.SetKeyState((uint16_t)wParam, false);

			KeyReleasedEvent e((uint16_t)wParam);
			for (auto* pListener : EventDispatcher<KeyReleasedEvent>::Get())
			{
				pListener->OnEvent(GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		case WM_CHAR:
		{
			CharEvent e((unsigned char)wParam);
			for (auto* pListener : EventDispatcher<CharEvent>::Get())
			{
				pListener->OnEvent(GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);

			if (delta > 0)
			{
				MouseWheelUpEvent e(delta / (float)WHEEL_DELTA);
				for (auto* pListener : EventDispatcher<MouseWheelUpEvent>::Get())
				{
					pListener->OnEvent(GetEventReceiver(e, Mouse), e);
				}
			}
			else if (delta < 0)
			{
				MouseWheelDownEvent e(delta / (float)WHEEL_DELTA);
				for (auto* pListener : EventDispatcher<MouseWheelDownEvent>::Get())
				{
					pListener->OnEvent(GetEventReceiver(e, Mouse), e);
				}
			}

			return 0;
		}
		case WM_MOUSEHWHEEL:
		{
			// Negate delta for GLFW compatibility
			int delta = -GET_WHEEL_DELTA_WPARAM(wParam);

			if (delta > 0)
			{
				MouseWheelRightEvent e(delta / (float)WHEEL_DELTA);
				for (auto* pListener : EventDispatcher<MouseWheelRightEvent>::Get())
				{
					pListener->OnEvent(GetEventReceiver(e, Mouse), e);
				}
			}
			else if (delta < 0)
			{
				MouseWheelLeftEvent e(delta / (float)WHEEL_DELTA);
				for (auto* pListener : EventDispatcher<MouseWheelLeftEvent>::Get())
				{
					pListener->OnEvent(GetEventReceiver(e, Mouse), e);
				}
			}

			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(m_hWnd, &ps);
			EndPaint(m_hWnd, &ps);

			PaintEvent e;
			for (auto* pListener : EventDispatcher<PaintEvent>::Get())
			{
				pListener->OnEvent(GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		case WM_SIZE:
		{
			Size2 newSize = { (float)LOWORD(lParam), (float)HIWORD(lParam) };
			//ResizeTo(newSize);

			//RAY_TODO: Read how windows handles events (how they're built, how they handle it)

			WindowResizeEvent e(m_OldSize, newSize);
			for (auto* pListener : EventDispatcher<WindowResizeEvent>::Get())
			{
				pListener->OnEvent(GetEventReceiver(e, Mouse), e);
			}

			m_OldSize = newSize;
			return 0;
		}
		case WM_MOVE:
		{
			Point2 newPos = { (float)LOWORD(lParam), (float)HIWORD(lParam) };
			//MoveTo(newPos);

			WindowMoveEvent e(m_OldPos, newPos);
			for (auto* pListener : EventDispatcher<WindowMoveEvent>::Get())
			{
				pListener->OnEvent(GetEventReceiver(e, Mouse), e);
			}

			m_OldPos = newPos;
			return 0;
		}
		case WM_CLOSE:
		{
			RAY_LOG_DEBUG("[MessageLoop] WM_CLOSE message received");
			m_IsOpen = false;

			WindowCloseEvent e;
			for (auto* pListener : EventDispatcher<WindowCloseEvent>::Get())
			{
				pListener->OnEvent(this, e);
			}

			if (e.WindowCloseAborted())
				return 0;

			break;
		}
		case WM_QUIT:
		{
			RAY_LOG_DEBUG("[MessageLoop] WM_QUIT message received");
			break;
		}
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	std::string WinAPIWindow::GetTitle() const
	{
		std::string str;
		int len = GetWindowTextLengthA(m_hWnd);
		if (len > 0)
		{
			str.resize(len);
			GetWindowTextA(m_hWnd, str.data(), len + 1);
		}

		return str;
	}

	void WinAPIWindow::SetTitle(std::string_view title)
	{
		RAY_WND_THROW_LAST_FAILED(SetWindowTextA(m_hWnd, title.data()));
	}

	void WinAPIWindow::Show() const
	{
		ShowWindow(m_hWnd, SW_SHOW);
	}

	void WinAPIWindow::Hide() const
	{
		ShowWindow(m_hWnd, SW_HIDE);
	}

	void WinAPIWindow::Maximize() const
	{
		ShowWindow(m_hWnd, SW_MAXIMIZE);
	}

	void WinAPIWindow::Minimize() const
	{
		ShowWindow(m_hWnd, SW_MINIMIZE);
	}

	void WinAPIWindow::SetPos(const Point2& pos)
	{
		auto size = GetSize();
		RAY_WND_THROW_LAST_FAILED(SetWindowPos(m_hWnd, 0, pos.x, pos.y, size.x, size.y, 0));
	}

	void WinAPIWindow::SetSize(const Size2& size)
	{
		auto pos = GetPos();
		RAY_WND_THROW_LAST_FAILED(SetWindowPos(m_hWnd, 0, pos.x, pos.y, size.x, size.y, 0));
	}

	Point2 WinAPIWindow::GetPos() const
	{
		RECT rc;
		RAY_WND_THROW_LAST_FAILED(GetWindowRect(m_hWnd, &rc));
		return Point2{ (float)rc.left, (float)rc.top };
	}

	Size2 WinAPIWindow::GetSize() const
	{
		RECT rc;
		RAY_WND_THROW_LAST_FAILED(GetWindowRect(m_hWnd, &rc));
		return Size2{ (float)rc.right - rc.left, (float)rc.bottom - rc.top };
	}

	void WinAPIWindow::Close()
	{
		if(IsOpen())
			SendMessage(m_hWnd, WM_CLOSE, 0, 0);
	}

	bool WinAPIWindow::IsOpen() const
	{
		return m_IsOpen;
	}

	/// <QUESTION>
	/// While GetMessage is much faster than if GetMessage.
	/// I need to execute code every iteration. Take fptr in fn? std::function?
	/// 
	/// while GetMessage doesn't close the window properly if GetMessage gets m_hWnd as parameter instead of the first '0'
	/// </QUESTION>
	void WinAPIWindow::Update()
	{
		RAY_PROFILE_FUNCTION();

		MSG msg;
		while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				m_IsOpen = false;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void WinAPIWindow::SetIcon(std::string_view path)
	{
		RAY_ASSERT(false, "Incomplete Implementation");
	}
}

#endif // _WIN32