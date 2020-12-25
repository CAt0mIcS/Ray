#pragma once

#include "../RBase.h"
#include <bitset>

#pragma warning(disable : 4251)


namespace At0::Ray
{
	class RAY_API KeyboardInput
	{
		friend class WinAPIWindow;
		friend class OpenGLWindow;
	public:
		KeyboardInput() = default;
		~KeyboardInput();

		/// <summary>
		/// Checks if a key with keycode is pressed
		/// </summary>
		/// <param name="keycode">Is the keycode to check for</param>
		/// <returns>True if the key with keycode is pressed, false otherwise</returns>
		bool IsKeyPressed(uint16_t keycode) const;

	private:
		void SetKeyState(uint16_t keycode, bool isPressed);

	private:
		/// <summary>
		/// Holds pressed-info about every keycode
		/// </summary>
		std::bitset<512> m_Keys;
	};
}


