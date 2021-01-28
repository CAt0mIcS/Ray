#include "Rpch.h"
#include "RKeyboard.h"

#include "Utils/RLogger.h"
#include "Utils/RAssert.h"


namespace At0::Ray
{
	bool Keyboard::IsKeyPressed(Key keycode)
	{
		RAY_MEXPECTS((uint16_t)keycode <= 512, "The keycode was too large");
		return m_Keys[(uint16_t)keycode];
	}

	void Keyboard::SetKeyState(Key keycode, bool isPressed)
	{
		RAY_MEXPECTS((uint16_t)keycode <= 512, "The keycode was too large");
		m_Keys[(uint16_t)keycode] = isPressed;
	}
}
