#pragma once

#include "REvent.h"
#include <../../RayUtil/include/RayUtil/RKeyCodes.h>


namespace At0::Ray
{
	class KeyEvent : public Event
	{
	protected:
		KeyEvent(Key keycode)
			: m_KeyCode(keycode)
		{
		}

	public:
		Key GetKey() const { return m_KeyCode; }
		virtual ~KeyEvent() = default;

	private:
		Key m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(Key keycode, uint32_t repeatCount);

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
		KeyReleasedEvent(Key keycode);

		virtual std::string ToString() const override;

		static EventType GetStaticType() { return EventType::KeyReleasedEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
	};

	class CharEvent : public Event
	{
	public:
		CharEvent(uint16_t key);

		virtual std::string ToString() const override;

		static EventType GetStaticType() { return EventType::CharEvent; }
		uint16_t GetKeyCode() const { return m_KeyCode; }
		virtual EventType GetType() const override { return GetStaticType(); }

	private:
		uint16_t m_KeyCode;
	};
}


