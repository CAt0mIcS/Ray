#pragma once

#ifdef _WIN32


#include "RlRender/RendererAPI.h"
#include "RlRender/RlRBase.h"
#include "D3D11GraphicsResource.h"

#include <wrl.h>

namespace At0::Reyal
{
	class D3D11RendererAPI : public RendererAPI, private D3D11GraphicsResource
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
		virtual void Init(void* window) override;

		/// <summary>
		/// Checks if the renderer API was initialized
		/// </summary>
		/// <returns>True if initialized, false otherwise</returns>
		virtual bool IsInitialized() const override;

		/// <summary>
		/// DEBUG: TODO: Renders test triangle onto the screen
		/// </summary>
		virtual void RenderTestTriangle() override;

		/// <summary>
		/// Draws the scene to the screen
		/// </summary>
		virtual void EndDraw() override;

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_TargetView;

		HWND m_hWnd;
	};
}

#endif // _WIN32

