#pragma once

#include "RlBase.h"

#include <sstream>


namespace Zeal::Reyal
{
	enum class EventType
	{
		INVALID = 0, KeyPressed
	};

	class RL_API Event
	{
	public:
		Event(EventType type = EventType::INVALID)
			: m_Type(type) {}

		EventType GetType() const { return m_Type; }

	protected:
		EventType m_Type;
	};


	class RL_API KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(unsigned int keycode)
			: Event(EventType::KeyPressed), m_KeyCode(keycode)
		{
		}

		unsigned int GetKeyCode() const { return m_KeyCode; }

	private:
		unsigned int m_KeyCode;
	};
}
