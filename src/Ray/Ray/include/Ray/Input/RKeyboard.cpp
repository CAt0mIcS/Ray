#include "Ray/rpch.h"
#include "Ray/Input/RKeyboard.h"

#include <RayDebug/RLogger.h>
#include <RayDebug/RAssert.h>


namespace At0::Ray
{
	KeyboardInput::~KeyboardInput()
	{
		Log::Debug("[KeyboardInput] Destroyed");
	}

	bool KeyboardInput::IsKeyPressed(Key keycode) const
	{
		RAY_MEXPECTS((uint16_t)keycode <= 512, "The keycode was too large");
		return m_Keys[(uint16_t)keycode];
	}

	void KeyboardInput::SetKeyState(Key keycode, bool isPressed)
	{
		RAY_MEXPECTS((uint16_t)keycode <= 512, "The keycode was too large");
		m_Keys[(uint16_t)keycode] = isPressed;
	}
}


