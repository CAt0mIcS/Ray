#ifndef RLR_D3D11RENDERERAPI_H
#define RLR_D3D11RENDERERAPI_H


#include "RlRender/RendererAPI.h"
#include "RlRender/RlRBase.h"

#ifdef _WIN32
#include <wrl.h>
#endif

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
#ifdef _WIN32
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_TargetView;

		HWND m_hWnd;
#endif // _WIN32
	};
}



#endif // RLR_D3D11RENDERERAPI_H