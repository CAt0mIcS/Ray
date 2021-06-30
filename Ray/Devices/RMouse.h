#pragma once

#include "Ray/RBase.h"
#include "Ray/Core/RMath.h"


namespace At0::Ray
{
	class Widget;

	class RAY_EXPORT Mouse
	{
		friend class Window;

	public:
		static const Float2& GetPos();

		static bool IsLeftPressed();
		static bool IsMiddlePressed();
		static bool IsRightPressed();

		static bool IsOnWidget(const Widget& widget);

	private:
		static void SetPos(const Float2& mousePos);

		static void SetLeftPressed(bool isLeftPressed) { m_IsLeftPressed = isLeftPressed; }
		static void SetRightPressed(bool isRightPressed) { m_IsRightPressed = isRightPressed; }
		static void SetMiddlePressed(bool isMiddlePressed) { m_IsMiddlePressed = isMiddlePressed; }

	private:
		static Float2 m_MousePos;

		static bool m_IsLeftPressed;
		static bool m_IsMiddlePressed;
		static bool m_IsRightPressed;
	};
}  // namespace At0::Ray
