#include "MainWindow.h"

#include "NPE/Handlers/KeyboardEvent.h"
#include "NPE/Handlers/MouseEvent.h"
#include "NPE/Handlers/ApplicationEvent.h"

#include "NPE/Controls/Node.h"
#include "NPE/Controls/Button.h"
#include "NPE/Controls/TextBox.h"

#include "NPE/Util/Exceptions.h"


namespace NPE
{
	MainWindow::MainWindow(const NSize& size, PCWSTR name, std::function<void(const Event&)> eventFn)
		: m_EventCallback(eventFn), m_LineCons{}
	{
		if (!CreateNativeWindow(name, WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, (int)size.width, (int)size.height))
		{
			NPE_THROW_WND_EXCEPT(GetLastError());
		}
		
		ShowWindow(m_hWnd, SW_MAXIMIZE);
		Renderer2D.Init(m_hWnd);
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

	//DEBUG:
	bool wasClickedB = false;
	Button* buttonBackup = nullptr;

	bool wasClickedN = false;
	Node* nodeBackup = nullptr;

	bool wasClickedT = false;
	TextBox* txtBoxBackup = nullptr;
	//DEBUG END

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

			PaintEvent e(hDC, &ps.rcPaint);
			m_EventCallback(e);

			EndPaint(m_hWnd, &ps);
			ReleaseDC(m_hWnd, hDC);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			POINTS pt = MAKEPOINTS(lParam);
			MouseMoveEvent e({ (float)pt.x, (float)pt.y });
			m_EventCallback(e);

			#pragma region TemporaryControlHoldEvent
			if (Mouse::IsLeftPressed() || Mouse::IsRightPressed() || Mouse::IsMiddlePressed())
			{
				//TODO: make recursive to work with an infinite amount of child-parent relationships
				for (auto* control : m_Controls)
				{
					for (auto* child : control->GetChildren())
					{
						if (child->GetType() == Control::Type::Button && wasClickedB && child == buttonBackup)
						{
							Button* btn = (Button*)child;
							btn->OnButtonClickedEventCallback(*btn);
							return 0;
						}
						else if (child->GetType() == Control::Type::TextBox && wasClickedT && child == txtBoxBackup)
						{
							TextBox* txtBox = (TextBox*)child;
							txtBox->OnTextBoxClickedEventCallback(*txtBox);
						}
					}

					if (control->GetType() == Control::Type::Node && wasClickedN && control == nodeBackup)
					{
						Node* node = (Node*)control;
						node->OnNodeClickedEventCallback(*node);
						return 0;
					}

				}
			}
			#pragma endregion

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			MouseButtonPressedEvent e(MouseButton::Left);
			m_EventCallback(e);

			#pragma region TemporaryControlHoldEvent
			//TODO: make recursive to work with an infinite amount of child-parent relationships
			for (auto* control : m_Controls)
			{
				for (auto* child : control->GetChildren())
				{
					if (child->GetType() == Control::Type::Button && Mouse::IsOnControl(child))
					{
						Button* btn = (Button*)child;
						buttonBackup = btn;
						btn->OnButtonClickedEventCallback(*btn);
						wasClickedB = true;
						return 0;
					}
					else if (child->GetType() == Control::Type::TextBox && Mouse::IsOnControl(child))
					{
						TextBox* txtBox = (TextBox*)child;
						txtBoxBackup = txtBox;
						txtBox->OnTextBoxClickedEventCallback(*txtBox);
						wasClickedT = true;
						return 0;
					}
				}

				if (control->GetType() == Control::Type::Node && Mouse::IsOnControl(control))
				{
					Node* node = (Node*)control;

					//DEBUG:
					nodeBackup = node;

					node->OnNodeClickedEventCallback(*node);
					wasClickedN = true;
					return 0;
				}
			}
			#pragma endregion

			return 0;
		}
		case WM_LBUTTONUP:
		{
			#pragma region TemporaryControlHoldEvent
			wasClickedB = false;
			wasClickedN = false;
			wasClickedT = false;
			#pragma endregion

			MouseButtonReleasedEvent e(MouseButton::Left);
			m_EventCallback(e);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			MouseButtonPressedEvent e(MouseButton::Right);
			m_EventCallback(e);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			MouseButtonReleasedEvent e(MouseButton::Right);
			m_EventCallback(e);
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			MouseButtonPressedEvent e(MouseButton::Middle);
			m_EventCallback(e);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			MouseButtonReleasedEvent e(MouseButton::Middle);
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
	
	NPoint MainWindow::GetPos() const
	{
		RECT rc = GetRect();
		return { (float)rc.left, (float)rc.top };
	}

	NSize MainWindow::GetSize() const
	{
		RECT rc = GetRect();
		return { (float)(rc.left + rc.right), (float)(rc.top + rc.bottom) };
	}
	
	RECT MainWindow::GetRect() const
	{
		RECT rc;
		GetWindowRect(m_hWnd, &rc);
		return rc;
	}

	void MainWindow::RenderLines()
	{
		for (std::pair<Button*, Button*>& line : m_LineCons)
		{
			//TODO: Render only lines that are in the window, but don't check if button is in window because line could go from button left to button right while both buttons are outsise the window
			
			//if (line.first->IsInWindow() || line.second->IsInWindow())
			//{
			float x1 = line.first->GetPos().x + line.first->GetSize().width / 2;
			float y1 = line.first->GetPos().y + line.first->GetSize().height / 2;

			float x2 = line.second->GetPos().x + line.second->GetSize().width / 2;
			float y2 = line.second->GetPos().y + line.second->GetSize().height / 2;

			unsigned int fontsize = (unsigned int)(line.first->GetSize().width / 3);
			if (fontsize == 0) fontsize = 1;

			Renderer2D.RenderLine({ x1, y1 }, { x2, y2 }, { 160.0f, 160.0f, 160.0f }, fontsize);
			//}
		}
	}
}