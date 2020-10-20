#include "rlpch.h"
#include "TransformedWidget.h"


namespace Zeal::Reyal
{
	void TransformedWidget::MoveBy(const Util::Point& pos)
	{
		m_Matrix = m_Matrix * D2D1::Matrix3x2F::Translation(pos);
	}

	void TransformedWidget::MoveTo(const Util::Point& pos)
	{
		m_Matrix = D2D1::Matrix3x2F::Translation(pos);
	}

	void TransformedWidget::ResizeBy(const Util::Size& size)
	{
		m_Matrix = m_Matrix * D2D1::Matrix3x2F::Scale(size);
	}

	void TransformedWidget::ResizeTo(const Util::Size& size)
	{
		m_Matrix = D2D1::Matrix3x2F::Scale(size);
	}

	TransformedWidget::TransformedWidget(const std::wstring_view name, _In_opt_ Widget* parent)
		: PositionedWidget(name, parent)
	{
	}
}



