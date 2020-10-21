#include "pch.h"
#include "Sandbox.h"

#include "GUILayer.h"


namespace Zeal
{
	Sandbox::Sandbox()
	{
		ZL_PROFILE_FUNCTION();

		m_MainWindow.Show();
		PushLayer(new GUILayer(L"GUI-Layer", m_MainWindow.GetRenderer()));
	}
	
	Sandbox::~Sandbox()
	{
		ZL_PROFILE_FUNCTION();
	}

	void Sandbox::OnEvent(Reyal::Widget* receiver, Reyal::Event& e)
	{
		ZL_PROFILE_FUNCTION();

	}

}
