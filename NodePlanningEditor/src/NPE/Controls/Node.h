#pragma once

#include "pch.h"



namespace NPE
{
	class MainWindow;

	class Node
	{
	public:
		Node(MainWindow* parent);

	private:
		HWND m_hWnd;
	};
}



