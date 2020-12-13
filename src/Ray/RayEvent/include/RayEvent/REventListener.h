#pragma once

#include "REventDispatcher.h"


namespace At0::Ray
{
	class Widget;

	template<typename E>
	class EventListener
	{
	public:
		/// <summary>
		/// Is called whenever the templated event is received
		/// </summary>
		/// <param name="receiver">Is the object which sended the event</param>
		/// <param name="e">Is the received event</param>
		virtual void OnEvent(Ray::Widget* receiver, E& e) = 0;

		/// <summary>
		/// Sets this listener up to receiver events of type E
		/// </summary>
		/// <param name="dispatcher">Is the dispatcher dispatching the templated event</param>
		void Subscribe(EventDispatcher<E>& dispatcher)
		{
			dispatcher.AddListener(this);
			m_Dispatchers.push_back(&dispatcher);
		}

		/// <summary>
		/// Stops the dispatcher from sending events to this listener
		/// </summary>
		/// <param name="dispatcher">Is the dispatcher which sends messages to the listener</param>
		void Unsubscribe(EventDispatcher<E>& dispatcher)
		{
			dispatcher.RemoveListener(this);
		}

		/// <summary>
		/// EventListener Constructor
		/// </summary>
		virtual ~EventListener()
		{
			for (EventDispatcher<E>* dispatcher : m_Dispatchers)
			{
				dispatcher->RemoveListener(this);
			}
		}

	protected:
		/// <summary>
		/// Automatically adds a new EventListener to the list of listeners
		/// </summary>
		/// <param name="dispatcher">Is the object that dispatches the event you want to listen to</param>
		EventListener(EventDispatcher<E>& dispatcher)
		{
			Subscribe(dispatcher);
		}

		EventListener() = default;

	private:
		// TODO: Possible invalid memory access
		std::vector<EventDispatcher<E>*> m_Dispatchers;
	};
}