#include "glpch.h"
#include "GUILayer.h"

#include <iostream>


namespace At0::Layers
{
	GUILayer::GUILayer(const std::wstring_view name)
		: Reyal::Layer(name)
	{

	}

	void GUILayer::OnMouseMove(Reyal::Widget* receiver, Reyal::MouseMoveEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] Mouse PosEvent {0}", e.GetPos());
	}
	
	void GUILayer::OnMousePress(Reyal::Widget* receiver, Reyal::MouseButtonPressedEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] MBP PosMouse {0}", Reyal::Application::Get().GetMainWindow().Mouse.GetPos());
	}

	void GUILayer::OnResize(Reyal::Widget* receiver, Reyal::WindowResizeEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] WindowResizeEvent {0}", e.GetSize());
	}

	void GUILayer::OnHoverEnter(Reyal::Widget* receiver, Reyal::HoverEnterEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] Widget {0} gained hovering focus", receiver->GetName());
	}

	void GUILayer::OnHoverLeave(Reyal::Widget* receiver, Reyal::HoverLeaveEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] Widget {0} lost hovering focus", receiver->GetName());
	}
}