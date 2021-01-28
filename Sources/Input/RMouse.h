#pragma once

#include "../RBase.h"
#include "../Utils/RMath.h"


namespace At0::Ray
{
	//class Widget;
	//class Window;

	class RAY_EXPORT MouseInput
	{
		friend class Window;
	public:
		MouseInput();
		~MouseInput();

		const Float2& GetPos() const;

		bool IsLeftPressed() const;
		bool IsMiddlePressed() const;
		bool IsRightPressed() const;

		//bool IsOnWidget(const Widget& widget) const;
		//bool IsOnWindow(const Window& window) const;

	private:
		void SetPos(const Float2& mousePos);

		void SetLeftPressed(bool isLeftPressed) { m_IsLeftPressed = isLeftPressed; }
		void SetRightPressed(bool isRightPressed) { m_IsRightPressed = isRightPressed; }
		void SetMiddlePressed(bool isMiddlePressed) { m_IsMiddlePressed = isMiddlePressed; }

	private:
		Float2 m_MousePos;

		bool m_IsLeftPressed;
		bool m_IsMiddlePressed;
		bool m_IsRightPressed;
	};
}
