#include "glpch.h"
#include "GUILayer.h"

#include <iostream>


namespace Zeal::Layers
{
	GUILayer::GUILayer(const std::wstring_view name)
		: Reyal::Layer(name)
	{

	}

	void GUILayer::OnMouseMove(Reyal::Widget* receiver, Reyal::MouseMoveEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] Mouse PosMouse {0}", Reyal::Application::Get()->GetMainWindow().Mouse.GetPos());
		ZL_LOG_DEBUG("[GUILayer] Mouse PosEvent {0}", e.GetPos());
	}
}
