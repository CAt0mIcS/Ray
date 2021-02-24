#pragma once

#include <vector>


namespace At0::Ray
{
	template<typename T>
	class EventListener;

	template<typename T>
	class EventDispatcher
	{
	public:
		EventDispatcher() = default;

		/**
		 * Iterate over all listeners in the event dispatcher
		 */
		std::vector<EventListener<T>*>& Get() { return m_Listeners; }

		/**
		 * Adds a new listener to dispatch the events to
		 */
		void Register(EventListener<T>* listener) { m_Listeners.emplace_back(listener); }

		/**
		 * Removes a listener from getting events
		 */
		void Unregister(EventListener<T>* listener)
		{
			if (auto it = std::find(m_Listeners.begin(), m_Listeners.end(), listener);
				it != m_Listeners.end())
				m_Listeners.erase(it);
		}

	private:
		std::vector<EventListener<T>*> m_Listeners;
	};
}  // namespace At0::Ray
