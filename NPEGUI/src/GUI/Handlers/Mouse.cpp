#include "pch.h"
#include "Mouse.h"
#include "GUI/Controls/Control.h"

#include "GUI/Graphics/Renderer.h"


namespace GUI
{
	bool Mouse::m_IsLeftPressed = false;
	bool Mouse::m_IsMiddlePressed = false;
	bool Mouse::m_IsRightPressed = false;
	Util::NPoint Mouse::m_Pos = { 0.0f, 0.0f };

	Util::NPoint Mouse::GetTransformedPos()
	{
		auto matrix = Renderer::Get().GetInverseViewMatrix();
		Util::NPoint pos;

		pos.x = (m_Pos.x * matrix.m11 + m_Pos.y * matrix.m21 + matrix.dx);
		pos.y = (m_Pos.x * matrix.m12 + m_Pos.y * matrix.m22 + matrix.dy);
		return pos;
	}

	bool Mouse::IsOnControl(_In_ const Control* const control)
	{
		const Util::NPoint& controlPos = control->GetPos();
		const Util::NSize& controlSize = control->GetSize();

		return m_Pos.x > controlPos.x && m_Pos.y > controlPos.y && m_Pos.x < controlPos.x + controlSize.width && m_Pos.y < controlPos.y + controlSize.height;

	}
	bool Mouse::IsOnTransformedControl(_In_ const Control* const control)
	{
		const Util::NPoint& controlPos = control->GetPos();
		const Util::NSize& controlSize = control->GetSize();

		Util::NPoint pos = GetTransformedPos();
		return pos.x > controlPos.x && pos.y > controlPos.y && pos.x < controlPos.x + controlSize.width && pos.y < controlPos.y + controlSize.height;
	}
}