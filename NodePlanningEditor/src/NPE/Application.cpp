#include "pch.h"
#include "pch.h"
#include "Application.h"

#include "Window/MainWindow.h"

#include "NPE/Handlers/Mouse.h"
#include "NPE/Handlers/Keyboard.h"


namespace NPE
{
	Application::Application()
		: m_Window(800, 600, L"NodePlanningEditor", [this](const Event& e) { OnEvent(e); }), m_Zoom(0), m_MousePos{ 0, 0 }
	{
	}

	int Application::Run()
	{
		return m_Window.ProcessMessage();
	}
	
	void Application::OnEvent(const Event& e)
	{
		MoveNodes(e);
		Zoom(e);
	}

	void Application::MoveNodes(const Event& e)
	{
		if (e.GetType() == EventType::MouseButtonPressedEvent && Mouse::IsMiddlePressed())
		{
			m_MousePos = Mouse::GetPos();
		}

		else if (e.GetType() == EventType::MouseMoveEvent && Mouse::IsMiddlePressed())
		{
			NPoint diff{};
			diff.x = Mouse::GetPos().x - m_MousePos.x;
			diff.y = Mouse::GetPos().y - m_MousePos.y;

			int mBuff = 10;
			if (diff.x > mBuff || diff.y > mBuff || diff.x < -mBuff || diff.y < -mBuff)
			{
				m_MousePos.x = Mouse::GetPos().x;
				m_MousePos.y = Mouse::GetPos().y;

				for (auto& control : m_Window.GetControls())
				{
					control.MoveBy(diff);
					//InvalidateRect(control.GetNativeWindow(), nullptr, FALSE);
					//SendMessage(control.GetNativeWindow(), WM_PAINT, control.GetSize().width, control.GetSize().height);
					UpdateWindow(control.GetNativeWindow());
				}
			}
		}
	}

	void Application::Zoom(const Event& e)
	{
		NPoint screenCenter = Mouse::GetPos();

		if (e.GetType() == EventType::MouseWheelUpEvent)
		{
			++m_Zoom;

			for (auto& control : m_Window.GetControls())
			{
				const NPoint& pos = control.GetPos();
				const NSize& size = control.GetSize();
				NPoint newPos = screenCenter - pos;
				NSize newSize;
				newPos.x *= -m_ZoomFactor;
				newPos.y *= -m_ZoomFactor;

				newSize.width = size.width * m_ResizeFactor;
				newSize.height = size.height * m_ResizeFactor;
				control.MoveBy(newPos);
				control.ResizeTo(newSize);
				//InvalidateRgn(control.GetNativeWindow(), NULL, FALSE);
				UpdateWindow(control.GetNativeWindow());
			}
		}
		else if (e.GetType() == EventType::MouseWheelDownEvent)
		{
			--m_Zoom;
			if (m_Zoom <= -m_ZoomBoundary)
			{
				m_Zoom = -m_ZoomBoundary;
				return;
			}

			for(auto& control : m_Window.GetControls())
			{
				const NPoint& pos = control.GetPos();
				const NSize& size = control.GetSize();
				NPoint newPos = screenCenter - pos;
				NSize newSize;
				newPos.x *= m_ZoomFactor;
				newPos.y *= m_ZoomFactor;

				newSize.width = size.width / m_ResizeFactor;
				newSize.height = size.height / m_ResizeFactor;
				control.MoveBy(newPos);
				control.ResizeTo(newSize);
				//InvalidateRgn(control.GetNativeWindow(), NULL, FALSE);
				UpdateWindow(control.GetNativeWindow());
			}
		}
	}
}


