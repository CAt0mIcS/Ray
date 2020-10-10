#pragma once

#include "Util/Util.h"
#include "GUIBase.h"


namespace GUI
{
	class Control;

	/// <summary>
	/// Static class which will always be updated with any mouse events
	/// </summary>
	class GUI_API Mouse
	{
	public:
		/// <summary>
		/// Deleted Constructor
		/// </summary>
		Mouse() = delete;

		/// <summary>
		/// Deleted Copy Constructor
		/// </summary>
		Mouse(_In_ const Mouse&) = delete;

		/// <summary>
		/// Deleted Copy Assignment operator
		/// </summary>
		Mouse operator=(_In_ const Mouse&) = delete;

		/// <summary>
		/// Sets mouse position, only sets member variable, does not actually move the mouse, 
		/// Also transforms the position to work with Renderer Transforms
		/// </summary>
		/// <param name="pos">Is the new mouse position</param>
		static void SetPos(_In_ const Util::NPoint& pos);

		/// <summary>
		/// Getter for the current mouse position in the window
		/// </summary>
		/// <returns>The current mouse position</returns>
		static const Util::NPoint& GetPos() { return m_Pos; }

		/// <summary>
		/// Sets left pressed, only sets member variable, does not actually press the Button
		/// </summary>
		/// <param name="pressed">Is the new state of the mouse</param>
		static void SetLeftPressed(_In_ bool pressed) { m_IsLeftPressed = pressed; }

		/// <summary>
		/// Getter for left pressed state
		/// </summary>
		/// <returns>True if the left mouse Button is pressed, false otherwise</returns>
		static bool IsLeftPressed() { return m_IsLeftPressed; }

		/// <summary>
		/// Sets middle pressed, only sets member variable, does not actually press the Button
		/// </summary>
		/// <param name="pressed">Is the new state of the mouse</param>
		static void SetMiddlePressed(_In_ bool pressed) { m_IsMiddlePressed = pressed; }
		
		/// <summary>
		/// Getter for middle pressed state
		/// </summary>
		/// <returns>True if the middle mouse Button is pressed, false otherwise</returns>
		static bool IsMiddlePressed() { return m_IsMiddlePressed; }

		/// <summary>
		/// Sets right pressed, only sets member variable, does not actually press the Button
		/// </summary>
		/// <param name="pressed">Is the new state of the mouse</param>
		static void SetRightPressed(_In_ bool pressed) { m_IsRightPressed = pressed; }

		/// <summary>
		/// Getter for right pressed state
		/// </summary>
		/// <returns>True if the right mouse Button is pressed, false otherwise</returns>
		static bool IsRightPressed() { return m_IsRightPressed; }

		/// <summary>
		/// Checks if the mouse is on the Control
		/// </summary>
		/// <param name="control">Is the Control to check if the mouse is on</param>
		/// <returns>True if the mouse is on the Control, false otherwise</returns>
		static bool IsOnControl(_In_ const Control* const control);

		/// <summary>
		/// Sets all pressed to false
		/// </summary>
		static void ClearStates() { m_IsLeftPressed = false; m_IsMiddlePressed = false; m_IsRightPressed = false; }

	private:
		/// <summary>
		/// Current mouse position
		/// </summary>
		static Util::NPoint m_Pos;

		/// <summary>
		/// True if left mouse Button is pressed
		/// </summary>
		static bool m_IsLeftPressed;

		/// <summary>
		/// True if right mouse Button is pressed
		/// </summary>
		static bool m_IsRightPressed;

		/// <summary>
		/// True if middle mouse Button is pressed
		/// </summary>
		static bool m_IsMiddlePressed;
	};
}

