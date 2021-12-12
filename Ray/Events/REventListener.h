#pragma once

#include "REventDispatcher.h"


namespace At0::Ray
{
	template<typename T>
	class RAY_EXPORT EventListener
	{
	public:
		/**
		 * Registers the listener to receive event of type T.
		 * If the dispatcher<T> is not a base class of the Window the dispatcher<T> needs to be
		 * specified
		 */
		EventListener(EventDispatcher<T>& dispatcher) { RegisterForDispatcher(dispatcher); }

		/**
		 * Requires dispatcher to be registered using RegisterForDispatcher
		 */
		EventListener() = default;

		/**
		 * Registers this to listen to events from dispatcher
		 */
		void RegisterForDispatcher(EventDispatcher<T>& dispatcher)
		{
			m_Dispatcher = &dispatcher;
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