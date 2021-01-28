#pragma once

#include "../RBase.h"
#include "RKeyCodes.h"

#include <bitset>


namespace At0::Ray
{
	class RAY_EXPORT KeyboardInput
	{
		friend class Window;
	public:
		KeyboardInput() = default;
		~KeyboardInput();

		/**
		* @param keycode Is the key to check for
		* @returns If the specified key is pressed
		*/
		bool IsKeyPressed(Key keycode) const;

	private:
		void SetKeyState(Key keycode, bool isPressed);

	private:
		// Holds pressed-info about every keycode
		std::bitset<512> m_Keys;
	};
}
