#pragma once

#include "REventDispatcher.h"


namespace At0::Ray
{
	class Widget;

	template<typename E>
	class EventListener
	{
	public:
		virtual void OnEvent(Ray::Widget* receiver, E& e) = 0;

	protected:
		EventListener(EventDispatcher<E>& dispatcher)
		{
			dispatcher.AddListener(this);
		}
	};
}