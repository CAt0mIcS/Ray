#include "rlpch.h"
#include "Keyboard.h"



namespace Zeal::Reyal
{
	bool Keyboard::IsKeyPressed(unsigned char keycode) const
	{
		return m_Keys[keycode];
	}
	
	void Keyboard::SetKeyState(unsigned char keycode, bool isPressed)
	{
		m_Keys[keycode] = isPressed;
	}
}

