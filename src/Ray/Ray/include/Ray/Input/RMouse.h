#pragma once

#include "../RBase.h"

#include <../../RayUtil/include/RayUtil/GlobalDefines.h>


namespace At0::Ray
{
	class Widget;

	class RAY_API MouseInput
	{
		friend class WinAPIWindow;
		friend class OpenGLWindow;
	public:
		/// <summary>
		/// Mouse Constructor. Initializes everything to false
		/// </summary>
		MouseInput();
		~MouseInput();

		/// <summary>
		/// Checks if the left mouse button is pressed
		/// </summary>
		/// <returns>True if it's pressed, false otherwise</returns>
		bool IsLeftPressed() const;
		
		/// <summary>
		/// Checks if the middle mouse button is pressed
		/// </summary>
		/// <returns>True if it's pressed, false otherwise</returns>
		bool IsMiddlePressed() const;

		/// <summary>
		/// Checks if the right mouse button is pressed
		/// </summary>
		/// <returns>True if it's pressed, false otherwise</returns>
		bool IsRightPressed() const;

		/// <summary>
		/// Checks if the mouse is on a certain widget, handles widget transforms
		/// </summary>
		/// <param name="widget">Is the widget to check</param>
		/// <returns>True if the mouse is on the widget, false otherwise</returns>
		bool IsOnWidget(const Widget* widget) const;
		
		/// <summary>
		/// Checks if the mouse is on a certain widget, handles widget transforms
		/// </summary>
		/// <param name="widget">Is the widget to check</param>
		/// <returns>True if the mouse is on the widget, false otherwise</returns>
		bool IsOnWidget(const Scope<Widget>& widget) const;

		/// <summary>
		/// Getter for the current mouse position
		/// </summary>
		/// <returns>The current mouse position</returns>
		const Point2& GetPos() const;

	private:
		/// <summary>
		/// Sets the position of the mouse, doesn't actually move the mouse
		/// </summary>
		/// <param name="mousePos">Is the new mouse position</param>
		void SetPos(const Point2& mousePos);

		/// <summary>
		/// Sets the state of the left mouse button
		/// </summary>
		/// <param name="isLeftPressed">Is true if the left mouse button is currently pressed, false otherwise</param>
		void SetLeftPressed(bool isLeftPressed) { m_IsLeftPressed = isLeftPressed; }

		/// <summary>
		/// Sets the state of the right mouse button
		/// </summary>
		/// <param name="isLeftPressed">Is true if the right mouse button is currently pressed, false otherwise</param>
		void SetRightPressed(bool isRightPressed) { m_IsRightPressed = isRightPressed; }

		/// <summary>
		/// Sets the state of the middle mouse button
		/// </summary>
		/// <param name="isLeftPressed">Is true if the middle mouse button is currently pressed, false otherwise</param>
		void SetMiddlePressed(bool isMiddlePressed) { m_IsMiddlePressed = isMiddlePressed; }

	private:
		/// <summary>
		/// True if the left mouse button is pressed
		/// </summary>
		bool m_IsLeftPressed;
		
		/// <summary>
		/// True if the middle mouse button is pressed
		/// </summary>
		bool m_IsMiddlePressed;
		
		/// <summary>
		/// True if the right mouse button is pressed
		/// </summary>
		bool m_IsRightPressed;

		/// <summary>
		/// Current mouse position, set in SetPos()
		/// </summary>
		Point2 m_MousePos;
	};
}


