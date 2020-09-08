#pragma once

#include "Util/Util.h"
#include "GUIBase.h"


namespace GUI
{
	class Control;

	class GUI_API Mouse
	{
	public:
		/**
		* Deleted contructor and copy-asignment operators
		*/
		Mouse() = delete;
		Mouse(const Mouse&) = delete;
		Mouse operator=(const Mouse&) = delete;

		/**
		* Sets mouse position
		* 
		* @param pos is the new mouse position
		* @warning function does not update actual mouse position, just the member variable
		*/
		static void SetPos(const Util::NPoint& pos) { m_Pos = pos; }

		/**
		* Getter for mouse position
		* 
		* @returns the current mouse position
		*/
		static const Util::NPoint& GetPos() { return m_Pos; }

		/**
		* Sets left pressed
		* 
		* @param pressed is the new state of the mouse
		* @warning function does not actually press the mouse, just the member variable
		*/
		static void SetLeftPressed(bool pressed) { m_IsLeftPressed = pressed; }

		/**
		* Getter for left pressed state
		* 
		* @returns true if the left mouse button is pressed, false otherwise
		*/
		static bool IsLeftPressed() { return m_IsLeftPressed; }

		/**
		* Sets middle pressed
		*
		* @param pressed is the new state of the mouse
		* @warning function does not actually press the mouse, just the member variable
		*/
		static void SetMiddlePressed(bool pressed) { m_IsMiddlePressed = pressed; }
		
		/**
		* Getter for middle pressed state
		*
		* @returns true if the middle mouse button is pressed, false otherwise
		*/
		static bool IsMiddlePressed() { return m_IsMiddlePressed; }
		
		/**
		* Sets right pressed
		*
		* @param pressed is the new state of the mouse
		* @warning function does not actually press the mouse, just the member variable
		*/
		static void SetRightPressed(bool pressed) { m_IsRightPressed = pressed; }

		/**
		* Getter for right pressed state
		*
		* @returns true if the right mouse button is pressed, false otherwise
		*/
		static bool IsRightPressed() { return m_IsRightPressed; }

		/**
		* Checks if the mouse is on the control
		* 
		* @param control is the control to check is the mouse is on
		* @returns true is the mouse is on the control, false otherwise
		*/
		static bool IsOnControl(const Control* const control);

		/**
		* Sets all pressed to false
		*/
		static void ClearStates() { m_IsLeftPressed = false; m_IsMiddlePressed = false; m_IsRightPressed = false; }

	private:
		static Util::NPoint m_Pos;
		static bool m_IsLeftPressed;
		static bool m_IsRightPressed;
		static bool m_IsMiddlePressed;
	};
}

