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

	void Mouse::SetPos(_In_ const Util::NPoint& pos)
	{
		auto matrix = Renderer::Get().GetInverseViewMatrix(Renderer::Get().GetOrigin());
		m_Pos = pos;

		m_Pos.x = (m_Pos.x * matrix.m11 + m_Pos.y * matrix.m21 + matrix.dx);
		m_Pos.y = (m_Pos.x * matrix.m12 + m_Pos.y * matrix.m22 + matrix.dy);
	}

	bool Mouse::IsOnControl(_In_ const Control* const control)
	{
		const Util::NPoint& controlPos = control->GetPos();
		const Util::NSize& controlSize = control->GetSize();

		return m_Pos.x > controlPos.x && m_Pos.y > controlPos.y && m_Pos.x < controlPos.x + controlSize.width && m_Pos.y < controlPos.y + controlSize.height;

	}
}