#include "Rpch.h"
#include "RThreadPool.h"

#include <mutex>
#include <thread>

#include "Utils/RLogger.h"

// RAY_TEMPORARY
#include "Core/RTime.h"


namespace At0::Ray
{
	const uint32_t ThreadPool::s_ThreadCount = std::thread::hardware_concurrency();

	ThreadPool::ThreadPool() : m_Threads(MakeScope<std::thread[]>(s_ThreadCount)), m_Shutdown(false)
	{
		Log::Info("[ThreadPool] Initialized {0} threads", s_ThreadCount);
		for (uint16_t i = 0; i < s_ThreadCount; ++i)
		{
			m_Threads[i] = std::thread([this]() { InfiniteWait(); });
		}
	}

	void ThreadPool::WaitForTasks()
	{
		// Time tStart = Time::Now();
		// RAY_TODO:
		while (m_TasksRunning != 0)
		{
		}

		// CLog::Debug("[ThreadPool] Waited for {0}us", (Time::Now() - tStart).AsMicroseconds());
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

		for (uint16_t i = 0; i < s_ThreadCount; ++i)
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
				++m_TasksRunning;

				if (m_Shutdown)
				{
					--m_TasksRunning;
					return;
				}

				task = m_TaskQueue.front();
				m_TaskQueue.pop();
			}
			task();
			--m_TasksRunning;
		}
	}
}  // namespace At0::Ray
