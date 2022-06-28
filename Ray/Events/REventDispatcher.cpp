#pragma once

#include "REventDispatcher.h"
#include "REventListener.h"

#include "Components/RTransform.h"


namespace At0::Ray
{
	EventDispatcher<TransformChangedEvent>& EventDispatcher<TransformChangedEvent>::operator=(
		const EventDispatcher<TransformChangedEvent>&& other)
	{
		for (auto listener : other.m_Listeners)
		{
			listener->UnregisterForDispatcher();
			listener->RegisterForDispatcher(*this);
		}

		return *this;
	}
}  // namespace At0::Ray