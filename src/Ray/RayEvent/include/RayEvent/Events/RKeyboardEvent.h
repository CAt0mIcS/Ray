#pragma once

#include "REvent.h"


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
		KeyPressedEvent(const unsigned char keycode, uint32_t repeatCount);

		virtual std::string ToString() const override;

		uint32_t GetRepeatCount() const { return m_RepeatCount; }

		static EventType GetStaticType() { return EventType::KeyPressedEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }

	private:
		uint32_t m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const unsigned char keycode);

		virtual std::string ToString() const override;

		static EventType GetStaticType() { return EventType::KeyReleasedEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
	};

	class CharEvent : public KeyEvent
	{
	public:
		CharEvent(const unsigned char keycode);

		virtual std::string ToString() const override;

		static EventType GetStaticType() { return EventType::CharEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
	};
}


