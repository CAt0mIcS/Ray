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

#include <stb_image/stb_image.h>

// RAY_TODO: App closes when non-mainwindow is resized and closed


namespace At0::Ray
{
	WinAPIWindow::WinAPIWindow(std::string_view name, const Point2 pos, const Size2 size, Widget* parent)
		: Window{ name, parent }, m_hWnd{ 0 }, m_WindowStyles{ WS_OVERLAPPEDWINDOW }
	{
		RAY_PROFILE_FUNCTION();

		auto rnd = Util::GenerateRandomToken<std::wstring>(5);
		RAY_MEXPECTS(!rnd.empty(), "The random token generated for the window class name is empty");
		Log::Info("[Window] Creating Window Class with Name '{0}'", rnd);
		RAY_WND_THROW_LAST_FAILED(CreateNativeWindow(L"", rnd.c_str(), m_WindowStyles, 0, pos.x, pos.y, size.x, size.y));
		m_IsOpen = true;

		if (s_KeycodeMap[0] == (Key)0)
			SetKeycodeMap();

		m_Renderer3D = MakeScope<Renderer3D>(*this);
		m_Renderer3D->Init(m_hWnd);
	}

	WinAPIWindow::~WinAPIWindow()
	{
		RAY_PROFILE_FUNCTION();
		Log::Debug("[WinAPIWindow] '{0}' destroyed", this->GetName());
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
			Log::Debug("[MessageLoop] WM_DESTROY message received");

			if (m_DestroyOnWindowCloseData.hCurrIcon)
				DestroyIcon(m_DestroyOnWindowCloseData.hCurrIcon);

			PostQuitMessage(0);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			// ANSWER: Classes need to be built in a hierachy and shouldn't depend on each other much

			POINTS pt = MAKEPOINTS(lParam);
			Mouse.SetPos({ (float)pt.x, (float)pt.y });

			// Loop over the widgets and determine the widget which should receive the mouse move event
			SetHoveringWidget();

			MouseMoveEvent e(Mouse.GetPos());
			for (auto* pListener : EventDispatcher<MouseMoveEvent>::Get())
			{
				pListener->OnEvent(*m_CurrentlyHovering, e);
			}

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			Mouse.SetLeftPressed(true);

			MouseButtonPressedEvent e(MouseButton::Left);
			for (auto* pListener : EventDispatcher<MouseButtonPressedEvent>::Get())
			{
				pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		case WM_LBUTTONUP:
		{
			Mouse.SetLeftPressed(false);

			MouseButtonReleasedEvent e(MouseButton::Left);
			for (auto* pListener : EventDispatcher<MouseButtonReleasedEvent>::Get())
			{
				pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			Mouse.SetMiddlePressed(true);

			MouseButtonPressedEvent e(MouseButton::Middle);
			for (auto* pListener : EventDispatcher<MouseButtonPressedEvent>::Get())
			{
				pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		case WM_MBUTTONUP:
		{
			Mouse.SetMiddlePressed(false);

			MouseButtonReleasedEvent e(MouseButton::Middle);
			for (auto* pListener : EventDispatcher<MouseButtonReleasedEvent>::Get())
			{
				pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		case WM_XBUTTONDOWN:
		{
			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
			{
				MouseButtonPressedEvent e(MouseButton::Button3);
				for (auto* pListener : EventDispatcher<MouseButtonPressedEvent>::Get())
				{
					pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
				}
			}
			else
			{
				MouseButtonPressedEvent e(MouseButton::Button4);
				for (auto* pListener : EventDispatcher<MouseButtonPressedEvent>::Get())
				{
					pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
				}
			}

			return TRUE;
		}
		case WM_XBUTTONUP:
		{
			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
			{
				MouseButtonReleasedEvent e(MouseButton::Button3);
				for (auto* pListener : EventDispatcher<MouseButtonReleasedEvent>::Get())
				{
					pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
				}
			}
			else
			{
				MouseButtonReleasedEvent e(MouseButton::Button4);
				for (auto* pListener : EventDispatcher<MouseButtonReleasedEvent>::Get())
				{
					pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
				}
			}

			return TRUE;
		}
		case WM_RBUTTONDOWN:
		{
			Mouse.SetRightPressed(true);

			MouseButtonPressedEvent e(MouseButton::Right);
			for (auto* pListener : EventDispatcher<MouseButtonPressedEvent>::Get())
			{
				pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		case WM_RBUTTONUP:
		{
			Mouse.SetRightPressed(false);

			MouseButtonReleasedEvent e(MouseButton::Right);
			for (auto* pListener : EventDispatcher<MouseButtonReleasedEvent>::Get())
			{
				pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
			}
			return 0;
		}
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			int scancode = (HIWORD(lParam) & (KF_EXTENDED | 0xff));
			bool pressed = (HIWORD(lParam) & KF_UP) ? false : true;
			// Some synthetic key messages have a scancode of 0
			if (!scancode)
				scancode = MapVirtualKeyW((UINT)wParam, MAPVK_VK_TO_VSC);

			if (scancode >= 512)
				RAY_THROW_RUNTIME("[WinAPIWindow::HandleMessage] Scancode '{0}' is invalid.", scancode);

			Key key = s_KeycodeMap[scancode];

			if (wParam == VK_CONTROL)
			{
				if (HIWORD(lParam) & KF_EXTENDED)
				{
					// Right side keys have the extended key bit set
					key = Key::RightControl;
				}
				else
				{
					// Alt Gr sends Left Ctrl followed by Right Alt.
					MSG next;
					const DWORD time = GetMessageTime();

					if (PeekMessageW(&next, NULL, 0, 0, PM_NOREMOVE))
					{
						if (next.message == WM_KEYDOWN ||
							next.message == WM_SYSKEYDOWN ||
							next.message == WM_KEYUP ||
							next.message == WM_SYSKEYUP)
						{
							if (next.wParam == VK_MENU && (HIWORD(next.lParam) & KF_EXTENDED) && next.time == time)
							{
								// Next message is Right Alt down. Discard it
								break;
							}
						}
					}

					// Regular Left Control message
					key = Key::LeftControl;
				}
			}
			else if (wParam == VK_PROCESSKEY)
			{
				break;
			}

			Keyboard.SetKeyState(key, pressed);

			if (pressed)
			{
				KeyPressedEvent e(key, (uint32_t)(lParam & 0xff));
				for (auto* pListener : EventDispatcher<KeyPressedEvent>::Get())
				{
					pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
				}
			}
			else
			{
				KeyReleasedEvent e(key);
				for (auto* pListener : EventDispatcher<KeyReleasedEvent>::Get())
				{
					pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
				}
			}
			return 0;
		}
		case WM_CHAR:
		{
			if (wParam >= 0xd800 && wParam <= 0xdbff)
				m_HighSurrogate = (wchar_t)wParam;
			else
			{
				uint16_t codepoint = 0;

				if (wParam >= 0xdc00 && wParam <= 0xdfff)
				{
					if (m_HighSurrogate)
					{
						codepoint += (m_HighSurrogate - 0xd800) << 10;
						codepoint += (wchar_t)wParam - 0xdc00;
						codepoint += 0x10000;
					}
				}
				else
					codepoint = (wchar_t)wParam;

				m_HighSurrogate = 0;

				CharEvent e(codepoint);
				for (auto* pListener : EventDispatcher<CharEvent>::Get())
				{
					pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
				}

			}

			return 0;
		}
		case WM_UNICHAR:
		{
			// Not sent by Windows but some third-party input method engines

			if (wParam == UNICODE_NOCHAR)
			{
				// Returning true means that we support this message
				return TRUE;
			}

			CharEvent e(wParam);
			for (auto* pListener : EventDispatcher<CharEvent>::Get())
			{
				pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
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
					pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
				}
			}
			else if (delta < 0)
			{
				MouseWheelDownEvent e(delta / (float)WHEEL_DELTA);
				for (auto* pListener : EventDispatcher<MouseWheelDownEvent>::Get())
				{
					pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
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
					pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
				}
			}
			else if (delta < 0)
			{
				MouseWheelLeftEvent e(delta / (float)WHEEL_DELTA);
				for (auto* pListener : EventDispatcher<MouseWheelLeftEvent>::Get())
				{
					pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
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
				pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
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
				pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
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
				pListener->OnEvent(*GetEventReceiver(e, Mouse), e);
			}

			m_OldPos = newPos;
			return 0;
		}
		case WM_CLOSE:
		{
			Log::Debug("[MessageLoop] WM_CLOSE message received");
			m_IsOpen = false;

			WindowCloseEvent e;
			for (auto* pListener : EventDispatcher<WindowCloseEvent>::Get())
			{
				pListener->OnEvent(*this, e);
			}

			if (e.WindowCloseAborted())
				return 0;

			break;
		}
		case WM_QUIT:
		{
			Log::Debug("[MessageLoop] WM_QUIT message received");
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

	void WinAPIWindow::Move(const Point2& pos)
	{
		Size2 size = GetSize();

		RECT wr;
		wr.left = pos.x;
		wr.top = pos.y;
		wr.right = wr.left + size.x;
		wr.bottom = wr.top + size.y;
		AdjustWindowRect(&wr, m_WindowStyles, FALSE);

		RAY_WND_THROW_LAST_FAILED(SetWindowPos(m_hWnd, NULL, wr.left, wr.top, 0, 0,
			SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE)
		);
	}

	void WinAPIWindow::Resize(const Size2& size)
	{
		Point2 pos = GetPos();

		RECT wr;
		wr.left = pos.x;
		wr.top = pos.y;
		wr.right = wr.left + size.x;
		wr.bottom = wr.top + size.y;
		AdjustWindowRect(&wr, m_WindowStyles, FALSE);

		RAY_WND_THROW_LAST_FAILED(SetWindowPos(m_hWnd, HWND_TOP,
			0, 0, wr.right - wr.left, wr.bottom - wr.top,
			SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER)
		);
	}

	Point2 WinAPIWindow::GetPos() const
	{
		POINT pt{ 0, 0 };
		RAY_WND_THROW_LAST_FAILED(ClientToScreen(m_hWnd, &pt));
		return Point2{ (float)pt.x, (float)pt.y };
	}

	Size2 WinAPIWindow::GetSize() const
	{
		RECT rc;
		RAY_WND_THROW_LAST_FAILED(GetClientRect(m_hWnd, &rc));
		return Size2{ (float)(rc.right - rc.left), (float)(rc.bottom - rc.top) };
	}

	void WinAPIWindow::Close()
	{
		if (IsOpen())
			SendMessage(m_hWnd, WM_CLOSE, 0, 0);
	}

	bool WinAPIWindow::IsOpen() const
	{
		return m_IsOpen;
	}

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

	// -------------------------------------------------------------------------------
	// Icon/Image loading
	struct Image
	{
		int width, height;
		unsigned char* pixels;
	};

	static HICON CreateIcon(Image& image)
	{
		BITMAPV5HEADER bitmap;
		ZeroMemory(&bitmap, sizeof(bitmap));
		bitmap.bV5Size = sizeof(bitmap);
		bitmap.bV5Width = image.width;
		bitmap.bV5Height = -image.height;
		bitmap.bV5Planes = 1;
		bitmap.bV5BitCount = 32;
		bitmap.bV5Compression = BI_BITFIELDS;
		bitmap.bV5RedMask = 0x00ff0000;
		bitmap.bV5GreenMask = 0x0000ff00;
		bitmap.bV5BlueMask = 0x000000ff;
		bitmap.bV5AlphaMask = 0xff000000;

		unsigned char* target = nullptr;
		HDC hDC = GetDC(NULL);
		HBITMAP color = CreateDIBSection(hDC, (BITMAPINFO*)&bitmap,
			DIB_RGB_COLORS, (void**)&target, NULL, (DWORD)0
		);

		ReleaseDC(NULL, hDC);
		// RAY_TODO: check if usefull error message
		RAY_WND_THROW_LAST_FAILED(color);

		HBITMAP mask = CreateBitmap(image.width, image.height, 1, 1, nullptr);
		if (!mask)
		{
			DeleteObject(color);
			RAY_WND_THROW_LAST_FAILED_NO_EXPR();
		}

		for (uint32_t i = 0; i < image.width * image.height; ++i)
		{
			target[0] = image.pixels[2];
			target[1] = image.pixels[1];
			target[2] = image.pixels[0];
			target[3] = image.pixels[3];
			target += 4;
			image.pixels += 4;
		}

		ICONINFO ii;
		ZeroMemory(&ii, sizeof(ii));
		ii.fIcon = true;
		ii.xHotspot = 0;
		ii.yHotspot = 0;
		ii.hbmMask = mask;
		ii.hbmColor = color;

		HICON hIcon = CreateIconIndirect(&ii);

		DeleteObject(color);
		DeleteObject(mask);

		RAY_WND_THROW_LAST_FAILED(hIcon);
		return hIcon;
	}

	void WinAPIWindow::SetIcon(std::string_view path)
	{
		Image image;
		image.pixels = stbi_load(path.data(), &image.width, &image.height, 0, 4);
		HICON hIcon = CreateIcon(image);

		SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		m_DestroyOnWindowCloseData.hCurrIcon = hIcon;
	}
	// End Icon/Image loading
	// -------------------------------------------------------------------------------


	void WinAPIWindow::SetKeycodeMap()
	{
		memset(s_KeycodeMap, -1, sizeof(s_KeycodeMap));

		s_KeycodeMap[0x00B] = Key::D0;
		s_KeycodeMap[0x002] = Key::D1;
		s_KeycodeMap[0x003] = Key::D2;
		s_KeycodeMap[0x004] = Key::D3;
		s_KeycodeMap[0x005] = Key::D4;
		s_KeycodeMap[0x006] = Key::D5;
		s_KeycodeMap[0x007] = Key::D6;
		s_KeycodeMap[0x008] = Key::D7;
		s_KeycodeMap[0x009] = Key::D8;
		s_KeycodeMap[0x00A] = Key::D9;
		s_KeycodeMap[0x01E] = Key::A;
		s_KeycodeMap[0x030] = Key::B;
		s_KeycodeMap[0x02E] = Key::C;
		s_KeycodeMap[0x020] = Key::D;
		s_KeycodeMap[0x012] = Key::E;
		s_KeycodeMap[0x021] = Key::F;
		s_KeycodeMap[0x022] = Key::G;
		s_KeycodeMap[0x023] = Key::H;
		s_KeycodeMap[0x017] = Key::I;
		s_KeycodeMap[0x024] = Key::J;
		s_KeycodeMap[0x025] = Key::K;
		s_KeycodeMap[0x026] = Key::L;
		s_KeycodeMap[0x032] = Key::M;
		s_KeycodeMap[0x031] = Key::N;
		s_KeycodeMap[0x018] = Key::O;
		s_KeycodeMap[0x019] = Key::P;
		s_KeycodeMap[0x010] = Key::Q;
		s_KeycodeMap[0x013] = Key::R;
		s_KeycodeMap[0x01F] = Key::S;
		s_KeycodeMap[0x014] = Key::T;
		s_KeycodeMap[0x016] = Key::U;
		s_KeycodeMap[0x02F] = Key::V;
		s_KeycodeMap[0x011] = Key::W;
		s_KeycodeMap[0x02D] = Key::X;
		s_KeycodeMap[0x015] = Key::Y;
		s_KeycodeMap[0x02C] = Key::Z;

		s_KeycodeMap[0x028] = Key::Apostrophe;
		s_KeycodeMap[0x02B] = Key::Backslash;
		s_KeycodeMap[0x033] = Key::Comma;
		s_KeycodeMap[0x00D] = Key::Equal;
		s_KeycodeMap[0x029] = Key::GraveAccent;
		s_KeycodeMap[0x01A] = Key::LeftBracket;
		s_KeycodeMap[0x00C] = Key::Minus;
		s_KeycodeMap[0x034] = Key::Period;
		s_KeycodeMap[0x01B] = Key::RightBracket;
		s_KeycodeMap[0x027] = Key::Semicolon;
		s_KeycodeMap[0x035] = Key::Slash;
		s_KeycodeMap[0x056] = Key::World2;

		s_KeycodeMap[0x00E] = Key::Backspace;
		s_KeycodeMap[0x153] = Key::Delete;
		s_KeycodeMap[0x14F] = Key::End;
		s_KeycodeMap[0x01C] = Key::Enter;
		s_KeycodeMap[0x001] = Key::Escape;
		s_KeycodeMap[0x147] = Key::Home;
		s_KeycodeMap[0x152] = Key::Insert;
		s_KeycodeMap[0x15D] = Key::Menu;
		s_KeycodeMap[0x151] = Key::PageDown;
		s_KeycodeMap[0x149] = Key::PageUp;
		s_KeycodeMap[0x045] = Key::Pause;
		s_KeycodeMap[0x146] = Key::Pause;
		s_KeycodeMap[0x039] = Key::Space;
		s_KeycodeMap[0x00F] = Key::Tab;
		s_KeycodeMap[0x03A] = Key::CapsLock;
		s_KeycodeMap[0x145] = Key::NumLock;
		s_KeycodeMap[0x046] = Key::ScrollLock;
		s_KeycodeMap[0x03B] = Key::F1;
		s_KeycodeMap[0x03C] = Key::F2;
		s_KeycodeMap[0x03D] = Key::F3;
		s_KeycodeMap[0x03E] = Key::F4;
		s_KeycodeMap[0x03F] = Key::F5;
		s_KeycodeMap[0x040] = Key::F6;
		s_KeycodeMap[0x041] = Key::F7;
		s_KeycodeMap[0x042] = Key::F8;
		s_KeycodeMap[0x043] = Key::F9;
		s_KeycodeMap[0x044] = Key::F10;
		s_KeycodeMap[0x057] = Key::F11;
		s_KeycodeMap[0x058] = Key::F12;
		s_KeycodeMap[0x064] = Key::F13;
		s_KeycodeMap[0x065] = Key::F14;
		s_KeycodeMap[0x066] = Key::F15;
		s_KeycodeMap[0x067] = Key::F16;
		s_KeycodeMap[0x068] = Key::F17;
		s_KeycodeMap[0x069] = Key::F18;
		s_KeycodeMap[0x06A] = Key::F19;
		s_KeycodeMap[0x06B] = Key::F20;
		s_KeycodeMap[0x06C] = Key::F21;
		s_KeycodeMap[0x06D] = Key::F22;
		s_KeycodeMap[0x06E] = Key::F23;
		s_KeycodeMap[0x076] = Key::F24;
		s_KeycodeMap[0x038] = Key::LeftAlt;
		s_KeycodeMap[0x01D] = Key::LeftControl;
		s_KeycodeMap[0x02A] = Key::LeftShift;
		s_KeycodeMap[0x15B] = Key::LeftSuper;
		s_KeycodeMap[0x137] = Key::PrintScreen;
		s_KeycodeMap[0x138] = Key::RightAlt;
		s_KeycodeMap[0x11D] = Key::RightControl;
		s_KeycodeMap[0x036] = Key::RightShift;
		s_KeycodeMap[0x15C] = Key::RightSuper;
		s_KeycodeMap[0x150] = Key::Down;
		s_KeycodeMap[0x14B] = Key::Left;
		s_KeycodeMap[0x14D] = Key::Right;
		s_KeycodeMap[0x148] = Key::Up;

		s_KeycodeMap[0x052] = Key::NumPad0;
		s_KeycodeMap[0x04F] = Key::NumPad1;
		s_KeycodeMap[0x050] = Key::NumPad2;
		s_KeycodeMap[0x051] = Key::NumPad3;
		s_KeycodeMap[0x04B] = Key::NumPad4;
		s_KeycodeMap[0x04C] = Key::NumPad5;
		s_KeycodeMap[0x04D] = Key::NumPad6;
		s_KeycodeMap[0x047] = Key::NumPad7;
		s_KeycodeMap[0x048] = Key::NumPad8;
		s_KeycodeMap[0x049] = Key::NumPad9;
		s_KeycodeMap[0x04E] = Key::NumPadAdd;
		s_KeycodeMap[0x053] = Key::NumPadDecimal;
		s_KeycodeMap[0x135] = Key::NumPadDivide;
		s_KeycodeMap[0x11C] = Key::NumPadEnter;
		s_KeycodeMap[0x059] = Key::NumPadEqual;
		s_KeycodeMap[0x037] = Key::NumPadMultiply;
		s_KeycodeMap[0x04A] = Key::NumPadSubtract;
	}

}

#endif // _WIN32