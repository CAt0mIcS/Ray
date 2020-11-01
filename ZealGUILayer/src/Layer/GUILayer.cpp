#include "glpch.h"
#include "GUILayer.h"

#include <iostream>


namespace At0::Layers
{
	GUILayer::GUILayer(const std::wstring_view name)
		: _In_ Reyal::Layer(name)
	{

	}

	void GUILayer::OnMouseMove(_In_ Reyal::Widget* receiver, Reyal::MouseMoveEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}
	
	void GUILayer::OnMousePress(_In_ Reyal::Widget* receiver, Reyal::MouseButtonPressedEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnMouseRelease(_In_ Reyal::Widget* receiver, Reyal::MouseButtonReleasedEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnMouseWheelUp(_In_ Reyal::Widget* receiver, Reyal::MouseWheelUpEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnMouseWheelDown(_In_ Reyal::Widget* receiver, Reyal::MouseWheelDownEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnResize(_In_ Reyal::Widget* receiver, Reyal::WindowResizeEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnWindowMove(_In_ Reyal::Widget* receiver, Reyal::WindowMoveEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnPaint(_In_ Reyal::Widget* receiver, Reyal::PaintEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnHoverEnter(_In_ Reyal::Widget* receiver, Reyal::HoverEnterEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnHoverLeave(_In_ Reyal::Widget* receiver, Reyal::HoverLeaveEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnKeyPress(_In_ Reyal::Widget* receiver, Reyal::KeyPressedEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnKeyRelease(_In_ Reyal::Widget* receiver, Reyal::KeyReleasedEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnChar(_In_ Reyal::Widget* receiver, Reyal::CharEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}
	
	bool GUILayer::OnWindowClose(_In_ Reyal::Widget* receiver, Reyal::WindowCloseEvent& e)
	{
		ZL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
		return false;
	}
}
