#include "RKeyboardEvent.h"
#include <sstream>


namespace At0::Ray
{
	// -------------------------------------------------------------------
	// KeyPressedEvent
	// -------------------------------------------------------------------
	KeyPressedEvent::KeyPressedEvent(const unsigned char keycode, uint32_t repeatCount)
		: KeyEvent(keycode), m_RepeatCount(repeatCount)
	{
	}

	std::string KeyPressedEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[KeyPressedEvent] Key " << (uint16_t)GetKeyCode() << " pressed";
		return oss.str();
	}

	// -------------------------------------------------------------------
	// KeyReleasedEvent
	// -------------------------------------------------------------------
	KeyReleasedEvent::KeyReleasedEvent(const unsigned char keycode)
		: KeyEvent(keycode)
	{
	}

	std::string KeyReleasedEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[KeyReleasedEvent] Key " << (uint16_t)GetKeyCode() << " released";
		return oss.str();
	}

	// -------------------------------------------------------------------
	// CharEvent
	// -------------------------------------------------------------------
	CharEvent::CharEvent(const unsigned char keycode)
		: KeyEvent(keycode)
	{
	}

	std::string CharEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[CharEvent] Character '" << (char)GetKeyCode() << "' written";
		return oss.str();
	}
}