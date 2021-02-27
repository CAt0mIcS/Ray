#pragma once

#include <vector>


namespace At0::Ray
{
	template<typename T, typename>
	class EventListener;

	template<typename T>
	class EventDispatcher
	{
	public:
		EventDispatcher() = default;

		/**
		 * Iterate over all listeners in the event dispatcher
		 */
		std::vector<EventListener<T, void>*>& Get() { return m_Listeners; }

		/**
		 * Adds a new listener to dispatch the events to
		 */
		void Register(EventListener<T, void>* listener) { m_Listeners.emplace_back(listener); }

		/**
		 * Removes a listener from getting events
		 */
		void Unregister(EventListener<T, void>* listener)
		{
			if (m_Listeners.size() == 0)
				return;

			if (auto it = std::find(m_Listeners.begin(), m_Listeners.end(), listener);
				it != m_Listeners.end())
				m_Listeners.erase(it);
		}

	private:
		std::vector<EventListener<T, void>*> m_Listeners;
	};
}  // namespace At0::Ray
