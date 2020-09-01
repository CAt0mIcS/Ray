#include "pch.h"
#include "Application.h"

#include "Window/MainWindow.h"


namespace NPE
{
	Application::Application()
		: m_Window(800, 600, L"NodePlanningEditor"), m_MousePos{ 0, 0 }
	{
	}

	int Application::Run()
	{
		return MainWindow::ProcessMessage([this]() { OnMessage(); });
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

			int mBuff = 5;
			if (diff.x > mBuff || diff.y > mBuff || diff.x < -mBuff || diff.y < -mBuff)
			{
				m_MousePos.x = e.GetPos().x;
				m_MousePos.y = e.GetPos().y;

				for (auto& control : m_Window.GetControls())
				{
					if (control.GetType() == Control::Type::Node)
					{
						control.MoveBy(diff);
					}
				}
			}
		}
	}

	float zoom = 1.0f;
	void Application::Zoom(const Mouse::Event& e)
	{
		const float zoomFactor = 0.05f;
		const float zoomFacTrue = 1.05f;
		NPoint screenCenter = { GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2 };

		if (e.GetType() == Mouse::Event::Type::WheelUp)
		{			
			for (auto& control : m_Window.GetControls())
			{
				const NPoint& pos = control.GetPos();
				const NSize& size = control.GetSize();

				zoom *= zoomFacTrue;
				
				NPoint newPos;
				newPos = (screenCenter - pos);
				newPos.x *= -zoomFactor;
				newPos.y *= -zoomFactor;
				
				NSize newSize;
				newSize.width = size.width * zoomFacTrue;
				newSize.height = size.height * zoomFacTrue;

				control.MoveBy(newPos);
				control.ResizeTo(newSize);
			}

		}
		else if (e.GetType() == Mouse::Event::Type::WheelDown)
		{
			// Move objects towards origin (middle) when zooming out
			for (auto& control : m_Window.GetControls())
			{
				const NPoint& pos = control.GetPos();
				const NSize& size = control.GetSize();
				
				zoom /= zoomFacTrue;
				
				NPoint newPos;
				newPos = (screenCenter - pos);
				newPos.x *= zoomFactor;
				newPos.y *= zoomFactor;
				
				NSize newSize;
				newSize.width = size.width / zoomFacTrue;
				newSize.height = size.height / zoomFacTrue;

				control.MoveBy(newPos);
				control.ResizeTo(newSize);
			}
		}
	}
}


