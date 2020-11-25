#include "rlrpch.h"

#ifdef _WIN32

#include "D3D11GraphicsResource.h"

#include "RlRBase.h"

#include <d3d11.h>

#include <RlDebug/ReyalLogger.h>
#include <RlDebug/Instrumentor.h>
#include <RlDebug/RlAssert.h>

#include <RlUtil/Exception.h>
#include "D3D11GraphicsResource.h"

namespace WRL = Microsoft::WRL;


namespace At0::Reyal
{
	WRL::ComPtr<ID3D11Device> D3D11GraphicsResource::s_Device = nullptr;
	WRL::ComPtr<ID3D11DeviceContext> D3D11GraphicsResource::s_Context = nullptr;
	WRL::ComPtr<IDXGIFactory> D3D11GraphicsResource::s_IDXGIFactory = nullptr;
}

#endif // _WIN32