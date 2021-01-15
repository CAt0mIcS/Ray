#pragma once

#ifdef _WIN32
#include <atomic>
#include <stdint.h>

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

		const ID3D11Device* GetDevice() const { return s_pDevice; }
		const ID3D11DeviceContext* GetContext() const { return s_pContext; }
		const IDXGIFactory* GetDXGIFactory() const { return s_pDXGIFactory; }

	private:
		static ID3D11Device* s_pDevice;
		static ID3D11DeviceContext* s_pContext;
		static IDXGIFactory* s_pDXGIFactory;

		static std::atomic<uint64_t> s_RefCount;
	};
}

#endif // _WIN32