#ifndef ZGL_GUILAYER_H
#define ZGL_GUILAYER_H

#include "glbase.h"

#include <Reyal/Reyal.h>


namespace At0::Layers
{
	class ZGL_API GUILayer : public Reyal::Layer
	{
	public:
		GUILayer(const std::string_view name);

		virtual void OnMousePress(Reyal::Widget* receiver, Reyal::MouseButtonPressedEvent& e) override;
		virtual void OnMouseRelease(Reyal::Widget* receiver, Reyal::MouseButtonReleasedEvent& e) override;
		virtual void OnMouseWheelUp(Reyal::Widget* receiver, Reyal::MouseWheelUpEvent& e) override;
		virtual void OnMouseWheelDown(Reyal::Widget* receiver, Reyal::MouseWheelDownEvent& e) override;
		virtual void OnMouseWheelLeft(Reyal::Widget* receiver, Reyal::MouseWheelLeftEvent& e) override;
		virtual void OnMouseWheelRight(Reyal::Widget* receiver, Reyal::MouseWheelRightEvent& e) override;
		virtual void OnMouseMove(Reyal::Widget* receiver, Reyal::MouseMoveEvent& e) override;
		virtual void OnHoverEnter(Reyal::Widget* receiver, Reyal::HoverEnterEvent& e) override;
		virtual void OnHoverLeave(Reyal::Widget* receiver, Reyal::HoverLeaveEvent& e) override;
		virtual void OnKeyPress(Reyal::Widget* receiver, Reyal::KeyPressedEvent& e) override;
		virtual void OnKeyRelease(Reyal::Widget* receiver, Reyal::KeyReleasedEvent& e) override;
		virtual void OnChar(Reyal::Widget* receiver, Reyal::CharEvent& e) override;
		virtual void OnResize(Reyal::Widget* receiver, Reyal::WindowResizeEvent& e) override;
		virtual void OnWindowMove(Reyal::Widget* receiver, Reyal::WindowMoveEvent& e) override;
		virtual void OnPaint(Reyal::Widget* receiver, Reyal::PaintEvent& e) override;
		virtual bool OnWindowClose(Reyal::Widget* receiver, Reyal::WindowCloseEvent& e) override;
	};
}

extern "C"
{
	ZGL_API At0::Layers::GUILayer* CreateLayer()
	{
		return new At0::Layers::GUILayer("GUI-Layer");
	}
}

#endif // ZGL_GUILAYER_H
