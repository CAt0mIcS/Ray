#include "rlrpch.h"
#include "GraphicsResource.h"

#include <RlUtil/Exception.h>
#include <RlDebug/ReyalLogger.h>


namespace WRL = Microsoft::WRL;

namespace At0::Reyal
{
	std::atomic<int> GraphicsResource::s_RefCount = 0;

	ID3D11Device* GraphicsResource::s_pDevice = nullptr;
	ID3D11DeviceContext* GraphicsResource::s_pContext = nullptr;
	IDXGIFactory* GraphicsResource::s_pIDXGIFactory = nullptr;

	GraphicsResource::GraphicsResource()
	{
#ifdef NDEBUG
		uint32_t flags = 0;
#else
		uint32_t flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

		// Assume that if there is no device then context and IDXGI factory also don't exist
		if (!s_pDevice)
		{
			RL_GFX_THROW_FAILED(D3D11CreateDevice(
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

			WRL::ComPtr<IDXGIDevice> pDevice;
			RL_GFX_THROW_FAILED(s_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice));

			WRL::ComPtr<IDXGIAdapter> pAdapter;
			RL_GFX_THROW_FAILED(pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter));

			RL_GFX_THROW_FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&s_pIDXGIFactory));
			
			for (auto i = 0; s_pIDXGIFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
			{
				DXGI_ADAPTER_DESC adapterDesc;
				RL_GFX_THROW_FAILED(pAdapter->GetDesc(&adapterDesc));
				RL_LOG_INFO("[Renderer3D] Found DXGIAdapter: "
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
	
	GraphicsResource::~GraphicsResource()
	{
		--s_RefCount;
		if (s_RefCount == 0)
		{
			s_pContext->Release();
			s_pIDXGIFactory->Release();

#ifndef NDEBUG
			WRL::ComPtr<ID3D11Debug> pDebug;
			s_pDevice->QueryInterface(__uuidof(ID3D11Debug), &pDebug);
			pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
			
			s_pDevice->Release();
		}
	}
}

