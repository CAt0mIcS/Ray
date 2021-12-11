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
			RegisterForDispatcher(&Window::Get());
		}

		/**
		 * Registers the listener to receive event of type T.
		 * If the dispatcher<T> is not a base class of the Window the dispatcher<T> needs to be
		 * specified
		 */
		template<typename U = T>
		EventListener(EventDispatcher<U>& dispatcher)
		{
			RegisterForDispatcher(&dispatcher);
		}

		/**
		 * Requires dispatcher to be registered using RegisterForDispatcher
		 */
		template<typename U = T>
		EventListener() requires(!std::derived_from<Window, EventDispatcher<U>>);

		/**
		 * Registers this to listen to events from dispatcher
		 */
		void RegisterForDispatcher(EventDispatcher<T>* dispatcher)
		{
			m_Dispatcher = dispatcher;
			m_Dispatcher->RegisterListener(this);
		}

		void UnregisterForDispatcher()
		{
			if (m_Dispatcher)
				m_Dispatcher->UnregisterListener(this);
			m_Dispatcher = nullptr;
		}

		/**
		 * Events of type T will get dispatched to this function
		 */
		virtual void OnEvent(T& e) = 0;

		virtual ~EventListener() { UnregisterForDispatcher(); }


		EventListener<T>& operator=(EventListener<T>&& other) noexcept
		{
			m_Dispatcher = other.m_Dispatcher;
			if (m_Dispatcher != nullptr)
			{
				other.UnregisterForDispatcher();
				m_Dispatcher->RegisterListener(this);
			}
			return *this;
		}
		EventListener(EventListener<T>&& other) noexcept { *this = std::move(other); }

	private:
		EventDispatcher<T>* m_Dispatcher = nullptr;
	};
}  // namespace At0::Ray