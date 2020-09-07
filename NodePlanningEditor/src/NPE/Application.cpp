#include "pch.h"
#include "Application.h"


namespace NPE
{
	Application::Application()
		: m_Database("saves\\save.dbs", 3)
	{
		//LoadFile();
		InstallEventFilter([this](GUI::Control* watched, GUI::Event& e) { return OnEvent(watched, e); });
	}

	bool Application::OnEvent(GUI::Control* watched, GUI::Event& e)
	{
		if (e.GetType() == GUI::EventType::AppPaintEvent)
		{
			GUI::Renderer::Get().BeginDraw();
			GUI::Renderer::Get().RenderScene();
			GUI::Renderer::Get().EndDraw();
			return true;
		}
		return false;
	}

	Application::~Application()
	{
		//SaveFile();
	}

}


