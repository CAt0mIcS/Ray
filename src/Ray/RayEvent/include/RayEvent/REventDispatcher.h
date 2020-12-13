#pragma once

#include <vector>


namespace At0::Ray
{
	template<typename E>
	class EventListener;

	// QUESTIONA: Should I make this automatically dispatch all events? (leave)
	// Possible: EventDispatcher<WindowResizeEvent, MouseMoveEvent>
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
		void AddListener(EventListener<E>* eListener)
		{
			m_Listeners.push_back(eListener);
		}

		/// <summary>
		/// Searches for the EventListener and removes it if found
		/// </summary>
		/// <param name="eListener">Is the EventListener to remove</param>
		void RemoveListener(EventListener<E>* eListener)
		{
			// TODO: operator= && no pointers
			m_Listeners.erase(std::find_if(m_Listeners.begin(), m_Listeners.end(), [this, &eListener](EventListener<E>* listener) 
				{
					return listener == eListener;
				}
			));
		}

	private:
		// TODO: Possible invalid memory access
		std::vector<EventListener<E>*> m_Listeners;
	};
}

