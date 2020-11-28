#pragma once

#include <wrl.h>

struct IDXGIFactory;
struct ID3D11Device;
struct ID3D11DeviceContext;


namespace At0::Reyal
{
	class GraphicsResource
	{
	protected:
		GraphicsResource();

	protected:
		static Microsoft::WRL::ComPtr<ID3D11Device> s_pDevice;
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext> s_pContext;
		static Microsoft::WRL::ComPtr<IDXGIFactory> s_pIDXGIFactory;
	};
}