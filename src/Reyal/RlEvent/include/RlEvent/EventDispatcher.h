#pragma once


#include "ApplicationEvent.h"
#include "MouseEvent.h"
#include "KeyboardEvent.h"


namespace At0::Reyal
{
	template<typename E>
	class EventListener;

	template<typename E>
	class EventDispatcher
	{
	public:
		virtual void AddListener(EventListener<E>* e) = 0;

	protected:
		EventDispatcher() = default;
	};
}

