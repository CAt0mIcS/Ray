#include "RKeyboardEvent.h"
#include <sstream>


namespace At0::Ray
{
	// -------------------------------------------------------------------
	// KeyPressedEvent
	// -------------------------------------------------------------------
	KeyPressedEvent::KeyPressedEvent(Key keycode, uint32_t repeatCount)
		: KeyEvent(keycode), m_RepeatCount(repeatCount)
	{
	}

	std::string KeyPressedEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[KeyPressedEvent] Key " << KeyToString(GetKey()) << " pressed (" << (uint16_t)GetKey() << ')';
		return oss.str();
	}

	// -------------------------------------------------------------------
	// KeyReleasedEvent
	// -------------------------------------------------------------------
	KeyReleasedEvent::KeyReleasedEvent(Key keycode)
		: KeyEvent(keycode)
	{
	}

	std::string KeyReleasedEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[KeyReleasedEvent] Key " << (uint16_t)GetKey() << " released";
		return oss.str();
	}

	// -------------------------------------------------------------------
	// CharEvent
	// -------------------------------------------------------------------
	CharEvent::CharEvent(unsigned char keycode)
		: m_Keycode(keycode)
	{
	}

	std::string CharEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[CharEvent] Character '" << (char)GetKeyCode() << "' written";
		return oss.str();
	}
}