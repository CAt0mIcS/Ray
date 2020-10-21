#pragma once
#include "RlBase.h"

#include "RlWin.h"
#include <wrl.h>

#include "Renderer.h"


namespace Zeal::Reyal
{
	class RL_API WindowRenderer : public Renderer
	{
	public:
		/// <summary>
		/// WindowRenderer Constructor
		/// </summary>
		WindowRenderer();

		/// <summary>
		/// Initializes the WindowRenderer
		/// </summary>
		/// <param name="hWnd">Is the Window which the WindowRenderer will render</param>
		void Init(HWND hWnd);

	private:
		/// <summary>
		/// Creates all private resources
		/// </summary>
		void CreateGraphicsResources();

	private:
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_pRenderTarget;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pSolidBrush;

		HWND m_hWnd;
	};
}




