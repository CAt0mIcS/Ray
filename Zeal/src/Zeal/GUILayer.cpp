#include "pch.h"
#include "GUILayer.h"

#include <Reyal/Reyal.h>


namespace Zeal
{

	GUILayer::GUILayer(const std::wstring_view name)
		: Reyal::Layer(name)
	{
		ZL_PROFILE_FUNCTION();
		//QUESTION:
		//Reyal::Application::Get()->GetMainWindow()->Close();
	}

	void GUILayer::OnUpdate()
	{

	}
	
	void GUILayer::OnEvent(Reyal::Widget* receiver, Reyal::Event& e)
	{

	}
}


