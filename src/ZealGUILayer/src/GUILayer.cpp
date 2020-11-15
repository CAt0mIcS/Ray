#include "glpch.h"
#include "GUILayer.h"

// You need to include this to use the logger
#include <RlDebug/ReyalLogger.h>
#include <RlDebug/Instrumentor.h>

#include <Reyal/Events/MouseEvent.h>
#include <Reyal/Events/ApplicationEvent.h>
#include <Reyal/Events/KeyboardEvent.h>



namespace At0::Layers
{
	GUILayer::GUILayer(const std::string_view name)
		: Reyal::Layer(name)
	{
		RL_PROFILE_FUNCTION();
	}

	void GUILayer::OnMouseMove(Reyal::Widget* receiver, Reyal::MouseMoveEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}
	
	void GUILayer::OnMousePress(Reyal::Widget* receiver, Reyal::MouseButtonPressedEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnMouseRelease(Reyal::Widget* receiver, Reyal::MouseButtonReleasedEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnMouseWheelUp(Reyal::Widget* receiver, Reyal::MouseWheelUpEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnMouseWheelDown(Reyal::Widget* receiver, Reyal::MouseWheelDownEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnResize(Reyal::Widget* receiver, Reyal::WindowResizeEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnWindowMove(Reyal::Widget* receiver, Reyal::WindowMoveEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnPaint(Reyal::Widget* receiver, Reyal::PaintEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnHoverEnter(Reyal::Widget* receiver, Reyal::HoverEnterEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnHoverLeave(Reyal::Widget* receiver, Reyal::HoverLeaveEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnKeyPress(Reyal::Widget* receiver, Reyal::KeyPressedEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnKeyRelease(Reyal::Widget* receiver, Reyal::KeyReleasedEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnChar(Reyal::Widget* receiver, Reyal::CharEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}
	
	bool GUILayer::OnWindowClose(Reyal::Widget* receiver, Reyal::WindowCloseEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
		return false;
	}
}
