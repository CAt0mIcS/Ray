#pragma once

#include "RlRBase.h"

#include "GraphicsResource.h"

#include <wrl.h>
#include <Windows.h>

// DEBUG
#include <mutex>
#include <d3d11.h>
#include <dxgi.h>

#include <../../RayEvent/include/RayEvent/REventListener.h>
#include <../../RayEvent/include/RayEvent/Events/RApplicationEvent.h>
#include <../../RayUtil/include/RayUtil/RMath.h>

#include <DirectXMath.h>


struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace At0::Ray
{
	class RR_API Renderer3D : private GraphicsResource,
		EventListener<WindowResizeEvent>
	{
	public:
		Renderer3D(EventDispatcher<WindowResizeEvent>& dispatcher);
		Renderer3D& operator=(const Renderer3D& other) = delete;
		Renderer3D(const Renderer3D& other) = delete;

		void Init(HWND hWnd);
		void DrawIndexed(uint32_t indicesCount);
		void ClearBuffer(float red, float green, float blue);
		void EndDraw();
		void SetProjection(const DirectX::XMMATRIX& mat) { m_Projection = mat; }
		const DirectX::XMMATRIX& GetProjection() const { return m_Projection; }
		~Renderer3D();
		void SetCamera(const DirectX::XMMATRIX& cam) { m_Camera = cam; }
		const auto GetCamera() const { return m_Camera; }

	private:
		virtual void OnEvent(Widget& receiver, WindowResizeEvent& e) override;

		ThreadSafeComPtr<ID3D11RenderTargetView> GetTarget() { return ThreadSafeComPtr<ID3D11RenderTargetView>{ m_pTarget, m_RenderTargetViewMutex }; }
		ThreadSafeComPtr<IDXGISwapChain> GetSwapChain() { return ThreadSafeComPtr<IDXGISwapChain>{ m_pSwapChain, m_SwapChainMutex }; }

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDSV;

		std::mutex m_RenderTargetViewMutex;
		std::mutex m_SwapChainMutex;

		DirectX::XMMATRIX m_Projection;
		DirectX::XMMATRIX m_Camera;
		HWND m_hWnd;
	};
}
