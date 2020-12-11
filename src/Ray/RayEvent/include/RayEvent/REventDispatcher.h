#pragma once


#include <vector>

namespace At0::Ray
{
	template<typename E>
	class EventListener;

	template<typename E>
	class EventDispatcher
	{
		friend class EventListener<E>;
	public:
		virtual ~EventDispatcher() = default;

	protected:
		EventDispatcher() = default;

		/// <summary>
		/// Getter for the vector of listeners.
		/// </summary>
		/// <returns>The vector of EventListeners for a specific event</returns>
		const auto Get() const { return m_Listeners; }

	private:
		/// <summary>
		/// Adds a new EventListener for template Event.
		/// </summary>
		/// <param name="eListener">Is a pointer to the EventListener</param>
		void AddListener(EventListener<E>* eListener);

	private:
		std::vector<EventListener<E>*> m_Listeners;
	};


	template<typename E>
	inline void EventDispatcher<E>::AddListener(EventListener<E>* eListener)
	{
		m_Listeners.push_back(eListener);
	}
}

