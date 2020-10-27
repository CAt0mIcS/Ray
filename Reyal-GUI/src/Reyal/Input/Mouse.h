#ifndef RL_MOUSE_H
#define RL_MOUSE_H

#include "RlBase.h"

#include "Reyal/Core/Vec2.h"


namespace At0::Reyal
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

		const Point& GetPos() const;
		const Point& GetOldPos() const;
		void SetPos(const Point& mousePos);

	private:
		bool m_IsLeftPressed;
		bool m_IsMiddlePressed;
		bool m_IsRightPressed;

		Point m_MousePos;
		Point m_OldMousePos;
	};
}



#endif /* RL_MOUSE_H */
