#pragma once

#include <bitset>
#include "GUIBase.h"


namespace GUI
{
	/// <summary>
	/// Static class which will always be updated with any keyboard events
	/// </summary>
	class GUI_API Keyboard
	{
	public:
		/// <summary>
		/// Deleted Constructor
		/// </summary>
		Keyboard() = delete;

		/// <summary>
		/// Deleted Copy Constructor
		/// </summary>
		Keyboard(_In_ const Keyboard&) = delete;

		/// <summary>
		/// Deleted Copy aassignment operator
		/// </summary>
		Keyboard operator=(_In_ const Keyboard&) = delete;

		/// <summary>
		/// Sets the key state of a keycode
		/// </summary>
		/// <param name="keycode">Is the code to change</param>
		/// <param name="state">Is either 1 for down or 0 for up</param>
		static void SetKeyState(_In_ const unsigned char keycode, _In_ const unsigned char state) { m_KeyStates[keycode] = state; }

		/// <summary>
		/// Checks if a key with keycode is pressed
		/// </summary>
		/// <param name="keycode">Is the code of the key</param>
		/// <returns>True if the key with keycode is pressed, false otherwise</returns>
		static bool IsKeyPressed(_In_ const unsigned char keycode) { return m_KeyStates[keycode]; }

		/// <summary>
		/// Sets all keys to 0
		/// </summary>
		static void ClearStates() { m_KeyStates = std::bitset<m_nKeys>(); }

		/// <summary>
		/// Getter for number of keys
		/// </summary>
		/// <returns>The amount of allowed keys</returns>
		static constexpr unsigned short GetNKeys() { return m_nKeys; }

	private:
		/// <summary>
		/// Number of keys, size of m_KeyStates
		/// </summary>
		static constexpr unsigned short m_nKeys = 256u;

		/// <summary>
		/// Holds information whether a key is down (1) or up (0) for every keycode
		/// </summary>
		static std::bitset<m_nKeys> m_KeyStates;

	};

}



