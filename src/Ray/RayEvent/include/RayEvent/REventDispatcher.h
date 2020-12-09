#pragma once

#include "RApplicationEvent.h"
#include "RMouseEvent.h"
#include "RKeyboardEvent.h"


namespace At0::Ray
{
	template<typename E>
	class EventListener;

	template<typename E>
	class EventDispatcher
	{
	public:
		virtual void AddListener(EventListener<E>* eListener) = 0;

	protected:
		EventDispatcher() = default;
	};
}

