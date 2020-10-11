#include "pch.h"
#include "SceneTab.h"

#include "GUI/Graphics/TextRenderer.h"


namespace NPE
{ 
	SceneTab::SceneTab(GUI::Control* parent)
		: GUI::Control(GUI::Control::Type::Tab, parent)
	{

	}
	
	bool SceneTab::Render()
	{
		return false;
	}
	
	bool SceneTab::OnEvent(GUI::Event& e)
	{
		return false;
	}
}


