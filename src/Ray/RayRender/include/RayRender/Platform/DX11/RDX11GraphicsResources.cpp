#include "RayRender/RRpch.h"

#ifdef _WIN32
#include "RDX11GraphicsResources.h"

#include <RayUtil/RException.h>
#include <RayDebug/RLogger.h>


namespace At0::Ray
{
	ID3D11Device* DX11GraphicsResources::s_pDevice = nullptr;
	ID3D11DeviceContext* DX11GraphicsResources::s_pContext = nullptr;
	IDXGIFactory* DX11GraphicsResources::s_pDXGIFactory = nullptr;

	std::atomic<uint64_t> DX11GraphicsResources::s_RefCount = 0;

	DX11GraphicsResources::DX11GraphicsResources()
	{
		// RAY_TODO: Better check whether DDX11GraphicsResources are initialized
		if (!s_pDevice)
		{
			uint32_t flags =
#ifndef NDEBUG
				D3D11_CREATE_DEVICE_DEBUG;
#else
				0;
#endif

			RAY_GFX_THROW_FAILED(D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				NULL,
				flags,
				nullptr,
				0,
				D3D11_SDK_VERSION,
				&s_pDevice,
				nullptr,
				&s_pContext
			));

			Microsoft::WRL::ComPtr<IDXGIDevice> pDevice;
			RAY_GFX_THROW_FAILED(s_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice));

			Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter;
			RAY_GFX_THROW_FAILED(pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter));

			RAY_GFX_THROW_FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&s_pDXGIFactory));

			for (auto i = 0; s_pDXGIFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
			{
				DXGI_ADAPTER_DESC adapterDesc;
				RAY_GFX_THROW_FAILED(pAdapter->GetDesc(&adapterDesc));
				Log::Info("[Renderer3D] Found DXGIAdapter: "
					"\n\tDescription: {0}\n\tVendorID: {1}\n\tDeviceID: {2}\n\tSubSysID: {3}"
					"\n\tRevision: {4}\n\tDedicated Video Memory: {5}\n\tDedicated System Memory: {6}"
					"\n\tShared System Memory: {7}\n\tAdapterLuid::LowPart: {8}\n\tAdapterLuid::HighPart: {9}\n",
					adapterDesc.Description,
					adapterDesc.VendorId,
					adapterDesc.DeviceId,
					adapterDesc.SubSysId,
					adapterDesc.Revision,
					adapterDesc.DedicatedVideoMemory,
					adapterDesc.DedicatedSystemMemory,
					adapterDesc.SharedSystemMemory,
					adapterDesc.AdapterLuid.LowPart,
					adapterDesc.AdapterLuid.HighPart
				);
			}
		}

		++s_RefCount;
	}

	DX11GraphicsResources::~DX11GraphicsResources()
	{
		// RAY_TODO: Some graphics resources are left at the end of the program!

		--s_RefCount;
		if (s_RefCount == 0)
		{
			OutputDebugStringA("\n------------ DirectX11 Debug Information ------------\n");
			Microsoft::WRL::ComPtr<ID3D11Debug> pDebug;
			s_pDevice->QueryInterface(__uuidof(ID3D11Debug), &pDebug);
			pDebug->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL);

			s_pDevice->Release();
			s_pContext->Release();
			s_pDXGIFactory->Release();
		}
	}
}

#endif // _WIN32