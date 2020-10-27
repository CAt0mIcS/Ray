#ifndef RL_KBDEVENT_H
#define RL_KBDEVENT_H

#include "RlBase.h"
#include "Event.h"



namespace At0::Reyal
{
	class RL_API KeyEvent : public Event
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

	class RL_API KeyPressedEvent : public KeyEvent
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

	class RL_API KeyReleasedEvent : public KeyEvent
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

	class RL_API CharEvent : public KeyEvent
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


#endif /* RL_KBDEVENT_H */
