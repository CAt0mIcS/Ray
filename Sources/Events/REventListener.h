#pragma once

#include "REventDispatcher.h"
#include "../Devices/RWindow.h"


namespace At0::Ray
{
	template<typename T>
	class EventListener
	{
	public:
		template<typename U = T>
		EventListener(typename std::enable_if_t<std::is_base_of_v<EventDispatcher<U>, Window>>* = 0)
		{
			// static_assert(std::is_base_of<EventDispatcher<T>, Window>::value,
			//	"Window does not dispatch specified event.");

			m_Dispatcher = &Window::Get();
			m_Dispatcher->Register(this);
		}

		template<typename U = T>
		EventListener(EventDispatcher<U>& dispatcher)
		{
			m_Dispatcher = &dispatcher;
			m_Dispatcher->Register(this);
		}

		/**
		 * Events of type T will get dispatched to this function
		 */
		virtual void OnEvent(T& e) = 0;

		virtual ~EventListener() { m_Dispatcher->Unregister(this); }

	private:
		EventDispatcher<T>* m_Dispatcher;
	};


	// template<typename T>
	// class EventListener<T,
	// 	typename std::enable_if_t<!std::is_base_of_v<EventDispatcher<T>, Window>>>
	// {
	// public:
	// 	EventListener(EventDispatcher<T>& dispatcher) : m_Dispatcher(&dispatcher)
	// 	{
	// 		m_Dispatcher->Register(this);
	// 	}
	// 	/**
	// 	 * Events of type T will get dispatched to this function
	// 	 */
	// 	virtual void OnEvent(T& e) = 0;

	// 	virtual ~EventListener() { m_Dispatcher->Unregister(this); }

	// private:
	// 	EventDispatcher<T>* m_Dispatcher;
	// };
}  // namespace At0::Ray
