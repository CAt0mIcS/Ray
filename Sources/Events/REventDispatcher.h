#pragma once

#include <vector>


namespace At0::Ray
{
#ifdef __GNUC__
	template<typename T>
	class EventListener;
#elif defined(_MSC_VER)
	template<typename T, typename>
	class EventListener;
#endif

	template<typename T>
	class EventDispatcher
	{
#ifdef __GNUC__
		using EventListenerType = EventListener<T>;
#elif defined(_MSC_VER)
		using EventListenerType = EventListener<T, void>;
#endif
	public:
		EventDispatcher() = default;

		/**
		 * Iterate over all listeners in the event dispatcher
		 */
		std::vector<EventListenerType*>& Get() { return m_Listeners; }

		/**
		 * Adds a new listener to dispatch the events to
		 */
		void Register(EventListenerType* listener) { m_Listeners.emplace_back(listener); }

		/**
		 * Removes a listener from getting events
		 */
		void Unregister(EventListenerType* listener)
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
		std::vector<EventListenerType*> m_Listeners;
	};
}  // namespace At0::Ray
