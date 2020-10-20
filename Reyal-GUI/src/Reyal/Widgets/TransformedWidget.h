#pragma once

#include "ZWidget.h"


namespace Zeal::Reyal
{
	class RL_API TransformedWidget : public Widget
	{
	public:
		virtual void MoveBy(const Util::Point& pos) override;
		virtual void MoveTo(const Util::Point& pos) override;
		virtual void ResizeBy(const Util::Size& size) override;
		virtual void ResizeTo(const Util::Size& size) override;

		const D2D1::Matrix3x2F& GetMatrix() const { return m_Matrix; }

	protected:
		TransformedWidget(const std::wstring_view name, _In_opt_ Widget* parent = nullptr);

	protected:
		D2D1::Matrix3x2F m_Matrix;
	};
}




