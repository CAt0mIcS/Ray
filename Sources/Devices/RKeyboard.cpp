#include "Rpch.h"
#include "RKeyboard.h"

#include "Utils/RLogger.h"
#include "Utils/RAssert.h"


namespace At0::Ray
{
	std::bitset<512> Keyboard::m_Keys{};

	bool Keyboard::IsKeyPressed(Key keycode)
	{
		RAY_MEXPECTS((uint16_t)keycode <= 512, "[Keyboard] The keycode {0} was too large",
			(uint16_t)keycode);
		return m_Keys[(uint16_t)keycode];
	}

	void Keyboard::SetKeyState(Key keycode, bool isPressed)
	{
		RAY_MEXPECTS((uint16_t)keycode <= 512, "[Keyboard] The keycode {0} was too large",
			(uint16_t)keycode);
		m_Keys[(uint16_t)keycode] = isPressed;
	}
}  // namespace At0::Ray
