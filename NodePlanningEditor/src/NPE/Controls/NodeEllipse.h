#pragma once

#include "NPE/Util/Props.h"


namespace NPE
{
	class MainWindow;

	class NodeEllipse
	{
	public:
		NodeEllipse(MainWindow* parent, const NPoint& pos, const NSize& size);

		unsigned short GetId() const { return m_Id; }

		void MoveBy(const NPoint& pos);
		void MoveTo(const NPoint& pos);

		void ResizeBy(const NSize& size);
		void ResizeTo(const NSize& size);

		void Draw(HDC hDC);

		NPoint GetPos() const { return m_Pos; }
		NSize GetSize() const { return m_Size; }

	private:
		NPoint m_Pos;
		NSize m_Size;
		int m_Id;

		static int m_NextId;
	};
}



