#pragma once

#include "Event.h"



namespace Zeal::Reyal
{
	class KeyEvent : public Event
	{
	protected:
		KeyEvent(const unsigned char keycode)
			: m_KeyCode(keycode)
		{
		}

	public:
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
		}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[KeyPressedEvent] Key " << (uint16_t)GetKeyCode() << " pressed";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(KeyPressedEvent)
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const unsigned char keycode)
			: KeyEvent(keycode)
		{
		}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[KeyReleasedEvent] Key " << (uint16_t)GetKeyCode() << " released";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(KeyReleasedEvent)
	};

	class CharEvent : public KeyEvent
	{
	public:
		CharEvent(const unsigned char keycode)
			: KeyEvent(keycode)
		{
		}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[CharEvent] Character " << GetKeyCode() << " written";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(CharEvent)
	};
}


