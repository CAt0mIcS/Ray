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

			for (int i = m_Listeners.size() - 1; i >= 0; --i)
			{
				if (m_Listeners[i] == nullptr)
					m_Listeners.erase(m_Listeners.begin() + i);
				else if (m_Listeners[i] == listener)
				{
					m_Listeners.erase(m_Listeners.begin() + i);
					break;
				}
			}
		}

	private:
		std::vector<EventListener<T>*> m_Listeners;
	};
}  // namespace At0::Ray
