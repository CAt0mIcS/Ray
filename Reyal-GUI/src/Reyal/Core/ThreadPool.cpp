#include "rlpch.h"
#include "ThreadPool.h"

#include <mutex>


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

		{
			std::unique_lock lock(m_PoolMutex);
			m_Shutdown = true;
		}
		m_WaitCondition.notify_all();

		for (uint16_t i = 0; i < m_MaxThreads; ++i)
		{
			m_Threads[i].join();
		}
	}

	ThreadPool::~ThreadPool()
	{
		ZL_PROFILE_FUNCTION();

		if (!m_Shutdown)
			Shutdown();
	}

	void ThreadPool::InfiniteWait()
	{
		ZL_PROFILE_FUNCTION();

		while (!m_Shutdown)
		{
			std::function<void()> task;
			{
				std::unique_lock lock(m_QueueMutex);
				m_WaitCondition.wait(lock, [this]() { return !m_TaskQueue.Empty() || m_Shutdown; });
				
				// Need to check again if waiting thread was restored because of m_Shutdown
				if(!m_TaskQueue.Empty())
					task = m_TaskQueue.PopFront();
			}
			if(task)
				task();
		}
	}
}


