#include "pch.h"
#include "pch.h"
#include "Application.h"

#include "Window/MainWindow.h"

#include "NPE/Handlers/Mouse.h"
#include "NPE/Handlers/Keyboard.h"


namespace NPE
{
	Application::Application()
		: m_Window(800, 600, L"NodePlanningEditor", [this](const Event& e) { OnEvent(e); }), m_Zoom(0), m_MousePos{ 0, 0 }, m_Futures{}
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
					//InvalidateRgn(control.GetNativeWindow(), NULL, FALSE);
					SendMessage(control.GetNativeWindow(), WM_PAINT, control.GetSize().width, control.GetSize().height);
				}
			}
		}
	}

	static void Calc(Control* control, float zoomFactor, float resizeFactor)
	{

	}

	void Application::Zoom(const Event& e)
	{
		NPoint screenCenter = Mouse::GetPos();

		if (e.GetType() == EventType::MouseWheelUpEvent)
		{
			++m_Zoom;
			if (m_Zoom >= m_ZoomBoundary)
			{
				m_Zoom = m_ZoomBoundary;
				return;
			}

			for (auto& control : m_Window.GetControls())
			{
				m_Futures.push_back(std::async(std::launch::async, [](Control* control, const float zoomFactor, const float resizeFactor) 
					{
						NPoint screenCenter = Mouse::GetPos();
						const NPoint& pos = control->GetPos();
						const NSize& size = control->GetSize();
						NPoint newPos = screenCenter - pos;
						NSize newSize;
						newPos.x *= -zoomFactor;
						newPos.y *= -zoomFactor;

						newSize.width = size.width * resizeFactor;
						newSize.height = size.height * resizeFactor;
						control->MoveBy(newPos);
						control->ResizeTo(newSize);
						SendMessage(control->GetNativeWindow(), WM_THREAD_PAINT_UPDATE, control->GetSize().width, control->GetSize().height);
					}, &control, m_ZoomFactor, m_ResizeFactor));
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
				m_Futures.push_back(std::async(std::launch::async, [](Control* control, const float zoomFactor, const float resizeFactor) 
					{
						NPoint screenCenter = Mouse::GetPos();
						const NPoint& pos = control->GetPos();
						const NSize& size = control->GetSize();
						NPoint newPos = screenCenter - pos;
						NSize newSize;
						newPos.x *= zoomFactor;
						newPos.y *= zoomFactor;

						newSize.width = size.width / resizeFactor;
						newSize.height = size.height / resizeFactor;
						control->MoveBy(newPos);
						control->ResizeTo(newSize);
						SendMessage(control->GetNativeWindow(), WM_THREAD_PAINT_UPDATE, control->GetSize().width, control->GetSize().height);
					}, &control, m_ZoomFactor, m_ResizeFactor));
			}
		}
	}
}


