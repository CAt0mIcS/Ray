#include "pch.h"
#include "GUIApplication.h"


namespace GUI
{
	int GUIApplication::Run()
	{
		return m_Window.ProcessMessage();
	}

	GUIApplication::GUIApplication()
	{
		//set default event filter
		InstallEventFilter([](Control* watched, Event& e) { return false; });
	}
}


