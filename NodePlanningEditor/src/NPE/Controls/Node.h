#pragma once

#include "pch.h"



namespace NPE
{
	class MainWindow;


	struct NPoint
	{
		int x, y;

		NPoint& operator+(const NPoint& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		NPoint& operator+=(const NPoint& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}
	};

	struct NSize
	{
		int width, height;
	};


	class Node
	{
	public:
		Node(MainWindow* parent, const NPoint pos, const NSize size);

		void AdjustPos(NPoint posToAdd);

		NPoint GetPos() const { return m_Pos; }
		NSize GetSize() const { return m_Size; }
		float GetScale() const { return m_Scale; }

		HWND GetNativeWindow() const { return m_hWnd; }

	private:
		HWND m_hWnd;
		NPoint m_Pos;
		NSize m_Size;
		float m_Scale;
	};
}



