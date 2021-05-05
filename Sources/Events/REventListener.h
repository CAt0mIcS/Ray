#pragma once

#include "REventDispatcher.h"
#include "../Devices/RWindow.h"

#include <type_traits>
#include <concepts>


namespace At0::Ray
{
	template<typename T>
	class EventListener
	{
	public:
		/**
		 * Registers the listener to receive event of type T.
		 * If the dispatcher<T> is a base class of the Window then the window is selected
		 * automatically as the dispatcher
		 */
		template<typename U = T>
		EventListener() requires std::derived_from<Window, EventDispatcher<U>>
		{
			m_Dispatcher = &Window::Get();
			m_Dispatcher->Register(this);
		}

		/**
		 * Registers the listener to receive event of type T.
		 * If the dispatcher<T> is not a base class of the Window the dispatcher<T> needs to be
		 * specified
		 */
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

		EventListener<T>& operator=(EventListener<T>&& other)
		{
			m_Dispatcher = std::move(other.m_Dispatcher);

			// Register again because deconstructore unregisters (RAY_TODO)
			m_Dispatcher->Register(this);
			return *this;
		}

		EventListener(EventListener<T>&& other) { *this = std::move(other); }

		EventListener<T>& operator=(const EventListener<T>& other)
		{
			m_Dispatcher = other.m_Dispatcher;

			// Register again because deconstructore unregisters (RAY_TODO)
			m_Dispatcher->Register(this);
			return *this;
		}
		EventListener(const EventListener<T>& other) { *this = other; }

	private:
		EventDispatcher<T>* m_Dispatcher;
	};
}  // namespace At0::Ray
