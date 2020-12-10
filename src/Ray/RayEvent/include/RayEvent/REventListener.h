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

	protected:
		/// <summary>
		/// Automatically adds a new EventListener to the list of listeners
		/// </summary>
		/// <param name="dispatcher">Is the object that dispatches the event you want to listen to</param>
		EventListener(EventDispatcher<E>& dispatcher)
		{
			dispatcher.AddListener(this);
		}
	};
}