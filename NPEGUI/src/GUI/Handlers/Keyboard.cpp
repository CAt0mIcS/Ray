#include "pch.h"
#include "Keyboard.h"


namespace GUI
{
	std::bitset<Keyboard::GetNKeys()> Keyboard::m_KeyStates{};
}