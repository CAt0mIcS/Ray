#include "Rpch.h"
#include "RThreadPool.h"

#include <mutex>
#include <thread>

#include "Utils/RLogger.h"


namespace At0::Ray
{
	const uint32_t ThreadPool::s_MaxThreads = std::thread::hardware_concurrency();

	ThreadPool::ThreadPool() : m_Threads(MakeScope<std::thread[]>(s_MaxThreads)), m_Shutdown(false)
	{
		Log::Info("[ThreadPool] Initialized {0} threads", NumThreads());
		for (uint16_t i = 0; i < s_MaxThreads; ++i)
		{
			m_Threads[i] = std::thread([this]() { InfiniteWait(); });
		}
	}

	void ThreadPool::WaitForTasks()
	{
		// RAY_TODO:
		// if (m_TaskQueue.empty())
		//	return;

		// std::unique_lock lock(m_QueueMutex);
		// m_Condition.wait(lock, [this]() { return m_TaskQueue.empty(); });
		while (!m_TaskQueue.empty())
		{
		}
	}

	void ThreadPool::Shutdown()
	{
		Log::Info("[ThreadPool] Shutting down");
		{
			std::scoped_lock lock(m_PoolMutex);
			m_Shutdown = true;
		}

		m_Condition.notify_all();

		for (uint16_t i = 0; i < s_MaxThreads; ++i)
		{
			auto id = m_Threads[i].get_id();
			m_Threads[i].join();
			Log::Info("[ThreadPool] Thread {0} joined", id);
		}
	}

	ThreadPool::~ThreadPool()
	{
		if (!m_Shutdown)
			Shutdown();
		Log::Info("[ThreadPool] Destroyed");
	}

	void ThreadPool::InfiniteWait()
	{
		while (!m_Shutdown)
		{
			std::function<void()> task;
			{
				std::unique_lock lock(m_QueueMutex);
				m_Condition.wait(lock, [this]() { return !m_TaskQueue.empty() || m_Shutdown; });

				if (m_Shutdown)
					return;
				task = m_TaskQueue.front();
				m_TaskQueue.pop();
			}
			task();
		}
	}
}  // namespace At0::Ray
