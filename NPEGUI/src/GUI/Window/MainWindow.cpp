#include "pch.h"
#include "MainWindow.h"

#include "GUI/Events/KeyboardEvent.h"
#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/ApplicationEvent.h"

#include "GUI/Graphics/TextRenderer.h"

#include "Util/Exceptions.h"

#include "GUI/Util/Timer.h"

/// <TODO>
/// Implement scrolling for the multiline TextBox
/// </TODO>

namespace GUI
{
	MainWindow::MainWindow()
		: Control(Type::Window)
	{
		if (!CreateNativeWindow(L"NPE", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT))
		{
			NPE_THROW_LAST_WND_EXCEPT();
		}
	
		this->SetFocus();

		ShowWindow(m_hWnd, SW_MAXIMIZE);
		Renderer::Get().Init(m_hWnd);
		TextRenderer::Get().Init();
	}

	bool MainWindow::Render()
	{
		Renderer::Get().RenderScene();
		for (auto* child : m_Children)
		{
			child->Render();
		}
		return true;
	}

	bool MainWindow::OnEvent(_In_ Event& e)
	{
		switch (e.GetType())
		{
		case EventType::MouseButtonPressedEvent:
		{
			if (((MouseButtonPressedEvent&)e).GetButton() == MouseButton::Left)
			{
				//TODO: Make more performant, only render part where the caret disappeared
				this->SetFocus();
			
				PostRedraw();
			}
			
			break;
		}
		case EventType::SetCursorEvent:
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			return true;
		}
		}
		return false;
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

	LRESULT MainWindow::HandleMessage(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
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
		case WM_SETCURSOR:
		{
			SetCursorEvent e;
			if (!DispatchEvent(e)) break;
			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(m_hWnd, &ps);

			PaintEvent e(hDC, &ps.rcPaint);

			if (!DispatchPaintEvent(e))
			{
				EndPaint(m_hWnd, &ps);
				break;
			}
			EndPaint(m_hWnd, &ps);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			POINTS pt = MAKEPOINTS(lParam);
			MouseMoveEvent e({ (float)pt.x, (float)pt.y });
			if (!DispatchEvent(e)) break;

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			MouseButtonPressedEvent e(MouseButton::Left);
			if (!DispatchEvent(e)) break;

			return 0;
		}
		case WM_LBUTTONUP:
		{
			MouseButtonReleasedEvent e(MouseButton::Left);
			if (!DispatchEvent(e)) break;
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			MouseButtonPressedEvent e(MouseButton::Right);
			if (!DispatchEvent(e)) break;
			return 0;
		}
		case WM_RBUTTONUP:
		{
			MouseButtonReleasedEvent e(MouseButton::Right);
			if (!DispatchEvent(e)) break;
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			MouseButtonPressedEvent e(MouseButton::Middle);
			if (!DispatchEvent(e)) break;
			return 0;
		}
		case WM_MBUTTONUP:
		{
			MouseButtonReleasedEvent e(MouseButton::Middle);
			if (!DispatchEvent(e)) break;
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			const auto delta = GET_WHEEL_DELTA_WPARAM(wParam);
			if (delta < 0)
			{
				MouseWheelDownEvent e(delta);
				if (!DispatchEvent(e)) break;
			}
			else
			{ 
				MouseWheelUpEvent e(delta);
				if (!DispatchEvent(e)) break;
			}
			return 0;
		}
		case WM_KEYDOWN:
		{
			if (wParam < 256)
			{
				KeyPressedEvent e((unsigned char)wParam);
				if (!DispatchEvent(e)) break;
			}
			return 0;
		}
		case WM_KEYUP:
		{
			if (wParam < 256)
			{
				KeyReleasedEvent e((unsigned char)wParam);
				if (!DispatchEvent(e)) break;
			}
			return 0;
		}
		case WM_CHAR:
		{
			if (wParam > 0 && wParam < 0x10000)
			{
				CharEvent e((unsigned char)wParam);
				if (!DispatchEvent(e)) break;
			}
			return 0;
		}
		case WM_SIZE:
		{
			m_Size = Util::NSize{ (float)LOWORD(lParam), (float)HIWORD(lParam) };

			AppResizeEvent e(GetSize());
			if (!DispatchEvent(e)) break;
			return 0;
		}
		case WM_MOVE:
		{
			m_Pos = Util::NPoint{ (float)LOWORD(lParam), (float)HIWORD(lParam) };

			AppMoveEvent e(GetPos());
			if (!DispatchEvent(e)) break;
			return 0;
		}
		case WM_TIMER:
		{
			if (!HandleTimer((unsigned int)wParam)) break;
			return 0;
		}
		case WM_CLOSE:
		{
			AppCloseEvent e;
			if (!DispatchEvent(e)) break;
			return 0;
		}
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	
	RECT MainWindow::GetRect() const
	{
		RECT rc;
		GetWindowRect(m_hWnd, &rc);
		return rc;
	}
	
	void MainWindow::CreateTimer(_In_ unsigned int time, _In_ bool repeats)
	{
		auto& timer = m_Timers.emplace_back(m_hWnd, repeats);
		timer.Run(time);
	}

	void MainWindow::SetTitle(const std::wstring& title)
	{
		SetWindowText(m_hWnd, title.c_str());
	}

	MainWindow::~MainWindow()
	{
		delete &Renderer::Get();
		delete &TextRenderer::Get();

		for (auto& timer : m_Timers)
		{
			timer.Destroy();
		}

		UnregisterClass(s_MainWindowClassName, NULL);
	}

	bool MainWindow::DispatchEvent(_In_ Event& e)
	{
		if (!m_EventCallbackFn)
			return false;

		Control* receiver = nullptr;
		for (auto* control : GetControls())
		{
			receiver = control->GetEventReceiver(e);
			if (receiver)
				break;
		}
		//if no Control was clicked/hovered/... then the window will receive the event
		if (receiver == nullptr)
			receiver = this;

		//Defines events which need to be handled by the focused object, like CharEvent
		if ((e.GetType() == EventType::CharEvent || 
			e.GetType() == EventType::KeyPressedEvent) && GetFocus())
		{
			if (GetFocus()->OnEvent(e))
			{
				m_EventCallbackFn(GetFocus(), e);
				return true;
			}
		}

		//dispatch event to all controls first
		if (receiver->OnEvent(e))
		{
			m_EventCallbackFn(receiver, e);
			return true;
		}

		//call event filter set by user
		return m_EventCallbackFn(receiver, e);
	}

	bool MainWindow::DispatchPaintEvent(_In_ PaintEvent& e)
	{
		Util::NTransform transform = Util::ToTransform(*e.GetRect());

		Control* receiver = nullptr;
		for (auto* control : m_Children)
		{
			auto& pos = control->GetPos();
			auto& size = control->GetSize();

			Util::NPoint diffP{ floor(pos.x), floor(pos.y) };
			Util::NSize diffS{ floor(size.width), floor(size.height) + 1 };

			if (transform.pos == diffP && transform.size == diffS)
			{
				receiver = control;
				break;
			}
		}
		if (!receiver)
			receiver = this;

		return m_EventCallbackFn(receiver, e);
	}
	
	bool MainWindow::HandleTimer(_In_ unsigned int id)
	{
		unsigned int idInVec = 0xffffffff;
		Timer* timer = nullptr;

		for (unsigned int i = 0; i < m_Timers.size(); ++i)
		{
			if (m_Timers[i].GetId() == id)
			{
				timer = &m_Timers[i];
				idInVec = i;
			}
		}

		auto destroyTimerFn = [&]()
		{
			if (!timer->IsRepeating())
			{
				timer->Destroy();
				m_Timers.erase(m_Timers.begin() + idInVec);
			}
		};

		if (!timer)
		{
			std::ostringstream oss;
			oss << "Failed to find timer with ID " << id << '\n';
			NPE_THROW_EXCEPT_MSG(oss.str().c_str());
		}

		TimerEvent e(timer);
		if (!DispatchEvent(e))
		{
			destroyTimerFn();
			return false;
		}

		destroyTimerFn();
		return true;
	}
}