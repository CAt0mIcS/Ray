#pragma once

#include "NPE/Util/Props.h"


namespace NPE
{
	class Control;

	class Mouse
	{
	public:
		Mouse() = delete;

		static void SetPos(const NPoint& pos) { m_Pos = pos; }
		static const NPoint& GetPos() { return m_Pos; }

		static void SetLeftPressed(bool pressed) { m_IsLeftPressed = pressed; }
		static bool IsLeftPressed() { return m_IsLeftPressed; }

		static void SetMiddlePressed(bool pressed) { m_IsMiddlePressed = pressed; }
		static bool IsMiddlePressed() { return m_IsMiddlePressed; }
		
		static void SetRightPressed(bool pressed) { m_IsRightPressed = pressed; }
		static bool IsRightPressed() { return m_IsRightPressed; }

		static bool IsOnControl(const Control* const control);

		static void ClearStates() { m_IsLeftPressed = false; m_IsMiddlePressed = false; m_IsRightPressed = false; }

	private:
		static NPoint m_Pos;
		static bool m_IsLeftPressed;
		static bool m_IsRightPressed;
		static bool m_IsMiddlePressed;
	};

	inline bool Mouse::m_IsLeftPressed = false;
	inline bool Mouse::m_IsMiddlePressed = false;
	inline bool Mouse::m_IsRightPressed = false;
	inline NPoint Mouse::m_Pos = { 0, 0 };
}

