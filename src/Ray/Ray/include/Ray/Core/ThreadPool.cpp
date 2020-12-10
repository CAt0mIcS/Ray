#include "rpch.h"
#include "Ray/Core/ThreadPool.h"

#include <mutex>
#include <thread>

#include <RayDebug/RLogger.h>
#include <RayDebug/RInstrumentor.h>


namespace At0::Ray
{
	ThreadPool::ThreadPool()
		: m_Threads(MakeScope<std::thread[]>(m_MaxThreads)), m_Shutdown(false)
	{
		RAY_PROFILE_FUNCTION();

		RAY_LOG_INFO("[ThreadPool] Initialized {0} threads", MaxThreads());
		for (uint16_t i = 0; i < m_MaxThreads; ++i)
		{
			m_Threads[i] = std::thread([this]() { InfiniteWait(); });
		}
	}
	
	ThreadPool& ThreadPool::Get()
	{
		static ThreadPool instance;
		return instance;
	}

	void ThreadPool::Shutdown()
	{
		RAY_PROFILE_FUNCTION();

		RAY_LOG_DEBUG("[ThreadPool] Shutting down");
		{
			std::scoped_lock lock(m_PoolMutex);
			m_Shutdown = true;
		}
		m_TaskQueue.GetWaiter().notify_all();

		RAY_LOG_DEBUG("[ThreadPool] Joining Threads");
		for (uint16_t i = 0; i < m_MaxThreads; ++i)
		{
			auto id = m_Threads[i].get_id();
			RAY_LOG_DEBUG("[ThreadPool] Joining Thread {0}", id);
			m_Threads[i].join();
			RAY_LOG_DEBUG("[ThreadPool] Thread {0} joined", id);
		}
		RAY_LOG_DEBUG("[ThreadPool] Finnished joining Threads");
	}

	ThreadPool::~ThreadPool()
	{
		RAY_PROFILE_FUNCTION();

		if (!m_Shutdown)
		{
			RAY_LOG_DEBUG("[ThreadPool] Calling ThreadPool::Shutdown from Deconstructor");
			Shutdown();
		}
		RAY_LOG_DEBUG("[ThreadPool] Destroyed");
	}

	void ThreadPool::InfiniteWait()
	{
		RAY_PROFILE_FUNCTION();

		while (!m_Shutdown)
		{
			RAY_PROFILE_SCOPE("[ThreadPool] InfiniteWait while(!m_Shutdown) Scope");

			RAY_LOG_DEBUG("[ThreadPool] Thread {0} entered ThreadPool::InfiniteWait", std::this_thread::get_id());
			std::function<void()> task;
			{
				std::scoped_lock lock(m_QueueMutex);
				m_TaskQueue.WaitFor([this]() { return !m_TaskQueue.Empty() || m_Shutdown; });
				
				// Need to check again if waiting thread was restored because of m_Shutdown
				if(!m_TaskQueue.Empty())
					task = m_TaskQueue.PopFront();
			}
			if (task)
			{
				RAY_LOG_DEBUG("[ThreadPool] Thread {0} Task {1} Execution started", std::this_thread::get_id(), &task);
				task();
				RAY_LOG_DEBUG("[ThreadPool] Thread {0} Task {1} Execution finnished", std::this_thread::get_id(), &task);
			}
		}
	}
}


