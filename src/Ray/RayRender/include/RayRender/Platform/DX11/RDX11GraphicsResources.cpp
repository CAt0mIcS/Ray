#include "RayRender/RRpch.h"
#include "RDX11GraphicsResources.h"

#include <RayUtil/RException.h>


namespace At0::Ray
{
	ID3D11Device* DX11GraphicsResources::s_pDevice = nullptr;
	ID3D11DeviceContext* DX11GraphicsResources::s_pContext = nullptr;
	IDXGIFactory* DX11GraphicsResources::s_pDXGIFactory = nullptr;

	std::atomic<uint32_t> DX11GraphicsResources::s_RefCount = 0;

	DX11GraphicsResources::DX11GraphicsResources()
	{
		// RAY_TODO: Better check whether DDX11GraphicsResources are initialized
		if (!s_pDevice)
		{
			// Create the device
			RAY_GFX_THROW_FAILED(D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				NULL,
				0,
				nullptr,
				0,
				D3D11_SDK_VERSION,
				&s_pDevice,
				nullptr,
				&s_pContext
			));

			// Create IDXGIDevice to create swap chains in the Renderer constructor
			IDXGIDevice* pDevice;
			RAY_GFX_THROW_FAILED(s_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice));

			IDXGIAdapter* pAdapter;
			RAY_GFX_THROW_FAILED(pDevice->GetAdapter(&pAdapter));

			RAY_GFX_THROW_FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&s_pDXGIFactory));

			pDevice->Release();
			pAdapter->Release();
		}

		++s_RefCount;
	}

	DX11GraphicsResources::~DX11GraphicsResources()
	{
		--s_RefCount;
		if (s_RefCount == 0)
		{
			while (s_pDevice->Release());
			while (s_pContext->Release());
			while (s_pDXGIFactory->Release());
		}
	}
}