#include "rlpch.h"
#include "VectorizedWidget.h"


namespace Zeal::Reyal
{
	void VectorizedWidget::MoveBy(const Util::Point& pos)
	{
		m_Rect.left += pos.x;
		m_Rect.top += pos.y;
	}

	void VectorizedWidget::MoveTo(const Util::Point& pos)
	{
		m_Rect.left = pos.x;
		m_Rect.top = pos.y;
	}

	void VectorizedWidget::ResizeBy(const Util::Size& size)
	{
		m_Rect.right += m_Rect.left + size.width;
		m_Rect.bottom += m_Rect.top + size.height;
	}

	void VectorizedWidget::ResizeTo(const Util::Size& size)
	{
		m_Rect.right = m_Rect.left + size.width;
		m_Rect.bottom = m_Rect.top + size.height;
	}

	VectorizedWidget::VectorizedWidget(const std::wstring_view name, WindowRenderer& renderer, Widget* parent)
		: PositionedWidget(name, renderer, parent)
	{

	}
}


