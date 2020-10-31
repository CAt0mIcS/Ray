#include "rlpch.h"
#include "ThreadPool.h"

#include <mutex>
#include "Reyal/Debug/ReyalLogger.h"


namespace At0::Reyal
{
	ThreadPool::ThreadPool()
		: m_Threads(MakeScope<std::thread[]>(m_MaxThreads)), m_Shutdown(false)
	{
		ZL_PROFILE_FUNCTION();

		for (uint16_t i = 0; i < m_MaxThreads; ++i)
		{
			m_Threads[i] = std::thread([this]() { InfiniteWait(); });
		}
	}
	
	void ThreadPool::Shutdown()
	{
		ZL_PROFILE_FUNCTION();

		ZL_LOG_DEBUG("[ThreadPool] Shutting down");
		{
			std::unique_lock lock(m_PoolMutex);
			m_Shutdown = true;
		}
		m_WaitCondition.notify_all();

		ZL_LOG_DEBUG("[ThreadPool] Joining Threads");
		for (uint16_t i = 0; i < m_MaxThreads; ++i)
		{
			auto id = m_Threads[i].get_id();
			ZL_LOG_DEBUG("[ThreadPool] Joining Thread {0}", id);
			m_Threads[i].join();
			ZL_LOG_DEBUG("[ThreadPool] Thread {0} joined", id);
		}
		ZL_LOG_DEBUG("[ThreadPool] Finnished joining Threads");
	}

	ThreadPool::~ThreadPool()
	{
		ZL_PROFILE_FUNCTION();

		if (!m_Shutdown)
		{
			ZL_LOG_DEBUG("[ThreadPool] Calling ThreadPool::Shutdown from Deconstructor");
			Shutdown();
		}
	}

	void ThreadPool::InfiniteWait()
	{
		ZL_PROFILE_FUNCTION();

		while (!m_Shutdown)
		{
			ZL_LOG_DEBUG("[ThreadPool] Thread {0} entered ThreadPool::InfiniteWait", std::this_thread::get_id());
			std::function<void()> task;
			{
				std::unique_lock lock(m_QueueMutex);
				m_WaitCondition.wait(lock, [this]() { return !m_TaskQueue.Empty() || m_Shutdown; });
				
				// Need to check again if waiting thread was restored because of m_Shutdown
				if(!m_TaskQueue.Empty())
					task = m_TaskQueue.PopFront();
			}
			if (task)
			{
				ZL_LOG_DEBUG("[ThreadPool] Thread {0} Task {1} Execution started", std::this_thread::get_id(), &task);
				task();
				ZL_LOG_DEBUG("[ThreadPool] Thread {0} Task {1} Execution finnished", std::this_thread::get_id(), &task);
			}
		}
	}
}


