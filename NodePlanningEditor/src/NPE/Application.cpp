#include "pch.h"
#include "pch.h"
#include "Application.h"

#include "Window/MainWindow.h"

#include "NPE/Handlers/Mouse.h"
#include "NPE/Handlers/Keyboard.h"

#include "NPE/Controls/Node.h"


namespace NPE
{
	Application::Application()
		: m_Window({ 800, 600 }, L"NodePlanningEditor", [this](const Event& e) { OnEvent(e); }), m_Zoom(0), m_MousePos{ 0, 0 }
	{
		m_Window.AddControl(new Node(m_Window.Renderer2D, { 10, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }));
		m_Window.AddControl(new Node(m_Window.Renderer2D, { 250, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }));
		m_Window.AddControl(new Node(m_Window.Renderer2D, { 500, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }));
		m_Window.AddControl(new Node(m_Window.Renderer2D, { 750, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }));
		m_Window.AddControl(new Node(m_Window.Renderer2D, { 1000, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }));
		m_Window.AddControl(new Node(m_Window.Renderer2D, { 1250, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }));
		m_Window.AddControl(new Node(m_Window.Renderer2D, { 1500, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }));
		m_Window.AddControl(new Node(m_Window.Renderer2D, { 1750, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }));
		m_Window.AddControl(new Node(m_Window.Renderer2D, { 2000, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }));
		m_Window.AddControl(new Node(m_Window.Renderer2D, { 2250, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }));
		m_Window.AddControl(new Node(m_Window.Renderer2D, { 2500, 350 }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }));
	}

	int Application::Run()
	{
		return m_Window.ProcessMessage();
	}
	
	void Application::OnEvent(const Event& e)
	{
		MoveNodes(e);
		Zoom(e);
		OnPaint(e);

		//TODO: Implement functions bellow this point
		Resize(e);
		DrawLine(e);
	}

	void Application::DrawLine(const Event& e)
	{
		if (e.GetType() == EventType::MouseButtonPressedEvent && Mouse::IsLeftPressed())
		{
			for (const auto* control : m_Window.GetControls())
			{
				for (const auto* child : control->GetChildren())
				{
					if (Mouse::IsOnControl(child))
					{
						MessageBox(NULL, L"Button clicked", L"", NULL);
					}
				}
				
			}
		}
		
		
	}

	void Application::Resize(const Event& e)
	{

	}

	void Application::OnPaint(const Event& e)
	{
		if (e.GetType() == EventType::AppPaintEvent)
		{
			m_Window.Renderer2D.BeginDraw();
			m_Window.Renderer2D.RenderScene(NColor{ 35.0f, 38.0f, 40.0f });
			for (auto* control : m_Window.GetControls())
				control->Render();
			m_Window.Renderer2D.EndDraw();
		}
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

			int mBuff = 0;
			if (diff.x > mBuff || diff.y > mBuff || diff.x < -mBuff || diff.y < -mBuff)
			{
				m_MousePos.x = Mouse::GetPos().x;
				m_MousePos.y = Mouse::GetPos().y;

				m_Window.Renderer2D.BeginDraw();
				m_Window.Renderer2D.RenderScene({ 35.0f, 38.0f, 40.0f });
				for (auto* control : m_Window.GetControls())
				{
					control->MoveBy(diff);
					control->Render();
				}
				m_Window.Renderer2D.EndDraw();
			}
		}
	}

	void Application::Zoom(const Event& e)
	{
		NPoint screenCenter = Mouse::GetPos();

		if (e.GetType() == EventType::MouseWheelUpEvent)
		{
			++m_Zoom;

			m_Window.Renderer2D.BeginDraw();
			m_Window.Renderer2D.RenderScene({ 35.0f, 38.0f, 40.0f });
			for (auto* control : m_Window.GetControls())
			{
				const NPoint& pos = control->GetPos();
				const NSize& size = control->GetSize();
				NPoint newPos = screenCenter - pos;
				NSize newSize;
				newPos.x *= -m_ZoomFactor;
				newPos.y *= -m_ZoomFactor;

				newSize.width = size.width * m_ResizeFactor;
				newSize.height = size.height * m_ResizeFactor;
				control->MoveBy(newPos);
				control->ResizeTo(newSize);
				control->Render();
			}
			m_Window.Renderer2D.EndDraw();
		}
		else if (e.GetType() == EventType::MouseWheelDownEvent)
		{
			--m_Zoom;
			if (m_Zoom <= -m_ZoomBoundary)
			{
				m_Zoom = -m_ZoomBoundary;
				return;
			}

			m_Window.Renderer2D.BeginDraw();
			m_Window.Renderer2D.RenderScene({ 35.0f, 38.0f, 40.0f });
			for(auto* control : m_Window.GetControls())
			{
				const NPoint& pos = control->GetPos();
				const NSize& size = control->GetSize();
				NPoint newPos = screenCenter - pos;
				NSize newSize;
				newPos.x *= m_ZoomFactor;
				newPos.y *= m_ZoomFactor;

				newSize.width = size.width / m_ResizeFactor;
				newSize.height = size.height / m_ResizeFactor;
				control->MoveBy(newPos);
				control->ResizeTo(newSize);
				control->Render();
			}
			m_Window.Renderer2D.EndDraw();
		}
	}
}


