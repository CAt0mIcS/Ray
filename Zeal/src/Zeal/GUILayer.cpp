#include "pch.h"
#include "GUILayer.h"

#include <Reyal/Debug/ReyalLogger.h>

#include <Reyal/Events/ApplicationEvent.h>
#include <Reyal/Events/KeyboardEvent.h>
#include <Reyal/Events/MouseEvent.h>


namespace Zeal
{
	GUILayer::GUILayer(const std::string_view name)
		: Reyal::Layer(name)
	{
		
	}

	void GUILayer::OnUpdate()
	{

	}
	
	void GUILayer::OnEvent(Reyal::Widget* receiver, Reyal::Event& e)
	{

	}
}


