#pragma once

#include "EventDispatcher.h"


namespace At0::Reyal
{
	class Widget;

	template<typename E>
	class EventListener
	{
	public:
		virtual void OnEvent(Reyal::Widget* receiver, E& e) = 0;

	protected:
		EventListener(EventDispatcher<E>& dispatcher)
		{
			dispatcher.AddListener(this);
		}
	};
}