#pragma once

#include "RlBase.h"
#include <bitset>


namespace At0::Reyal
{
	class RL_API Keyboard
	{
	public:
		Keyboard() = default;

		bool IsKeyPressed(unsigned char keycode) const;

		void SetKeyState(unsigned char keycode, bool isPressed);

	private:
		static constexpr uint8_t s_nKeys = 0xff;
		std::bitset<s_nKeys> m_Keys;
	};
}



