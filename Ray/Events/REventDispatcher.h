#pragma once

#include <set>
#include "RCustomEvents.h"


namespace At0::Ray
{
	template<typename T>
	class EventListener;

	template<typename T>
	class RAY_EXPORT EventDispatcher
	{
	public:
		EventDispatcher() = default;

		// RAY_TODO:
		EventDispatcher(EventDispatcher&& other) = delete;
		EventDispatcher& operator=(const EventDispatcher&& other) = delete;
		EventDispatcher(EventDispatcher&) = delete;
		EventDispatcher& operator=(const EventDispatcher&) = delete;

		/**
		 * Iterate over all listeners in the event dispatcher
		 */
		auto& Get() { return m_Listeners; }

		/**
		 * Adds a new listener to dispatch the events to
		 */
		void RegisterListener(EventListener<T>* listener) { m_Listeners.emplace(listener); }

		/**
		 * Removes a listener from getting events
		 */
		void UnregisterListener(EventListener<T>* listener) { m_Listeners.erase(listener); }

	private:
		std::set<EventListener<T>*> m_Listeners;
	};

	template<>
	class RAY_EXPORT EventDispatcher<TransformChangedEvent>
	{
	public:
		EventDispatcher() = default;

		EventDispatcher(EventDispatcher&& other) { *this = std::move(other); }
		EventDispatcher& operator=(const EventDispatcher&& other);

		/**
		 * Iterate over all listeners in the event dispatcher
		 */
		auto& Get() { return m_Listeners; }

		/**
		 * Adds a new listener to dispatch the events to
		 */
		void RegisterListener(EventListener<TransformChangedEvent>* listener)
		{
			m_Listeners.emplace(listener);
		}

		/**
		 * Removes a listener from getting events
		 */
		void UnregisterListener(EventListener<TransformChangedEvent>* listener)
		{
			m_Listeners.erase(listener);
		}

	private:
		std::set<EventListener<TransformChangedEvent>*> m_Listeners;
	};
}  // namespace At0::Ray