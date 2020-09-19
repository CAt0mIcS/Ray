#pragma once

#include "Event.h"

#include "GUI/Handlers/Keyboard.h"


namespace GUI
{
	class KeyEvent : public Event
	{
	protected:
		KeyEvent(_In_ const unsigned char keycode)
			: m_KeyCode(keycode)
		{
		}

	public:
		virtual unsigned char GetKeyCode() const { return m_KeyCode; }

	private:
		unsigned char m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(_In_ const unsigned char keycode)
			: KeyEvent(keycode)
		{
			Keyboard::SetKeyState(keycode, 1);
		}

		NPE_DECLARE_TYPE_FN(KeyPressedEvent)
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(_In_ const unsigned char keycode)
			: KeyEvent(keycode)
		{
			Keyboard::SetKeyState(keycode, 0);
		}

		NPE_DECLARE_TYPE_FN(KeyReleasedEvent)
	};

	class CharEvent : public KeyEvent
	{
	public:
		CharEvent(_In_ const unsigned char keycode)
			: KeyEvent(keycode)
		{
		}

		NPE_DECLARE_TYPE_FN(CharEvent)
	};
}


