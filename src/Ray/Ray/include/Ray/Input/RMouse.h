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
		MouseInput();
		~MouseInput();

		const Point2& GetPos() const;

		bool IsLeftPressed() const;
		bool IsMiddlePressed() const;
		bool IsRightPressed() const;
		
		bool IsOnWidget(const Widget* widget) const;
		bool IsOnWidget(const Scope<Widget>& widget) const;

	private:
		void SetPos(const Point2& mousePos);
		
		void SetLeftPressed(bool isLeftPressed) { m_IsLeftPressed = isLeftPressed; }
		void SetRightPressed(bool isRightPressed) { m_IsRightPressed = isRightPressed; }
		void SetMiddlePressed(bool isMiddlePressed) { m_IsMiddlePressed = isMiddlePressed; }

	private:
		Point2 m_MousePos;
		
		bool m_IsLeftPressed;
		bool m_IsMiddlePressed;
		bool m_IsRightPressed;
	};
}


