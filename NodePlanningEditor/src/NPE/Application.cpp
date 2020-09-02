#include "pch.h"
#include "Application.h"

#include "Window/MainWindow.h"


namespace NPE
{
	Application::Application()
		: m_Window(800, 600, L"NodePlanningEditor"), m_Zoom(0), m_MousePos{ 0, 0 }
	{
	}

	int Application::Run()
	{
		return m_Window.ProcessMessage([this]() { OnMessage(); });
	}
	
	void Application::OnMessage()
	{
		const auto e = m_Window.Mouse.GetEvent();
		if (e.IsValid())
		{
			MoveNodes(e);
			Zoom(e);
		}
	}

	void Application::MoveNodes(const Mouse::Event& e)
	{
		if (e.GetType() == Mouse::Event::Type::MWheelPress)
		{
			m_MousePos = e.GetPos();
		}

		else if (e.GetType() == Mouse::Event::Type::Move && e.IsMiddlePressed())
		{

			NPoint diff{};
			diff.x = e.GetPos().x - m_MousePos.x;
			diff.y = e.GetPos().y - m_MousePos.y;

			int mBuff = 10;
			if (diff.x > mBuff || diff.y > mBuff || diff.x < -mBuff || diff.y < -mBuff)
			{
				m_MousePos.x = e.GetPos().x;
				m_MousePos.y = e.GetPos().y;

				for (auto& control : m_Window.GetControls())
				{
					control.MoveBy(diff);
				}
				InvalidateRect(m_Window.GetNativeWindow(), NULL, FALSE);
				UpdateWindow(m_Window.GetNativeWindow());
			}
		}
	}

	void Application::Zoom(const Mouse::Event& e)
	{
		NPoint screenCenter = e.GetPos();
		constexpr int zoomBoundary = 20;

		if (e.GetType() == Mouse::Event::Type::WheelUp)
		{
			++m_Zoom;
			if (m_Zoom >= zoomBoundary)
			{
				m_Zoom = zoomBoundary;
				return;
			}

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

				InvalidateRect(m_Window.GetNativeWindow(), NULL, FALSE);
				UpdateWindow(m_Window.GetNativeWindow());
			}
		}
		else if (e.GetType() == Mouse::Event::Type::WheelDown)
		{
			--m_Zoom;
			if (m_Zoom <= -zoomBoundary)
			{
				m_Zoom = -zoomBoundary;
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

				InvalidateRect(m_Window.GetNativeWindow(), NULL, FALSE);
				UpdateWindow(m_Window.GetNativeWindow());
			}
		}
	}
}


