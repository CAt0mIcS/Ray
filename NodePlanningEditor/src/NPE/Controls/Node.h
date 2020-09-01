#pragma once

#include "pch.h"
#include "Control.h"

#include "NPE/Util/Props.h"


namespace NPE
{
	class MainWindow;

	class Node : public Control
	{
	public:
		Node(MainWindow* parent, const NPoint pos, const NSize size);

		LRESULT CALLBACK HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	private:
		static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMessagePass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	};
}



