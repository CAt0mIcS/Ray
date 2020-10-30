#ifndef ZGL_GUILAYER_H
#define ZGL_GUILAYER_H

#include "glbase.h"

#include <Reyal/Reyal.h>

// You need to include this to use the logger
#include <Reyal/Debug/ReyalLogger.h>

namespace At0::Layers
{
	class ZGL_API GUILayer : public Reyal::Layer
	{
	public:
		GUILayer(const std::wstring_view name);

		virtual void OnMouseMove(Reyal::Widget* receiver, Reyal::MouseMoveEvent& e) override;
		virtual void OnResize(Reyal::Widget* receiver, Reyal::WindowResizeEvent& e) override;
		virtual void OnMousePress(Reyal::Widget* receiver, Reyal::MouseButtonPressedEvent& e) override;

		virtual void OnHoverEnter(Reyal::Widget* receiver, Reyal::HoverEnterEvent& e) override;
		virtual void OnHoverLeave(Reyal::Widget* receiver, Reyal::HoverLeaveEvent& e) override;
	};
}

extern "C"
{
	ZGL_API At0::Layers::GUILayer* CreateLayer()
	{
		return new At0::Layers::GUILayer(L"GUI-Layer");
	}
}

#endif /* ZGL_GUILAYER_H */
