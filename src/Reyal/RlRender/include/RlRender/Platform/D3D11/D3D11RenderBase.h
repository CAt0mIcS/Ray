#pragma once

#ifdef _WIN32

#include <mutex>
#include <wrl.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGIFactory;
struct IDXGISwapChain;


namespace At0::Reyal
{
	/// <summary>
	/// Class which will contain all functions the internal RendererAPI will need to access static resources.
	/// This class should be thread-safe. (TODO: Check speed and make thread-safe)
	/// 
	/// The future plan for this class is to not have it be derivable and have thread-safe (mutex) functions
	/// for each operation requiring the static objects. For now the RendererAPIs will just derive from this
	/// class to access the not protected (in the future private) members
	/// </summary>
	class D3D11RenderBase
	{
	protected:
		D3D11RenderBase() = default;

	protected:
		//static std::mutex m_DeviceMutex;
		//static std::mutex m_ContextMutex;
		//static std::mutex m_IDXGIFactoryMutex;

		static Microsoft::WRL::ComPtr<ID3D11Device> s_Device;
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext> s_Context;
		static Microsoft::WRL::ComPtr<IDXGIFactory> s_IDXGIFactory;
	};
}

#endif // _WIN32

