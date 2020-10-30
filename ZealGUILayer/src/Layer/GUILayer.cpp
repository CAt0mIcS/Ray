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
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}
	
	void GUILayer::OnMousePress(Reyal::Widget* receiver, Reyal::MouseButtonPressedEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnResize(Reyal::Widget* receiver, Reyal::WindowResizeEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnHoverEnter(Reyal::Widget* receiver, Reyal::HoverEnterEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnHoverLeave(Reyal::Widget* receiver, Reyal::HoverLeaveEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}
	
	bool GUILayer::OnWindowClose(Reyal::Widget* receiver, Reyal::WindowCloseEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
		return false;
	}
}
