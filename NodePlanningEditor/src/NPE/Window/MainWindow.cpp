#include "MainWindow.h"

#include "NPE/Handlers/KeyboardEvent.h"
#include "NPE/Handlers/MouseEvent.h"
#include "NPE/Handlers/ApplicationEvent.h"

#include "NPE/Controls/Node.h"


namespace NPE
{
	MainWindow::MainWindow(const NSize& size, PCWSTR name, std::function<void(const Event&)> eventFn)
		: m_EventCallback(eventFn)
	{
		if (!CreateNativeWindow(name, WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, size.width, size.height))
			return;

		ShowWindow(m_hWnd, SW_MAXIMIZE);
		Renderer2D.Init(m_hWnd, 30);
		m_Controls.emplace_back(Node(m_hWnd, { 10, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }, "He"));
		m_Controls.emplace_back(Node(m_hWnd, { 250, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }, "He"));
		m_Controls.emplace_back(Node(m_hWnd, { 500, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }, "He"));
		m_Controls.emplace_back(Node(m_hWnd, { 750, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }, "He"));
		m_Controls.emplace_back(Node(m_hWnd, { 1000, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }, "He"));
		m_Controls.emplace_back(Node(m_hWnd, { 1250, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }, "He"));
		m_Controls.emplace_back(Node(m_hWnd, { 1500, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }, "He"));
		m_Controls.emplace_back(Node(m_hWnd, { 1750, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }, "He"));
		m_Controls.emplace_back(Node(m_hWnd, { 2000, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }, "He"));
		m_Controls.emplace_back(Node(m_hWnd, { 2250, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }, "He"));
		m_Controls.emplace_back(Node(m_hWnd, { 2500, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }, "He"));
	}

	int MainWindow::ProcessMessage()
	{
		MSG msg{};
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		return (int)msg.wParam;
	}

	LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_KILLFOCUS:
		{
			Mouse::ClearStates();
			Keyboard::ClearStates();
			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(m_hWnd, &ps);
			Paint(hDC, &ps.rcPaint, FALSE);
			EndPaint(m_hWnd, &ps);
			ReleaseDC(m_hWnd, hDC);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			POINTS pt = MAKEPOINTS(lParam);
			MouseMoveEvent e({ pt.x, pt.y });
			m_EventCallback(e);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			//DEBUG:
			InvalidateRect(m_hWnd, nullptr, FALSE);
			UpdateWindow(m_hWnd);

			MouseButtonPressedEvent e(Button::LeftMouseButton);
			m_EventCallback(e);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			MouseButtonReleasedEvent e(Button::LeftMouseButton);
			m_EventCallback(e);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			MouseButtonPressedEvent e(Button::RightMouseButton);
			m_EventCallback(e);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			MouseButtonReleasedEvent e(Button::RightMouseButton);
			m_EventCallback(e);
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			MouseButtonPressedEvent e(Button::MiddleMouseButton);
			m_EventCallback(e);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			MouseButtonReleasedEvent e(Button::MiddleMouseButton);
			m_EventCallback(e);
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			const auto delta = GET_WHEEL_DELTA_WPARAM(wParam);
			if (delta < 0)
			{
				MouseWheelDownEvent e(delta);
				m_EventCallback(e);
			}
			else
			{ 
				MouseWheelUpEvent e(delta);
				m_EventCallback(e);
			}
			return 0;
		}
		case WM_KEYDOWN:
		{
			KeyPressedEvent e((unsigned char)wParam);
			m_EventCallback(e);
			return 0;
		}
		case WM_KEYUP:
		{
			KeyReleasedEvent e((unsigned char)wParam);
			m_EventCallback(e);
			return 0;
		}
		case WM_CHAR:
		{
			//Keyboard::OnChar((unsigned char)wParam);
			return 0;
		}
		case WM_SIZE:
		{
			AppResizeEvent e(GetSize());
			m_EventCallback(e);
			return 0;
		}
		case WM_MOVE:
		{
			AppMoveEvent e(GetPos());
			m_EventCallback(e);
			return 0;
		}
		case WM_CLOSE:
		{
			AppCloseEvent e;
			m_EventCallback(e);
			break;
		}
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	void MainWindow::Paint(HDC hDC, RECT* rcDirty, BOOL bErase)
	{
		Renderer2D.BeginDraw();
		Renderer2D.RenderScene(NColor{ 35.0f, 38.0f, 40.0f });
		for (auto& control : m_Controls)
			Renderer2D.RenderRoundedRectControl(control);
		Renderer2D.EndDraw();
	}
	
	NPoint MainWindow::GetPos() const
	{
		RECT rc = GetRect();
		return { rc.left, rc.top };
	}

	NSize MainWindow::GetSize() const
	{
		RECT rc = GetRect();
		return { rc.left + rc.right, rc.top + rc.bottom };
	}
	
	RECT MainWindow::GetRect() const
	{
		RECT rc;
		GetWindowRect(m_hWnd, &rc);
		return rc;
	}
}