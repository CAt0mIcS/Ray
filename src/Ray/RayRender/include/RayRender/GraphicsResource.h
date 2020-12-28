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
	template<typename T>
	class ThreadSafeComPtr
	{
	public:
		ThreadSafeComPtr(Microsoft::WRL::ComPtr<T>& ptr, std::mutex& mutex)
			: m_Ptr(ptr), m_Mutex(mutex)
		{
			m_Mutex.lock();
		}

		~ThreadSafeComPtr()
		{
			m_Mutex.unlock();
		}

		T* operator->()
		{
			return m_Ptr.Get();
		}

		T* Get()
		{
			return m_Ptr.Get();
		}

		T** GetAddressOf()
		{
			return m_Ptr.GetAddressOf();
		}

		T** operator&()
		{
			return &m_Ptr;
		}

		T** ReleaseAndGetAddressOf()
		{
			return m_Ptr.ReleaseAndGetAddressOf();
		}

	private:
		Microsoft::WRL::ComPtr<T>& m_Ptr;
		std::mutex& m_Mutex;
	};


	template<typename T>
	class ThreadSafeComObject
	{
	public:
		ThreadSafeComObject(T* ptr, std::mutex& mutex)
			: m_Ptr(ptr), m_Mutex(mutex)
		{
			m_Mutex.lock();
		}

		~ThreadSafeComObject()
		{
			m_Mutex.unlock();
		}

		T* operator->()
		{
			return m_Ptr;
		}

	private:
		T* m_Ptr;
		std::mutex& m_Mutex;
	};

	class GraphicsResource
	{
	public:
		static int GetDerivedCount() { return s_RefCount; }

	protected:
		GraphicsResource();
		virtual ~GraphicsResource();

		static ThreadSafeComObject<ID3D11DeviceContext> GetContext() { return ThreadSafeComObject<ID3D11DeviceContext>{ s_pContext, m_DeviceContextMutex }; }

	protected:
		static ID3D11Device* s_pDevice;
		static ID3D11DeviceContext* s_pContext;
		static IDXGIFactory* s_pIDXGIFactory;

		static inline std::mutex m_DeviceContextMutex;

	private:
		static std::atomic<uint32_t> s_RefCount;
	};
}