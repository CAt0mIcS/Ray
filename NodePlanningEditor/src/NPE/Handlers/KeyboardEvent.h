#pragma once

#include "Event.h"

#include "Keyboard.h"


namespace NPE
{
	class KeyEvent : public Event
	{
	protected:
		KeyEvent(const unsigned char keycode)
			: m_KeyCode(keycode)
		{
		}

		unsigned char GetKeyCode() const { return m_KeyCode; }

	private:
		unsigned char m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const unsigned char keycode)
			: KeyEvent(keycode)
		{
			Keyboard::SetKeyState(keycode, 1);
		}

		NPE_DECLARE_TYPE_FN(KeyPressedEvent)
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const unsigned char keycode)
			: KeyEvent(keycode)
		{
			Keyboard::SetKeyState(keycode, 0);
		}

		NPE_DECLARE_TYPE_FN(KeyReleasedEvent)
	};
}


