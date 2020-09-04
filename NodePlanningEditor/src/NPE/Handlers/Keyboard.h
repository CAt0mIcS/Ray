#pragma once

#include <bitset>


//TODO: fix bitset out of range exception
namespace NPE
{
	class Keyboard
	{
	public:
		Keyboard() = delete;

		static void SetKeyState(const unsigned char keycode, const unsigned char state) { m_KeyStates[keycode] = state; }
		static bool IsKeyPressed(const unsigned char keycode) { return m_KeyStates[keycode]; }

		static void ClearStates() { m_KeyStates = std::bitset<m_nKeys>(); }

		static constexpr unsigned short GetNKeys() { return m_nKeys; }

	private:
		static constexpr unsigned short m_nKeys = 256u;
		static std::bitset<m_nKeys> m_KeyStates;

	};

	inline std::bitset<Keyboard::GetNKeys()> Keyboard::m_KeyStates;
}



