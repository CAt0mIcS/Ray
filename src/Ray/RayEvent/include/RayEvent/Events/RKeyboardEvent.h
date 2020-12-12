#pragma once

#include "REvent.h"


namespace At0::Ray
{
	class KeyEvent : public Event
	{
	protected:
		KeyEvent(uint16_t keycode)
			: m_KeyCode(keycode)
		{
		}

	public:
		uint16_t GetKeyCode() const { return m_KeyCode; }
		virtual ~KeyEvent() = default;

	private:
		uint16_t m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(uint16_t keycode, uint32_t repeatCount);

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
		KeyReleasedEvent(uint16_t keycode);

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


