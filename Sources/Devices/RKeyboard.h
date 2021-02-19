#pragma once

#include "../RBase.h"
#include "../Core/RKeyCodes.h"

#include <bitset>


namespace At0::Ray
{
	class RAY_EXPORT Keyboard
	{
		friend class Window;

	public:
		Keyboard() = delete;

		/**
		 * @param keycode Is the key to check for
		 * @returns If the specified key is pressed
		 */
		static bool IsKeyPressed(Key keycode);

	private:
		static void SetKeyState(Key keycode, bool isPressed);

	private:
		// Holds pressed-info about every keycode
		inline static std::bitset<512> m_Keys;
	};
}  // namespace At0::Ray
