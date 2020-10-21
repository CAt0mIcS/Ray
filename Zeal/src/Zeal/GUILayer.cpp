#include "pch.h"
#include "GUILayer.h"

#include <Reyal/Debug/ReyalLogger.h>

#include <Reyal/Events/ApplicationEvent.h>
#include <Reyal/Events/KeyboardEvent.h>
#include <Reyal/Events/MouseEvent.h>

#include <Reyal/Window/Window.h>


namespace Zeal
{
	GUILayer::GUILayer(const std::wstring_view name)
		: Reyal::Layer(name)
	{
		Reyal::Window* win = new Reyal::Window(L"H", L"Title");
		AddChild(win);
	}

	void GUILayer::OnUpdate()
	{
		auto winChild = FindChild(L"H");
		auto str = ((Reyal::Window*)winChild)->GetTitle();
		ZL_LOG_DEBUG("{0}", str);
	}
	
	void GUILayer::OnEvent(Reyal::Widget* receiver, Reyal::Event& e)
	{

	}
}


