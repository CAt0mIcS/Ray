#include "pch.h"
#include "Application.h"


namespace NPE
{
	Application::Application()
		: m_FileHandler("saves\\save.dbs")
	{
		m_FileHandler.LoadScene(this->GetWindow());
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

}


