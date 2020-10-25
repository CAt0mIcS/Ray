#pragma once

#include "RlBase.h"

#include "Reyal/Core/Vec2.h"


namespace Zeal::Reyal
{
	class Widget;

	class RL_API Mouse
	{
	public:
		Mouse();

		bool IsLeftPressed() const;
		bool IsMiddlePressed() const;
		bool IsRightPressed() const;

		bool IsOnWidget(const Widget* widget) const;

		const Point& GetMousePos() const;
		void SetMousePos(const Point& mousePos);

	private:
		bool m_IsLeftPressed;
		bool m_IsMiddlePressed;
		bool m_IsRightPressed;

		Point m_MousePos;
	};
}



