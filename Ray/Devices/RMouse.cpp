#include "RMouse.h"

#include "Components/Base/RWidget.h"


namespace At0::Ray
{
	Float2 Mouse::m_MousePos{};
	bool Mouse::m_IsLeftPressed = false;
	bool Mouse::m_IsMiddlePressed = false;
	bool Mouse::m_IsRightPressed = false;

	bool Mouse::IsLeftPressed() { return m_IsLeftPressed; }

	bool Mouse::IsMiddlePressed() { return m_IsMiddlePressed; }

	bool Mouse::IsRightPressed() { return m_IsRightPressed; }

	bool Mouse::IsOnWidget(const Widget& widget)
	{
		float width = widget.GetWidth();
		float height = widget.GetHeight();
		Float2 translation = widget.GetTranslation();

		return (m_MousePos.x >= translation.x && m_MousePos.x < translation.x + width &&
				m_MousePos.y >= translation.y && m_MousePos.y < translation.y + height);
	}

	const Float2& Mouse::GetPos() { return m_MousePos; }

	void Mouse::SetPos(const Float2& mousePos) { m_MousePos = mousePos; }
}  // namespace At0::Ray
