#pragma once

#include "RlBase.h"

#include "Reyal/Renderer/RendererAPI.h"
#include <wrl.h>


namespace At0::Reyal
{
	class D3D11RendererAPI : public RendererAPI
	{
	public:
		/// <summary>
		/// D3D11RendererAPI Constructor
		/// </summary>
		D3D11RendererAPI();

		/// <summary>
		/// Initializes all components required for rendering
		/// <param name="window">Is the platform specific window handle</param>
		/// </summary>
		virtual void Init(_In_ void* window) override;

		/// <summary>
		/// DEBUG: TODO: Renders test triangle onto the screen
		/// </summary>
		virtual void RenderTestTriangle() override;

		/// <summary>
		/// Draws the scene to the screen
		/// </summary>
		virtual void EndDraw() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_TargetView;

		HWND m_hWnd;
	};
}



