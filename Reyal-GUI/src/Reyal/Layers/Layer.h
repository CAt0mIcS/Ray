#ifndef RL_LAYER_H
#define RL_LAYER_H

#include "RlBase.h"

#include "Reyal/Events/ApplicationEvent.h"
#include "Reyal/Events/KeyboardEvent.h"
#include "Reyal/Events/MouseEvent.h"

#include "Reyal/Widgets/ZWidget.h"


namespace At0::Reyal
{
	class RL_API Layer
	{
	public:
		/// <summary>
		/// Layer Constructor
		/// </summary>
		/// <param name="name">Is the unique name of this Layer</param>
		Layer(const std::wstring_view name);

		/// <summary>
		/// Function which is called every frame
		/// </summary>
		virtual void OnUpdate() { };

		virtual void OnMousePress(_In_ Widget* receiver, MouseButtonPressedEvent& e) {}
		virtual void OnMouseRelease(_In_ Widget* receiver, MouseButtonReleasedEvent& e) {}
		virtual void OnMouseWheelUp(_In_ Widget* receiver, MouseWheelUpEvent& e) {}
		virtual void OnMouseWheelDown(_In_ Widget* receiver, MouseWheelDownEvent& e) {}
		virtual void OnMouseMove(_In_ Widget* receiver, MouseMoveEvent& e) {}
		virtual void OnHoverEnter(_In_ Widget* receiver, HoverEnterEvent& e) {}
		virtual void OnHoverLeave(_In_ Widget* receiver, HoverLeaveEvent& e) {}
		virtual void OnKeyPress(_In_ Widget* receiver, KeyPressedEvent& e) {}
		virtual void OnKeyRelease(_In_ Widget* receiver, KeyReleasedEvent& e) {}
		virtual void OnChar(_In_ Widget* receiver, CharEvent& e) {}
		virtual void OnResize(_In_ Widget* receiver, WindowResizeEvent& e) {}
		virtual void OnWindowMove(_In_ Widget* receiver, WindowMoveEvent& e) {}
		virtual void OnPaint(_In_ Widget* receiver, PaintEvent& e) {}
	
	private:
		std::wstring m_Name;
	};
	
}


#endif /* RL_LAYER_H */

