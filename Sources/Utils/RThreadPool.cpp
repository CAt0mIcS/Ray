#include "Rpch.h"
#include "RThreadPool.h"

#include <mutex>
#include <thread>

#include "Utils/RLogger.h"


namespace At0::Ray
{
	ThreadPool::ThreadPool(uint32_t threadCount)
		: m_ThreadCount(threadCount), m_Threads(MakeScope<std::thread[]>(threadCount)),
		  m_Shutdown(false)
	{
		Log::Info("[ThreadPool] Initialized {0} thread(s)", m_ThreadCount);
		for (uint16_t i = 0; i < m_ThreadCount; ++i)
		{
			m_Threads[i] = std::thread([this]() { InfiniteWait(); });
		}
	}

	void ThreadPool::WaitForTasks()
	{
		std::unique_lock lock(m_QueueMutex);
		m_TaskFinished.wait(
			lock, [this]() { return m_TaskQueue.empty() && (m_TasksRunning == 0); });
	}

	uint32_t ThreadPool::GetTasksQueued() const
	{
		std::scoped_lock lock(m_QueueMutex);
		return m_TaskQueue.size();
	}

	void ThreadPool::Shutdown()
	{
		Log::Info("[ThreadPool] Shutting down");
		{
			std::scoped_lock lock(m_PoolMutex);
			m_Shutdown = true;
		}

		m_Condition.notify_all();

		for (uint16_t i = 0; i < m_ThreadCount; ++i)
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
			std::unique_lock lock(m_QueueMutex);
			m_Condition.wait(lock, [this]() { return m_Shutdown || !m_TaskQueue.empty(); });

			if (m_Shutdown)
				break;

			++m_TasksRunning;

			// Pull from queue
			auto fn = m_TaskQueue.front();
			m_TaskQueue.pop_front();

			// Release lock, run task async
			lock.unlock();

			// Run function outside context
			fn();

			lock.lock();
			--m_TasksRunning;
			m_TaskFinished.notify_one();
		}
	}
}  // namespace At0::Ray
