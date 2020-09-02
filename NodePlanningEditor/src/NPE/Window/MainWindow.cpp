#include "MainWindow.h"

#include "NPE/Controls/Node.h"

#include "NPE/Handlers/KeyboardEvent.h"
#include "NPE/Handlers/MouseEvent.h"
#include "NPE/Handlers/ApplicationEvent.h"


namespace NPE
{
	MainWindow::MainWindow(unsigned short width, unsigned short height, PCWSTR name, std::function<void(const Event&)> eventFn)
		: m_Data{ { 0, 0 }, { width, height }, eventFn }
	{
		if (!CreateNativeWindow(name, WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, m_Data.size.width, m_Data.size.height))
			return;

		RECT winRec;
		GetWindowRect(m_hWnd, &winRec);
		m_Data.pos.x = winRec.left;
		m_Data.pos.y = winRec.top;


		ShowWindow(m_hWnd, SW_MAXIMIZE);
		m_Controls.emplace_back(Node(this, { 10, 400 }, { 200, 200 }));
		//m_Controls.emplace_back(Node(this, { 250, 400 }, { 200, 200 }));
		//m_Controls.emplace_back(Node(this, { 500, 400 }, { 200, 200 }));
		//m_Controls.emplace_back(Node(this, { 750, 400 }, { 200, 200 }));
		//m_Controls.emplace_back(Node(this, { 1000, 400 }, { 200, 200 }));
		//m_Controls.emplace_back(Node(this, { 1250, 400 }, { 200, 200 }));
		//m_Controls.emplace_back(Node(this, { 1500, 400 }, { 200, 200 }));
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

	void MainWindow::Update(const RECT* rc, BOOL bErase)
	{
		InvalidateRect(m_hWnd, rc, FALSE);
		UpdateWindow(m_hWnd);
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
			m_Data.EventCallback(e);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			MouseButtonPressedEvent e(Button::LeftMouseButton);
			m_Data.EventCallback(e);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			MouseButtonReleasedEvent e(Button::LeftMouseButton);
			m_Data.EventCallback(e);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			MouseButtonPressedEvent e(Button::RightMouseButton);
			m_Data.EventCallback(e);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			MouseButtonReleasedEvent e(Button::RightMouseButton);
			m_Data.EventCallback(e);
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			MouseButtonPressedEvent e(Button::MiddleMouseButton);
			m_Data.EventCallback(e);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			MouseButtonReleasedEvent e(Button::MiddleMouseButton);
			m_Data.EventCallback(e);
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			const auto delta = GET_WHEEL_DELTA_WPARAM(wParam);
			if (delta < 0)
			{
				MouseWheelDownEvent e(delta);
				m_Data.EventCallback(e);
			}
			else
			{ 
				MouseWheelUpEvent e(delta);
				m_Data.EventCallback(e);
			}
			return 0;
		}
		case WM_KEYDOWN:
		{
			KeyPressedEvent e((unsigned char)wParam);
			m_Data.EventCallback(e);
			return 0;
		}
		case WM_KEYUP:
		{
			KeyReleasedEvent e((unsigned char)wParam);
			m_Data.EventCallback(e);
			return 0;
		}
		case WM_CHAR:
		{
			//Keyboard::OnChar((unsigned char)wParam);
			return 0;
		}
		case WM_SIZE:
		{
			m_Data.size.width = LOWORD(lParam);
			m_Data.size.height = HIWORD(lParam);
			AppResizeEvent e(m_Data.size);
			m_Data.EventCallback(e);
			return 0;
		}
		case WM_MOVE:
		{
			m_Data.pos.x = LOWORD(lParam);
			m_Data.pos.y = HIWORD(lParam);
			AppMoveEvent e(m_Data.pos);
			m_Data.EventCallback(e);
			return 0;
		}
		case WM_CLOSE:
		{
			//AppCloseEvent e;
			//m_Data.EventCallback(e);
			//return 0;
			break;
		}
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	void MainWindow::Paint(HDC hDC, RECT* rcDirty, BOOL bErase)
	{
		FillRect(hDC, rcDirty, CreateSolidBrush(RGB(35, 38, 40)));
	}
	
}