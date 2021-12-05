#pragma once

#include <vector>
#include <algorithm>


namespace At0::Ray
{
	template<typename T>
	class EventListener;

	template<typename T>
	class EventDispatcher
	{
	public:
		EventDispatcher() = default;

		// RAY_TODO:
		EventDispatcher(EventDispatcher&&) = delete;
		EventDispatcher& operator=(const EventDispatcher&&) = delete;
		EventDispatcher(EventDispatcher&) = delete;
		EventDispatcher& operator=(const EventDispatcher&) = delete;

		/**
		 * Iterate over all listeners in the event dispatcher
		 */
		auto& Get() { return m_Listeners; }

		/**
		 * Adds a new listener to dispatch the events to
		 */
		void RegisterListener(EventListener<T>* listener)
		{
			m_Listeners.emplace_back(listener);
			// Remove duplicates (low number of listeners --> faster than set)
			// (RAY_TODO: Test performance)
			std::sort(m_Listeners.begin(), m_Listeners.end());
			m_Listeners.erase(
				std::unique(m_Listeners.begin(), m_Listeners.end()), m_Listeners.end());
		}

		/**
		 * Removes a listener from getting events
		 */
		void UnregisterListener(EventListener<T>* listener)
		{
			if (m_Listeners.size() == 0)
				return;

			for (int i = m_Listeners.size() - 1; i >= 0; --i)
				if (m_Listeners[i] == nullptr || m_Listeners[i] == listener)
					m_Listeners.erase(m_Listeners.begin() + i);
		}

	private:
		// RAY_TODO: Test performance to std::set (faster lookups for small amount of listeners)
		std::vector<EventListener<T>*> m_Listeners;
	};
}  // namespace At0::Ray
