#pragma once

#include <bitset>
#include "GUIBase.h"


namespace GUI
{
	/**
	* Static class to get Keyboard stuff
	*/
	class GUI_API Keyboard
	{
	public:
		/**
		* Deleted contructor and copy-asignment operators
		*/
		Keyboard() = delete;
		Keyboard(const Keyboard&) = delete;
		Keyboard operator=(const Keyboard&) = delete;

		/**
		* Sets the key state of a keycode
		* 
		* @param keycode is the code to change
		* @param state is either 1 for down or 0 for up
		*/
		static void SetKeyState(const unsigned char keycode, const unsigned char state) { m_KeyStates[keycode] = state; }

		/**
		* Checks if a key with keycode is pressed
		* 
		* @param keycode is the code of the key
		* @returns true if the key with keycode is pressed, false otherwise
		*/
		static bool IsKeyPressed(const unsigned char keycode) { return m_KeyStates[keycode]; }

		/**
		* Sets all keys to 0
		*/
		static void ClearStates() { m_KeyStates = std::bitset<m_nKeys>(); }

		/**
		* Getter for the amount of keys
		* 
		* @returns the amount of keys
		*/
		static constexpr unsigned short GetNKeys() { return m_nKeys; }

	private:
		static constexpr unsigned short m_nKeys = 256u;
		static std::bitset<m_nKeys> m_KeyStates;

	};

}



