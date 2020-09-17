#include "pch.h"
#include "Application.h"

#include "GUI/Events/ApplicationEvent.h"
#include "GUI/Events/MouseEvent.h"

#include "GUI/Controls/TextBox.h"

#include "GUI/Handlers/Mouse.h"


struct UI
{
	GUI::TextBox* txtbox;
} ui;


namespace NPE
{
	Application::Application()
		: m_FileHandler("saves\\save.dbs"), m_MousePos{ 0.0f, 0.0f }
	{
		m_FileHandler.LoadScene(this->GetWindow());
		InstallEventFilter([this](GUI::Control* watched, GUI::Event& e) { return OnEvent(watched, e); });
	
		ui.txtbox = (GUI::TextBox*)m_Window.AddControl(new GUI::TextBox(&m_Window));
		ui.txtbox->SetText(L"Hello World");
		ui.txtbox->SetColor({ 15, 17, 19 });
		ui.txtbox->SetFontFamily(L"Consolas");
		ui.txtbox->SetFontSize(20);
		ui.txtbox->SetPos({ 250, 250 });
		ui.txtbox->SetSize({ 400, 550 });
		ui.txtbox->SetMultiline(true);
	
	}

	bool Application::OnEvent(GUI::Control* watched, GUI::Event& e)
	{
		switch (e.GetType())
		{
		case GUI::EventType::AppPaintEvent:
		{
			return OnPaintEvent((GUI::PaintEvent&)e);
		}
		case GUI::EventType::MouseMoveEvent:
		{
			return OnMouseMove((GUI::MouseMoveEvent&)e);
		}
		case GUI::EventType::MouseButtonPressedEvent:
		{
			return OnMouseButtonPressed((GUI::MouseButtonPressedEvent&)e);
		}
		case GUI::EventType::TimerEvent:
		{
			std::cout << "H\n";
			return true;
		}
		}
		return false;
	}

	bool Application::OnMouseMove(GUI::MouseMoveEvent& e)
	{
		if (GUI::Mouse::IsMiddlePressed())
		{
			Util::NPoint diff = GUI::Mouse::GetPos() - m_MousePos;
			m_MousePos = GUI::Mouse::GetPos();

			GUI::Renderer::Get().BeginDraw();
			GUI::Renderer::Get().RenderScene();
			for (auto* control : m_Window.GetControls())
			{
				control->MoveBy(diff);
				control->Render();
			}

			GUI::Renderer::Get().EndDraw();
			return true;
		}
		return false;
	}

	bool Application::OnMouseButtonPressed(GUI::MouseButtonPressedEvent& e)
	{
		if (e.GetButton() == GUI::MouseButton::Middle)
		{
			m_MousePos = GUI::Mouse::GetPos();
			return true;
		}
		if (e.GetButton() == GUI::MouseButton::Left)
		{
			m_Window.CreateTimer(1000, true);
		}
		return false;
	}

	bool Application::OnPaintEvent(GUI::PaintEvent& e)
	{
		GUI::Renderer& renderer = GUI::Renderer::Get();
	
		renderer.BeginDraw();
		renderer.RenderScene();

		for (auto* control : m_Window.GetChildren())
		{
			control->Render();
		}

		renderer.EndDraw();
		
		return true;
	}

}


