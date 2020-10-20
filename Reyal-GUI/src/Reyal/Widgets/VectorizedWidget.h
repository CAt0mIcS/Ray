#pragma once

#include "RlBase.h"
#include "ZWidget.h"

#include <Util/ZRect.h>


namespace Zeal::Reyal
{
	class RL_API VectorizedWidget : public Widget
	{
	public:
		virtual void MoveBy(const Util::Point& pos) override;
		virtual void MoveTo(const Util::Point& pos) override;
		virtual void ResizeBy(const Util::Size& size) override;
		virtual void ResizeTo(const Util::Size& size) override;

		const Util::ZRect& GetRect() const { return m_Rect; }

	protected:
		VectorizedWidget(const std::wstring_view name, _In_opt_ Widget* parent = nullptr);

	protected:
		Util::ZRect m_Rect;
	};
}


