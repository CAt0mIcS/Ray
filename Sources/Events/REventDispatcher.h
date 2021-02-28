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
			if (m_Listeners.size() == 0)
				return;

			for (auto it = m_Listeners.begin(); it != m_Listeners.end(); ++it)
			{
				if (*it == listener)
				{
					m_Listeners.erase(it);
					break;
				}
			}
		}

	private:
		std::vector<EventListener<T>*> m_Listeners;
	};
}  // namespace At0::Ray
