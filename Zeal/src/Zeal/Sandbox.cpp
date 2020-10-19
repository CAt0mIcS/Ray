#include "pch.h"
#include "Sandbox.h"

#include "GUILayer.h"


namespace Zeal
{
	Sandbox::Sandbox()
	{
		ZL_PROFILE_FUNCTION();

		m_MainWindow.Show();
		PushLayer(new GUILayer());
	}
	
	Sandbox::~Sandbox()
	{
		ZL_PROFILE_FUNCTION();
	}
	
}
