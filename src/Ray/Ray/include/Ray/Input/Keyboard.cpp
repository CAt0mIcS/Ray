#include "Ray/rpch.h"
#include "Ray/Input/Keyboard.h"

#include <RayDebug/RLogger.h>
#include <RayDebug/RAssert.h>


namespace At0::Ray
{
	KeyboardInput::~KeyboardInput()
	{
		RAY_LOG_DEBUG("[KeyboardInput] Destroyed");
	}

	bool KeyboardInput::IsKeyPressed(uint16_t keycode) const
	{
		RAY_MEXPECTS(keycode <= 512, "The keycode was too large");
		return m_Keys[keycode];
	}
	
	void KeyboardInput::SetKeyState(uint16_t keycode, bool isPressed)
	{
		RAY_MEXPECTS(keycode <= 512, "The keycode was too large");
		m_Keys[keycode] = isPressed;
	}
}


