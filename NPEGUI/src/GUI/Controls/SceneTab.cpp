#include "pch.h"
#include "SceneTab.h"

#include "GUI/Graphics/TextRenderer.h"
#include "GUI/Handlers/Mouse.h"

#include "GUI/Events/MouseEvent.h"

namespace GUI
{ 
	SceneTab::SceneTab(Control* parent)
		: GUI::Control(Control::Type::Tab, parent), m_IsActive(false), m_Text{}
	{

	}
	
	bool SceneTab::Render()
	{
		if (Mouse::IsOnControl(this) || IsActive())
			Renderer::Get().RenderRect(m_Pos, m_Size, Constants::g_DefaultActiveSceneTabColor);
		else
			Renderer::Get().RenderRect(m_Pos, m_Size, Constants::g_DefaultInactiveSceneTabColor);

		RenderText();

		return true;
	}
	
	bool SceneTab::OnEvent(Event& e)
	{
		if (e.GetType() == EventType::MouseMoveEvent)
		{
			// Mouse is on this SceneTab --> Issue rendering call to draw the tab in the active color
			PostRedraw();
			return true;
		}
		return false;
	}
	
	void SceneTab::RenderText() const
	{
		TextRenderer::Get().RenderText(m_Text);
	}
}


