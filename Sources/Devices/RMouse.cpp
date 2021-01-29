#include "Rpch.h"
#include "RMouse.h"

#include "Debug/RLogger.h"
#include "Debug/RAssert.h"


namespace At0::Ray
{
	bool Mouse::IsLeftPressed()
	{
		return m_IsLeftPressed;
	}

	bool Mouse::IsMiddlePressed()
	{
		return m_IsMiddlePressed;
	}

	bool Mouse::IsRightPressed()
	{
		return m_IsRightPressed;
	}

	//bool MouseInput::IsOnWindow(const Window& window) const
	//{
	//	Float2 widgetPos = window.GetPos();
	//	Float2 widgetSize = window.GetSize();

	//	// Mouse position is stored in window pixel coordinates.
	//	// We need to transform the window pos/size to match
	//	widgetSize -= widgetPos;
	//	widgetPos = { 0.0f, 0.0f };

	//	return (m_MousePos.x > widgetPos.x) && (m_MousePos.x < widgetPos.x + widgetSize.x) &&
	//		(m_MousePos.y > widgetPos.y) && (m_MousePos.y < widgetPos.y + widgetSize.y);
	//}

	//bool MouseInput::IsOnWidget(const Widget& widget) const
	//{
	//	Float2 widgetPos = widget.GetPos();
	//	Float2 widgetSize = widget.GetSize();

	//	return (m_MousePos.x > widgetPos.x) && (m_MousePos.x < widgetPos.x + widgetSize.x) &&
	//		(m_MousePos.y > widgetPos.y) && (m_MousePos.y < widgetPos.y + widgetSize.y);
	//}

	const Float2& Mouse::GetPos()
	{
		return m_MousePos;
	}

	void Mouse::SetPos(const Float2& mousePos)
	{
		m_MousePos = mousePos;
	}
}
