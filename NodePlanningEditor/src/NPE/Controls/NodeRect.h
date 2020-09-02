#pragma once

#include "NPE/Util/Props.h"


namespace NPE
{
	class MainWindow;

	class NodeRect
	{
	public:
		NodeRect(MainWindow* parent, const NPoint& pos, const NSize& size);

		unsigned short GetId() const { return m_Id; }

		void MoveBy(const NPoint& pos);
		void MoveTo(const NPoint& pos);

		void ResizeBy(const NSize& size);
		void ResizeTo(const NSize& size);

		const D2D1_ROUNDED_RECT& GetRect() const { return m_RoundedRect; }

		NPoint GetPos() const { return { m_RoundedRect.rect.left, m_RoundedRect.rect.top }; }
		NSize GetSize() const { return { m_RoundedRect.rect.right - m_RoundedRect.rect.left, m_RoundedRect.rect.bottom - m_RoundedRect.rect.top }; }

	private:
		D2D1_ROUNDED_RECT m_RoundedRect;
		int m_Id;

		static int m_NextId;
	};
}



