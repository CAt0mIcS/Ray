#pragma once

#include "NPE/Util/Props.h"

#include "NPE/Controls/Control.h"

#include <string>

#include <wrl.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")


namespace NPE
{
	class Renderer2D
	{
	public:
		Renderer2D();
		void Init(HWND hWnd, const unsigned int fontSize);

		void RenderControl(Control& control);
		void RenderText(const std::wstring text, const NPoint& pos, const NSize& size);

		void RenderScene(const NColor& color);

	private:
		void CreateGraphicsResources(const unsigned int fontSize);

	private:
		HWND m_hWnd;

		Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush > m_pBrush;
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_pRenderTarget;
		Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTextFormat;
		Microsoft::WRL::ComPtr<IDWriteFactory> m_pWriteFactory;
	};
}



