#include "rpch.h"
#include "Ray/Input/Keyboard.h"

#include <RayDebug/RLogger.h>


namespace At0::Ray
{
	KeyboardInput::~KeyboardInput()
	{
		RAY_LOG_DEBUG("[KeyboardInput] Destroyed");
	}

	bool KeyboardInput::IsKeyPressed(unsigned char keycode) const
	{
		return m_Keys[keycode];
	}
	
	void KeyboardInput::SetKeyState(unsigned char keycode, bool isPressed)
	{
		m_Keys[keycode] = isPressed;
	}
}


