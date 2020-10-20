#include "pch.h"
#include "Sandbox.h"

#include "GUILayer.h"


namespace Zeal
{
	Sandbox::Sandbox()
	{
		ZL_PROFILE_FUNCTION();

		m_MainWindow.Show();
		PushLayer(new GUILayer("GUI-Layer"));
	}
	
	Sandbox::~Sandbox()
	{
		ZL_PROFILE_FUNCTION();
	}

	bool Sandbox::OnEvent(Reyal::Widget* receiver, Reyal::Event& e)
	{
		ZL_PROFILE_FUNCTION();
		return false;
	}
	
}
