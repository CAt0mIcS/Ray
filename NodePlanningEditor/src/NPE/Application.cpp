#include "pch.h"
#include "Application.h"

#include "GUI/Events/ApplicationEvent.h"

#include "GUI/Controls/MenuControl.h"

namespace NPE
{
	GUI::MenuItem* item;
	Application::Application()
		: m_FileHandler("saves\\save.dbs")
	{
		m_FileHandler.LoadScene(this->GetWindow());
		InstallEventFilter([this](GUI::Control* watched, GUI::Event& e) { return OnEvent(watched, e); });

		GUI::MenuBar* mb = (GUI::MenuBar*)m_Window.AddControl(new GUI::MenuBar(&m_Window));
		
		item = new GUI::MenuItem(&m_Window, "Hello");
		mb->AddItem(item);
	}

	bool Application::OnEvent(GUI::Control* watched, GUI::Event& e)
	{
		switch (e.GetType())
		{
		case GUI::EventType::AppPaintEvent:
		{
			return OnPaintEvent((GUI::PaintEvent&)e);
		}
		case GUI::EventType::MouseButtonPressedEvent:
		{
			if (watched == item)
				MessageBox(NULL, L"CLICKED", L"", NULL);
		}
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


