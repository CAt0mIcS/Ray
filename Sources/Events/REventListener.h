#pragma once

#include "REventDispatcher.h"
#include "../Devices/RWindow.h"


namespace At0::Ray
{
	template<typename T>
	class EventListener
	{
	public:
		EventListener(EventDispatcher<T>& dispatcher = Window::Get()) : m_Dispatcher(dispatcher)
		{
			m_Dispatcher.Register(this);
		}

		/**
		 * Events of type T will get dispatched to this function
		 */
		virtual void OnEvent(T& e) = 0;

		virtual ~EventListener() { m_Dispatcher.Unregister(this); }

	private:
		EventDispatcher<T>& m_Dispatcher;
	};
}  // namespace At0::Ray
