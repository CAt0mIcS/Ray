#include "rlpch.h"
#include "Reyal/Input/Keyboard.h"



namespace At0::Reyal
{
	bool KeyboardInput::IsKeyPressed(unsigned char keycode) const
	{
		return m_Keys[keycode];
	}
	
	void KeyboardInput::SetKeyState(unsigned char keycode, bool isPressed)
	{
		m_Keys[keycode] = isPressed;
	}
}


