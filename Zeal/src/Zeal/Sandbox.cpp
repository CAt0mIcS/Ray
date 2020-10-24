#include "pch.h"
#include "Sandbox.h"


//QUESTION: When entry point is included in Reyal.h
//#include <Reyal/Reyal.h>
#include <Reyal/EntryPoint.h>

#include "GUILayer.h"

namespace Zeal
{
	Sandbox::Sandbox()
	{
		ZL_PROFILE_FUNCTION();

		m_MainWindow.SetTitle(L"Zeal");
		m_MainWindow.Show();
		PushLayer(new GUILayer(L"GUI-Layer"));
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
