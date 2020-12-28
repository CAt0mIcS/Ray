#pragma once

#include <wrl.h>
#include <atomic>

// DEBUG
#include <mutex>

struct IDXGIFactory;
struct ID3D11Device;
struct ID3D11DeviceContext;


namespace At0::Ray
{
	class ThreadSafeContext
	{
	public:
		ThreadSafeContext(ID3D11DeviceContext* pContext)
			: m_pContext(pContext)
		{
			m_Mutex.lock();
		}

		~ThreadSafeContext()
		{
			m_Mutex.unlock();
		}

		ID3D11DeviceContext* operator->()
		{
			return m_pContext;
		}

	private:
		ID3D11DeviceContext* m_pContext;
		inline static std::mutex m_Mutex;
	};

	class GraphicsResource
	{
	public:
		static int GetDerivedCount() { return s_RefCount; }

	protected:
		GraphicsResource();
		virtual ~GraphicsResource();

		static ThreadSafeContext GetContext() { return ThreadSafeContext{ s_pContext }; }

	protected:
		static ID3D11Device* s_pDevice;
		static ID3D11DeviceContext* s_pContext;
		static IDXGIFactory* s_pIDXGIFactory;

	private:
		static std::atomic<uint32_t> s_RefCount;
	};
}