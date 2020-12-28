#pragma once

#include "RlRBase.h"

#include "GraphicsResource.h"

#include <wrl.h>
#include <Windows.h>
#include <DirectXMath.h>

// DEBUG
#include <mutex>
#include <d3d11.h>
#include <dxgi.h>

#include <../../RayEvent/include/RayEvent/REventListener.h>
#include <../../RayEvent/include/RayEvent/Events/RApplicationEvent.h>


struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace At0::Ray
{
	class ThreadSafeSwapChain
	{
	public:
		ThreadSafeSwapChain(Microsoft::WRL::ComPtr<IDXGISwapChain>& chain)
			: m_pSwapChain(chain)
		{
			m_Mutex.lock();
		}

		IDXGISwapChain** operator&()
		{
			return &m_pSwapChain;
		}

		IDXGISwapChain** GetAddressOf()
		{
			return m_pSwapChain.GetAddressOf();
		}

		IDXGISwapChain* Get()
		{
			return m_pSwapChain.Get();
		}

		IDXGISwapChain* operator->()
		{
			return m_pSwapChain.Get();
		}

		~ThreadSafeSwapChain()
		{
			m_Mutex.unlock();
		}

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain>& m_pSwapChain;
		inline static std::mutex m_Mutex;
	};

	class ThreadSafeRenderTarget
	{
	public:
		ThreadSafeRenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& view)
			: m_pView(view)
		{
			m_Mutex.lock();
		}

		~ThreadSafeRenderTarget()
		{
			m_Mutex.unlock();
		}

		ID3D11RenderTargetView* operator->() { return m_pView.Get(); }
		ID3D11RenderTargetView* Get() { return m_pView.Get(); }
		ID3D11RenderTargetView** GetAddressOf() { return m_pView.GetAddressOf(); }
		ID3D11RenderTargetView** Release() { return m_pView.ReleaseAndGetAddressOf(); }

		ID3D11RenderTargetView** operator&() { return &m_pView; }

	private:
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& m_pView;
		inline static std::mutex m_Mutex;
	};

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
		void SetCamera(const DirectX::FXMMATRIX& cam) { m_Camera = cam; }
		const auto GetCamera() const { return m_Camera; }

	private:
		virtual void OnEvent(Widget* receiver, WindowResizeEvent& e) override;

		ThreadSafeRenderTarget GetTarget() { return ThreadSafeRenderTarget{ m_pTarget }; }
		ThreadSafeSwapChain GetSwapChain() { return ThreadSafeSwapChain{ m_pSwapChain }; }
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDSV;

		DirectX::XMMATRIX m_Projection;
		DirectX::XMMATRIX m_Camera;
		HWND m_hWnd;
	};
}
