#include "pch.h"
#include "GUILayer.h"

#include <Reyal/Reyal.h>


namespace Zeal
{

	GUILayer::GUILayer(const std::wstring_view name)
		: Reyal::Layer(name)
	{
		ZL_PROFILE_FUNCTION();
		// Function in base class to close main window
		//Reyal::Application::Get()->GetMainWindow()->Close();
	}

	void GUILayer::OnUpdate()
	{

	}
	
	
}


