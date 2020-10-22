#include "pch.h"
#include "GUILayer.h"

#include <Reyal/Debug/ReyalLogger.h>

#include <Reyal/Events/ApplicationEvent.h>
#include <Reyal/Events/KeyboardEvent.h>
#include <Reyal/Events/MouseEvent.h>

#include <Reyal/Window/Window.h>


namespace Zeal
{
	GUILayer::GUILayer(const std::wstring_view name, Reyal::Window& win)
		: Reyal::Layer(name), m_Win(win)
	{
		ZL_PROFILE_FUNCTION();


	}

	void GUILayer::OnUpdate()
	{

	}
	
	void GUILayer::OnEvent(Reyal::Widget* receiver, Reyal::Event& e)
	{

	}
}


