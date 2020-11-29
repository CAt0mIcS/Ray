#pragma once

#include <wrl.h>
#include <atomic>

struct IDXGIFactory;
struct ID3D11Device;
struct ID3D11DeviceContext;


namespace At0::Reyal
{
	class GraphicsResource
	{
	public:
		static int GetDerivedCount() { return s_RefCount; }

	protected:
		GraphicsResource();
		~GraphicsResource();

	protected:
		static ID3D11Device* s_pDevice;
		static ID3D11DeviceContext* s_pContext;
		static IDXGIFactory* s_pIDXGIFactory;

	private:
		static std::atomic<int> s_RefCount;
	};
}