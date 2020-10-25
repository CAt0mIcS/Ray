#pragma once

#include "RlBase.h"


namespace Zeal::Reyal
{
	class Event;
	class Widget;

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

		virtual void OnMousePress(_In_ Widget* receiver, Event& e) {}
		virtual void OnMouseRelease(_In_ Widget* receiver, Event& e) {}
		virtual void OnMouseWheelUp(_In_ Widget* receiver, Event& e) {}
		virtual void OnMouseWheelDown(_In_ Widget* receiver, Event& e) {}
		virtual void OnMouseMove(_In_ Widget* receiver, Event& e) {}
		virtual void OnHoverEnter(_In_ Widget* receiver, Event& e) {}
		virtual void OnHoverLeave(_In_ Widget* receiver, Event& e) {}
		virtual void OnKeyPress(_In_ Widget* receiver, Event& e) {}
		virtual void OnKeyRelease(_In_ Widget* receiver, Event& e) {}
		virtual void OnChar(_In_ Widget* receiver, Event& e) {}
		virtual void OnResize(_In_ Widget* receiver, Event& e) {}
		virtual void OnWindowMove(_In_ Widget* receiver, Event& e) {}
		virtual void OnPaint(_In_ Widget* receiver, Event& e) {}
	protected:
	};
	
}



