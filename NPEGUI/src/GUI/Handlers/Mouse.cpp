#include "pch.h"
#include "Mouse.h"
#include "GUI/Controls/Control.h"


namespace GUI
{
	bool Mouse::m_IsLeftPressed = false;
	bool Mouse::m_IsMiddlePressed = false;
	bool Mouse::m_IsRightPressed = false;
	Util::NPoint Mouse::m_Pos = { 0.0f, 0.0f };

	bool Mouse::IsOnControl(const Control* const control)
	{
		const Util::NPoint& controlPos = control->GetPos();
		const Util::NSize& controlSize = control->GetSize();

		return m_Pos.x > controlPos.x && m_Pos.y > controlPos.y && m_Pos.x < controlPos.x + controlSize.width && m_Pos.y < controlPos.y + controlSize.height;

	}
}