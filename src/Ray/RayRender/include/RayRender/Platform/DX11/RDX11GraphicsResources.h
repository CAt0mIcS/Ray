#pragma once

#include <atomic>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGIFactory;


namespace At0::Ray
{
	class RR_API DX11GraphicsResources
	{
	protected:
		DX11GraphicsResources();
		virtual ~DX11GraphicsResources();

		ID3D11Device* GetDevice() { return s_pDevice; }
		ID3D11DeviceContext* GetContext() { return s_pContext; }
		IDXGIFactory* GetDXGIFactory() { return s_pDXGIFactory; }

	private:
		static ID3D11Device* s_pDevice;
		static ID3D11DeviceContext* s_pContext;
		static IDXGIFactory* s_pDXGIFactory;

		static std::atomic<uint32_t> s_RefCount;
	};
}