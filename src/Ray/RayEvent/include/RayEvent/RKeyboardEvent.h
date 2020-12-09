#pragma once

#include "REvent.h"

#include <sstream>


namespace At0::Ray
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
		virtual ~KeyEvent() = default;

	private:
		unsigned char m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const unsigned char keycode, uint32_t repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount)
		{
		}

		uint32_t GetRepeatCount() const { return m_RepeatCount; }

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[KeyPressedEvent] Key " << (uint16_t)GetKeyCode() << " pressed";
			return oss.str();
		}

		static EventType GetStaticType() { return EventType::KeyPressedEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }

	private:
		uint32_t m_RepeatCount;
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

		static EventType GetStaticType() { return EventType::KeyReleasedEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
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
			oss << "[CharEvent] Character '" << (char)GetKeyCode() << "' written";
			return oss.str();
		}

		static EventType GetStaticType() { return EventType::CharEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
	};
}


