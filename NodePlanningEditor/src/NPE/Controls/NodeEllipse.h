#pragma once

#include "NPE/Util/Props.h"

class MainWindow;


namespace NPE
{
	class NodeEllipse
	{
	public:
		NodeEllipse(int* parent, const NPoint& pos, const NSize& size);

		void MoveBy(const NPoint& pos);
		void MoveTo(const NPoint& pos);

		void ResizeBy(const NSize& size);
		void ResizeTo(const NSize& size);

		NPoint GetPos() const { return m_Pos; }
		NSize GetSize() const { return m_Size; }

	private:
		NPoint m_Pos;
		NSize m_Size;
		MainWindow* m_Parent;
	};
}



