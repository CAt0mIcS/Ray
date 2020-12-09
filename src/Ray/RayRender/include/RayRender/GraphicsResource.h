#pragma once

#include <wrl.h>
#include <atomic>

struct IDXGIFactory;
struct ID3D11Device;
struct ID3D11DeviceContext;


namespace At0::Ray
{
	class GraphicsResource
	{
	public:
		static int GetDerivedCount() { return s_RefCount; }

	protected:
		GraphicsResource();
		virtual ~GraphicsResource();

	protected:
		static ID3D11Device* s_pDevice;
		static ID3D11DeviceContext* s_pContext;
		static IDXGIFactory* s_pIDXGIFactory;

	private:
		static std::atomic<uint32_t> s_RefCount;
	};
}