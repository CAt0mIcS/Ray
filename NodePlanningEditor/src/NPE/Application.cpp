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
	
	NPoint startPos{ 0 };
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
						Node* node = (Node*)&control;
						node->AdjustPos(diff);
					}
				}
			}
		}
	}

	void Application::Zoom(const Mouse::Event& e)
	{
		if (e.GetType() == Mouse::Event::Type::WheelUp)
		{

		}
		else if (e.GetType() == Mouse::Event::Type::WheelDown)
		{

		}
	}
}


