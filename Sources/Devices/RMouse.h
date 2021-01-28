#pragma once

#include "../RBase.h"
#include "../Utils/RMath.h"


namespace At0::Ray
{
	//class Widget;
	//class Window;

	class RAY_EXPORT Mouse
	{
		friend class Window;
	public:
		static const Float2& GetPos();

		static bool IsLeftPressed();
		static bool IsMiddlePressed();
		static bool IsRightPressed();

		//bool IsOnWidget(const Widget& widget) const;
		//bool IsOnWindow(const Window& window) const;

	private:
		static void SetPos(const Float2& mousePos);

		static void SetLeftPressed(bool isLeftPressed) { m_IsLeftPressed = isLeftPressed; }
		static void SetRightPressed(bool isRightPressed) { m_IsRightPressed = isRightPressed; }
		static void SetMiddlePressed(bool isMiddlePressed) { m_IsMiddlePressed = isMiddlePressed; }

	private:
		inline static Float2 m_MousePos{};

		inline static bool m_IsLeftPressed = false;
		inline static bool m_IsMiddlePressed = false;
		inline static bool m_IsRightPressed = false;
	};
}
