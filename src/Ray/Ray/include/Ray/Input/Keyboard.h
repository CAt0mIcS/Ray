#pragma once

#include "Ray/RBase.h"
#include <bitset>

#pragma warning(disable : 4251)


namespace At0::Ray
{
	class RAY_API KeyboardInput
	{
		friend class WinAPIWindow;
		friend class OpenGLWindow;
	public:
		/// <summary>
		/// Default Keyboard Constructor
		/// </summary>
		KeyboardInput() = default;

		/// <summary>
		/// Checks if a key with keycode is pressed
		/// </summary>
		/// <param name="keycode">Is the keycode to check for</param>
		/// <returns>True if the key with keycode is pressed, false otherwise</returns>
		bool IsKeyPressed(unsigned char keycode) const;

	private:
		/// <summary>
		/// Sets the pressed state of a key to isPressed
		/// </summary>
		/// <param name="keycode">Is the keycode to set</param>
		/// <param name="isPressed">Is true if the key is pressed, false otherwise</param>
		void SetKeyState(unsigned char keycode, bool isPressed);

	private:
		/// <summary>
		/// Total number of keys
		/// </summary>
		static constexpr uint8_t s_nKeys = 0xff;

		/// <summary>
		/// Holds pressed-info about every keycode
		/// </summary>
		std::bitset<s_nKeys> m_Keys;
	};
}


